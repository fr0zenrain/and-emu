#include "../jvm/jvm.h"
#include "runtime.h"
#include "emulator.h"
#include "string.h"
#include "stdlib.h"
#include "../type.h"
#include "../engine.h"
#include "../../capstone/include/capstone.h"
#include "time.h"
#include "ctype.h"
#include "dirent_win.h"
#include "zlib.h"
#include <sys/stat.h>
#include "math.h"

#ifdef _MSC_VER
#include "io.h"
#include <fcntl.h>
#include "windows.h"
#else
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "unistd.h"
#include <netdb.h>
#include "sys/socket.h"
#endif

extern uc_engine* g_uc;
soinfo* libc::si =0;
extern void* s_dlsym(void* handle, const char* symbol);

void libc::init(emulator* emu)
{
    si = soinfo_alloc("libc.so");
    si->emu = 1;
    si->emulator = emu;
    si->load_bias = FUNCTION_VIRTUAL_ADDRESS;
	si->base = FUNCTION_VIRTUAL_ADDRESS;
    si->flags |= FLAG_LINKED;
}

void* libc::s_malloc(void*)
{
    int size = emulator::get_r0();
	void* addr = sys_malloc(size);

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("malloc(0x%x) ->0x%x\n",size,addr);
#else
    printf(RED "malloc(0x%x) ->0x%x\n" RESET,size,addr);
#endif
    uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	return 0;
}

void* libc::s_calloc(void*)
{
    int count = emulator::get_r0();
    int size = emulator::get_r1();
    void* addr = sys_malloc(size*count);

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("calloc(0x%x, 0x%x) ->0x%x\n", count, size,addr);
#else
    printf(RED "calloc(0x%x, 0x%x) ->0x%x\n" RESET,count, size,addr);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
    return 0;
}


void* libc::s_free(void*)
{
    unsigned int ptr = emulator::get_r0();

	emulator::update_cpu_model();
	
	//if(ptr)
	//	sys_free((void*)ptr); //FIXME free has a bug

#ifdef _MSC_VER
	printf("free(0x%x)\n",ptr);
#else
	printf(RED "free(0x%x)\n" RESET,ptr);
#endif

	return 0;
}

void* libc::s_memset(void*) {
    unsigned int addr = emulator::get_r0();
    int value = emulator::get_r1();
    int size = emulator::get_r2();
    for (int i = 0; i < size; i++) {
        uc_mem_write(g_uc, addr, &value, 1);
    }

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("memset(0x%x,0x%x,0x%x) -> 0x%x\n",addr,value,size, addr);
#else
	printf(RED "memset(0x%x,0x%x,0x%x) -> 0x%x\n" RESET,addr,value,size, addr);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);

	return 0;
}

void* libc::s__aeabi_memset(void*)
{
	uc_err err;
	unsigned int addr = emulator::get_r0();
	int value =emulator::get_r2();
    int size = emulator::get_r1();

	for(int i = 0; i < size; i++)
	{
		err = uc_mem_write(g_uc,addr+i,&value,1);
	}
    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x\n",addr,size,value,addr);
#else
    printf(RED "__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x \n" RESET,addr,size,value,addr);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);

	return 0;
}

void* libc::s__aeabi_memcpy(void*)
{
    uc_err err;
	unsigned int size = emulator::get_r2();
    unsigned int src = emulator::get_r1();
    unsigned int dst = emulator::get_r0();

    unsigned char* src_buf = (unsigned char*)malloc(size+1);
	if(src_buf)
	{
        memset(src_buf, 0, size+1);
		err = uc_mem_read(g_uc,src,src_buf,size);
        err = uc_mem_write(g_uc,dst,src_buf,size);
	}

    if (emulator::is_data_printable(src_buf, size) ) {
#ifdef _MSC_VER
        printf("__aeabi_memcpy(\"%s\",\"%s\",0x%x)-> 0x%x\n",src_buf,src_buf,size,dst);
#else
        printf(RED "__aeabi_memcpy(\"%s\",\"%s\",0x%x)-> 0x%x\n" RESET, src_buf, src_buf, size, dst);
#endif
    }else{
#ifdef _MSC_VER
        printf("__aeabi_memcpy(0x%x,0x%x,0x%x)-> 0x%x\n",dst,src,size,dst);
#else
        printf(RED "__aeabi_memcpy(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, dst, src, size, dst);
#endif
        print_hex_dump_bytes(src_buf,0x10);
    }

    if(src_buf)
        free(src_buf);

	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&dst);

	return 0;
}

void* libc::sys_mmap(int type)
{
    uc_err err;
    unsigned int addr = emulator::get_r0();
    int size = emulator::get_r1();
    int prot = emulator::get_r2();
    int flags = emulator::get_r3();
    int fd = emulator::get_r4();
    int offset = emulator::get_r5();

	if(addr == 0)
	{
        if (fd < -1){
            fd = -1;
        }
		addr = (unsigned int)s_mmap(NULL,size,PROT_NONE,MAP_PRIVATE, fd, offset);
		if (addr == -1){
			printf("mmap failed %s\n", strerror(errno));
		}
	}

#ifdef _MSC_VER
	printf("mmap(0x%x,0x%x,0x%x,0x%x,0x%x)-> 0x%x\n",addr,size,prot,fd,offset,addr);
#else
	printf(RED "mmap(0x%x,0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET,addr,size,prot,fd,offset,addr);
#endif

	if(type == 0)
	{
		err = uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
		return 0;
	}

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	emulator::update_cpu_model();

	return 0;
}

void* libc::sys_cacheflush(int type)
{
    int value = 0;
    unsigned int addr = emulator::get_r0();

	if(type == 0)
		uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);

#ifdef _MSC_VER
	printf("cacheflush(0x%x,0x%x,0x%x)-> 0x%x\n",addr,emulator::get_r1(),emulator::get_r2(),value);
#else
	printf(RED "cacheflush(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,addr,emulator::get_r1(),emulator::get_r2(),value);
#endif

	if(type == 0)
	{
		uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
		return 0;
	}

	emulator::update_cpu_model();
	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::sys_dlopen(void*)
{
	uc_err err;
	soinfo* si = 0;
	char buf[256] ={0};
    unsigned int addr = emulator::get_r0();
	int flags = emulator::get_r1();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}
    if (strcmp(buf,"libdl.so") == 0)
    {
        si = emulator::get_fake_solist();
    }
	else{
        si = (soinfo*)s_dlopen(buf,0);
    }
#ifdef _MSC_VER
	printf("dlopen(\"%s\",0x%x)-> 0x%x\n",buf,flags,si);
#else
	printf(RED "dlopen(\"%s\",0x%x)-> 0x%x\n" RESET,buf,flags,si);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&si);

    emulator::update_cpu_model();

	return 0;
}

void* libc::sys_dlclose(void*)
{
	int value = 0;
	unsigned int addr = emulator::get_r0();

#ifdef _MSC_VER
	printf("dlclose(0x%x)-> 0x%x\n",addr,value);
#else
	printf(RED "dlclose(0x%x)-> 0x%x\n" RESET,addr,value);
#endif
	uc_reg_write(g_uc,UC_ARM_REG_R0,&si);

	emulator::update_cpu_model();

	return 0;
}

void* libc::sys_dlsym(void*)
{
	uc_err err;
	int value = 0;
	char buf[256] = {0};
    Elf32_Sym* sym = NULL;
	unsigned int addr = emulator::get_r0();
	unsigned int sym_addr = emulator::get_r1();
	err = uc_mem_read(g_uc, sym_addr, buf, 256);
    value = (int)s_dlsym((void*)addr, buf);
	if (value == 0){
        sym = emulator::get_symbols(buf);
        value = sym->st_value + FUNCTION_VIRTUAL_ADDRESS;
    }

#ifdef _MSC_VER
    printf("dlsym(0x%x, \"%s\")-> 0x%x\n",addr,buf, value);
#else
    printf(RED "dlsym(0x%x, \"%s\")-> 0x%x\n" RESET,addr,buf, value);
#endif

    if(value)
    {
        //hook for performance
        if (strcmp(buf,"uncompress") == 0){
            sym = emulator::get_symbols(buf);
            value = sym->st_value + FUNCTION_VIRTUAL_ADDRESS;
#ifdef _MSC_VER
            printf("hook %s with 0x%x\n",buf, value);
#else
            printf(RED "hook %s with 0x%x\n" RESET,buf, value);
#endif
        }
    }

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	emulator::update_cpu_model();

	return 0;
}


void* libc::sys_dladdr(void*)
{
    uc_err err;
    int value = 1;
    Dl_info info;
    unsigned int addr = emulator::get_r0();
    unsigned int dlinfo = emulator::get_r1();
    info.dli_fname =(char*)sys_malloc(128);
    info.dli_fbase = (void*)emulator::get_main_module()->load_bias;

    err = uc_mem_write(g_uc, (uint64_t)info.dli_fname, "libjiagu.so", 11);
    err = uc_mem_write(g_uc, dlinfo, &info, sizeof(Dl_info));

#ifdef _MSC_VER
    printf("dladdr(0x%x,0x%x)-> 0x%x\n",addr, dlinfo, value);
#else
    printf(RED "dladdr(0x%x,0x%x)-> 0x%x\n" RESET,addr, dlinfo, value);
#endif
    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    emulator::update_cpu_model();

    return 0;
}


void* libc::s__system_property_get(void*)
{
	int ret = 0;
	char name[92] ={0};
	char value[92] ={0};
    unsigned int name_addr = emulator::get_r0();
    unsigned int value_addr = emulator::get_r1();

	if(name_addr && value_addr)
	{
		uc_mem_read(g_uc,name_addr,name,92);
		ret = get_prop(name,value);
		if(ret)
		{
			uc_mem_write(g_uc,value_addr,value,ret+1);
		}
	}
#ifdef _MSC_VER
	printf("__system_property_get(\"%s\",\"%s\")\n",name,value);
#else
	printf(RED "__system_property_get(\"%s\",\"%s\")\n" RESET,name,value);
#endif

    emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return 0;
}

void* libc::s_gettimeofday(void*)
{
	struct s_timeval tv;
	int value = 0;

    unsigned int addr = emulator::get_r0();
	unsigned int tz = emulator::get_r1();

	if(addr)
	{
		tv.tv_sec = 0x5bdbd3cc;
		tv.tv_usec = 960256;

		uc_mem_write(g_uc,addr,&tv,sizeof(s_timeval));
	}

	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("gettimeofday(%x,0x%x)-> 0x%x\n",addr,emulator::get_r1(),value);
#else
	printf(RED "gettimeofday(%x,0x%x)-> 0x%x\n" RESET,addr,emulator::get_r1(),value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strdup(void*)
{
	uc_err err;
	char buf[256] ={0};
	int value = 0;

    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}
	int len = strlen(buf);
	value = (int)sys_malloc(len+1);
	err = uc_mem_write(g_uc,value,buf,len+1);

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("strdup(\"%s\")-> 0x%x\n",buf,value);
#else
	printf(RED "strdup(\"%s\")-> 0x%x\n" RESET,buf,value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strlen(void*)
{
	uc_err err;
	int i = 0;
	char buf[512] ={0};
    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		for(i = 0; i < 512; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

    emulator::update_cpu_model();
    int len = strlen(buf);
    if (emulator::is_data_printable((unsigned char*)buf, len)) {
#ifdef _MSC_VER
        printf("strlen(\"%s\")-> 0x%x\n",buf,i);
#else
        printf(RED "strlen(\"%s\")-> 0x%x\n" RESET, buf, i);
#endif
    }
    else{
#ifdef _MSC_VER
        printf("strlen(0x%x)-> 0x%x\n",addr,i);
#else
        printf(RED "strlen(0x%x)-> 0x%x\n" RESET, addr, i);
#endif
        print_hex_dump_bytes(buf,0x10);
    }

	uc_reg_write(g_uc,UC_ARM_REG_R0,&i);

	return 0;
}

void* libc::s_strncmp(void*)
{
	uc_err err;
	int value = 0;
	char buf[256] ={0};
	char buf1[256] ={0};

    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();
    int size = emulator::get_r2();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	if(addr1)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr1+i,&buf1[i],1);
			if(buf1[i] == 0)
				break;
		}
	}

	value = strncmp(buf,buf1,size);

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("strncmp(\"%s\",\"%s\",0x%x)-> 0x%x\n",buf,buf1,size,value);
#else
	printf(RED "strncmp(\"%s\",\"%s\",0x%x)-> 0x%x\n" RESET,buf,buf1,size,value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strcmp(void*)
{
    uc_err err;
    int value = 0;
    char buf[256] ={0};
    char buf1[256] ={0};
    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();

    if(addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,addr+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }

    if(addr1)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,addr1+i,&buf1[i],1);
            if(buf1[i] == 0)
                break;
        }
    }

    value = strcmp(buf,buf1);

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("strcmp(\"%s\",\"%s\")-> 0x%x\n",buf,buf1,value);
#else
    printf(RED "strcmp(\"%s\",\"%s\")-> 0x%x\n" RESET,buf,buf1,value);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s_open(void*)
{
	uc_err err;
	char buf[256] ={0};
    char bak_path[256] = {0};
	char cur_dir[512] ={0};
	int value = 1;

    unsigned int addr = emulator::get_r0();
	int mode = emulator::get_r1();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

#ifdef _MSC_VER
	GetCurrentDirectory(1024,cur_dir);
#else
	getcwd(cur_dir,512);
#endif
    strcpy(bak_path,buf);
    if (strncmp(buf, "/data/data", 10) == 0){
        char tmp[256] ={0};
        char* ch = strtok(buf, "/");
        while (ch != NULL) {
            strcpy(tmp, ch);
            ch = strtok(NULL, "/");
        }
        strcpy(cur_dir,"/tmp/");
        strcat(cur_dir,tmp);
        value = (int)open(cur_dir,O_RDWR|O_CREAT, 0666);
    }
    if (strncmp(buf, "/", 1) == 0){
        strcat(cur_dir,buf);
        value = (int)open(cur_dir,mode);
    }

#ifdef _MSC_VER
	printf("open(\"%s\", 0x%x)-> 0x%x\n",bak_path, mode, value);
#else
	printf(RED "open(\"%s\", 0x%x)-> 0x%x\n" RESET,bak_path,mode,value);
#endif
    printf("redirect to %s\n", cur_dir);
	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_read(void*)
{
	uc_err err;
	char buf[4096] ={0};
	int value = 0;
    unsigned int fd = emulator::get_r0();
    unsigned int addr = emulator::get_r1();
    unsigned int size = emulator::get_r2();

    if(addr)
    {
        value = read(fd,buf,size);
        err = uc_mem_write(g_uc,addr,buf,size);
    }

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("read(%x,%x,%x)-> 0x%x\n",fd,addr,size,value);
#else
	printf(RED "read(%x,%x,%x)-> 0x%x\n" RESET,fd,addr,size,value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_write(void*)
{
    uc_err err;
    int value = 0;
    unsigned char buf[1024] ={0};
    int fd = emulator::get_r0();
    int addr = emulator::get_r1();
	unsigned int size = emulator::get_r2();

    if (addr)
    {
        for (int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc, addr + i, &buf[i], 1);
            if (buf[i] == 0)
                break;
        }
        value = write(fd, buf, size);
    }
		
#ifdef _MSC_VER
		printf("write(0x%x,%s,0x%x)-> 0x%x\n", fd, buf, size, value);
#else
		printf(RED "write(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, fd, addr, size, value);
#endif
    print_hex_dump_bytes(buf,0x10);

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}


void* libc::s_close(void*)
{
    int value = 0;
    unsigned int fd = emulator::get_r0();

	value = close(fd);
    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("close(%x)-> 0x%x\n",fd,value);
#else
    printf(RED "close(%x)-> 0x%x\n" RESET,fd,value);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::sys_mprotect(void* type)
{
	uc_err err;
	int value = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();
    unsigned int size = emulator::get_r2();
	
#ifdef _MSC_VER
	printf("mprotect(0x%x,0x%x,0x%x)-> 0x%x\n",addr,addr1,size,value);
#else
	printf(RED "mprotect(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,addr,addr1,size,value);
#endif

	if(type == 0)
	{
		err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
		return 0;
	}

    emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_sscanf(void*)
{
	uc_err err ;
	char buf[256] ={0};
	char format[256] ={0};
	unsigned int stack[10] ={0};
	char r2_buf[256]={0};
	char r3_buf[256]={0};
	char arg4[256]={0};
	char arg5[256]={0};
	char arg6[256]={0};
	char arg7[256]={0};
	char arg8[256]={0};
	char arg9[256]={0};

	int value = 1;
    int count = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int addr_format = emulator::get_r1();
    unsigned int r2_addr = emulator::get_r2();
    unsigned int r3_addr = emulator::get_r3();

    if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0 || buf[i] == '\r' || buf[i] == '\n')
            {
                buf[i] = 0;
                break;
            }
		}
	}

	if(addr_format)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr_format+i,&format[i],1);
			if(format[i] == 0)
				break;
		}
	}

#ifdef _MSC_VER
	printf("sscanf (\"%s\",\"%s\",...)-> 0x%x\n",buf,format,value);
#else
	printf(RED "sscanf(\"%s\",\"%s\",...)-> 0x%x\n" RESET,buf,format,value);
#endif

	for(int i = 0; i < 8; i++)
	{
		err = uc_mem_read(g_uc,emulator::get_sp()+i*4,&stack[i],4);
        count++;
		if(stack[i] == 0)
			break;
	}

	sscanf(buf,format,r2_buf, r3_buf, arg4, arg5, arg6, arg7, arg8, arg9);

	err = uc_mem_write(g_uc,r2_addr,r2_buf,4);
    err = uc_mem_write(g_uc,r3_addr,r3_buf,4);

    emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strchr(void*)
{
	uc_err err ;
	char buf[256] ={0};
	char ch = (char)emulator::get_r1();
	unsigned int value = 0;
    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	char* ptr = strchr(buf,ch);
	if(ptr)
	{
		value = (unsigned int)ptr - (int)buf + addr;
	}

#ifdef _MSC_VER
	printf("strchr (\"%s\",\"%c\")-> 0x%x\n",buf,ch,value);
#else
	printf(RED "strchr(\"%s\",\"%c\")-> 0x%x\n" RESET,buf,ch,value);
#endif

    emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strtoul(void*)
{
	uc_err err;
	char buf[256]={0};
    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	const char *nptr = buf;
	char* tp = &buf[256];
	char **endptr = &tp;
	int base = emulator::get_r2();
	const char *s = nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;


	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	} else if ((base == 0 || base == 2) &&
	    c == '0' && (*s == 'b' || *s == 'B')) {
		c = s[1];
		s += 2;
		base = 2;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
//		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *)(any ? s - 1 : nptr);

#ifdef _MSC_VER
	printf("strtoul (\"%s\",0x%x,0x%x)-> 0x%x\n",buf,emulator::get_r1(),base,acc);
#else
	printf(RED "strtoul(\"%s\",0x%x,0x%x)-> 0x%lx\n" RESET,buf,emulator::get_r1(),base,acc);
#endif

    emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&acc);
	return 0;
}

void* libc::s_bsd_signal(void*)
{
    uc_err err;
    int value = 0;
    unsigned int sig = emulator::get_r0();
    unsigned int addr = emulator::get_r1();

	if(addr)
	{
		emulator::get_emulator()->save_signal_handler(sig,(void*)addr);
	}
	else
	{
		value = -1;
	}

#ifdef _MSC_VER
	printf("bsd_signal(0x%x,0x%x)-> 0x%x\n",sig,addr, value);
#else
	printf(RED "bsd_signal(0x%x,0x%x)-> 0x%x\n" RESET,sig,addr,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_raise(void*)
{
    uc_err err;
    int value = 0;
    unsigned int sig = emulator::get_r0();

#ifdef _MSC_VER
    printf("raise(0x%x)-> 0x%x\n",sig,  value);
#else
    printf(RED "raise(0x%x)-> 0x%x\n" RESET,sig, value);
#endif

	emulator::update_cpu_model();
    //emulator::get_emulator()->process_signal(sig);
    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s_getpid(void*)
{
    uc_err err;
    int value = emulator::get_module_pid();

#ifdef _MSC_VER
    printf("getpid()-> 0x%x\n",  value);
#else
    printf(RED "getpid()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_strcpy(void*) {
    uc_err err;
    int value = 0;

    char buf[512] = {0};
    char buf1[512] = {0};

    unsigned int dst = emulator::get_r0();
    unsigned int src = emulator::get_r1();

    if (dst) {
        for (int i = 0; i < 512; i++) {
            err = uc_mem_read(g_uc, dst + i, &buf[i], 1);
            if (buf[i] == 0)
                break;
        }
    }

    if (src) {
        for (int i = 0; i < 512; i++) {
            err = uc_mem_read(g_uc, src + i, &buf1[i], 1);
            if (buf1[i] == 0)
                break;
        }
    }
    int len = strlen(buf1);
    if (emulator::is_data_printable((unsigned char*)buf, len) ||
            emulator::is_data_printable((unsigned char*)buf1, len))
    {
#ifdef _MSC_VER
    printf("strcpy(\"%s\",\"%s\")-> 0x%x\n", buf, buf1, dst);
#else
    printf(RED "strcpy(\"%s\",\"%s\")-> 0x%x\n" RESET, buf, buf1, dst);
#endif
    }
    else{
#ifdef _MSC_VER
        printf("strcpy(0x%x,0x%x)-> 0x%x\n", dst, src, dst);
#else
        printf(RED "strcpy(0x%x,0x%x)-> 0x%x\n" RESET, dst, src, dst);
#endif
        print_hex_dump_bytes(buf,0x10);
        print_hex_dump_bytes(buf1,0x10);
    }

    strcpy(buf, buf1);
    err = uc_mem_write(g_uc, dst, buf, strlen(buf));

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s__errno(void*)
{
    uc_err err;
    int value = 0;
    char buf[512] ={0};
    unsigned int dst = emulator::get_r0();

#ifdef _MSC_VER
    printf("__errno(0x%x,%s)-> 0x%x\n", dst, buf, value);
#else
    printf(RED "__errno(0x%x,%s)-> 0x%x\n" RESET, dst, buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_strerror(void*)
{
    uc_err err;
    int value = 0;

    char buf[512] ={0};
    unsigned int dst = emulator::get_r0();

#ifdef _MSC_VER
    printf("strerror(0x%x,%s)-> 0x%x\n", dst, buf, value);
#else
    printf(RED "strerror(0x%x,%s)-> 0x%x\n" RESET, dst, buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_access(void*)
{
    uc_err err;
    int value = 0;

    char buf[512] ={0};
    unsigned int path = emulator::get_r0();
    unsigned int mode = emulator::get_r1();

    if(path)
    {
        for(int i = 0; i < 512; i++)
        {
            err = uc_mem_read(g_uc,path+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }


#ifdef _MSC_VER
    printf("access(\"%s\",0x%x)-> 0x%x\n", buf, mode,value);
#else
    printf(RED "access(\"%s\",0x%x)-> 0x%x\n" RESET, buf,mode, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_abort(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("abort()-> 0x%x\n", value);
#else
    printf(RED "abort()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_strcat(void*)
{
    uc_err err;
    int value = 0;
    char buf[512] ={0};

    unsigned int dst = emulator::get_r0();
    unsigned int src = emulator::get_r1();

    if(src)
    {
        for(int i = 0; i < 512; i++)
        {
            err = uc_mem_read(g_uc,src+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }

#ifdef _MSC_VER
    printf("strcat(0x%x,\"%s\")-> 0x%x\n", dst,buf, value);
#else
    printf(RED "strcat(0x%x,\"%s\")-> 0x%x\n" RESET, dst, buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_lseek(void*)
{
    uc_err err;
    int value = 0;
    char buf[512] ={0};
    int fd = emulator::get_r0();
    unsigned int offset = emulator::get_r1();
    int whence = emulator::get_r2();
    value = lseek(fd, offset, whence);

#ifdef _MSC_VER
    printf("lseek(0x%x,0x%x,0x%x)-> 0x%x\n", fd, offset, whence, value);
#else
    printf(RED "lseek(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, fd, offset, whence, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_mutex_init(void*)
{
	uc_err err;
	int value = 0;
	unsigned int mutex = emulator::get_r0();

#ifdef _MSC_VER
	printf("pthread_mutex_init(0x%x)-> 0x%x\n", mutex, value);
#else
	printf(RED "pthread_mutex_init(0x%x)-> 0x%x\n" RESET, mutex, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_pthread_mutex_lock(void*)
{
    uc_err err;
    int value = 0;
    unsigned int mutex = emulator::get_r0();

#ifdef _MSC_VER
    printf("pthread_mutex_lock(0x%x)-> 0x%x\n", mutex, value);
#else
    printf(RED "pthread_mutex_lock(0x%x)-> 0x%x\n" RESET, mutex, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_mutex_unlock(void*)
{
    uc_err err;
    int value = 0;
    unsigned int mutex = emulator::get_r0();

#ifdef _MSC_VER
    printf("pthread_mutex_unlock(0x%x)-> 0x%x\n", mutex, value);
#else
    printf(RED "pthread_mutex_unlock(0x%x)-> 0x%x\n" RESET, mutex, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_snprintf(void*)
{
    uc_err err;
    int value = 0;
    char buf[1024] = {0};
    char format[128] ={0};

    unsigned int buf_addr = emulator::get_r0();
    int max = emulator::get_r1();
    unsigned int format_addr = emulator::get_r2();

    if(buf_addr)
    {
        for(int i = 0; i < 1024; i++)
        {
            err = uc_mem_read(g_uc,buf_addr+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }

    if(format_addr)
    {
        for(int i = 0; i < 128; i++)
        {
            err = uc_mem_read(g_uc,format_addr+i,&format[i],1);
            if(format[i] == 0)
                break;
        }
    }

#ifdef _MSC_VER
    printf("snprintf(\"%s\",0x%x,\"%s\",...)-> 0x%x\n",buf, max, format,  value);
#else
    printf(RED "snprintf(\"%s\",0x%x,\"%s\",...)-> 0x%x\n" RESET, buf, max, format, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pipe(void*)
{
    uc_err err;
    int value = 0;
	unsigned int pipe_addr = emulator::get_r0();
	int fd;
	
	fd = (int)fopen("rd.pipe","wb" );

	err = uc_mem_write(g_uc,pipe_addr,&fd,4);
	err = uc_mem_write(g_uc,pipe_addr+4,&fd,4);

#ifdef _MSC_VER
    printf("pipe(0x%x)-> 0x%x\n", pipe_addr, value);
#else
    printf(RED "pipe(0x%x)-> 0x%x\n" RESET, pipe_addr, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_fork(void*)
{
    uc_err err;
    int value = 0;
    //value = fork();

#ifdef _MSC_VER
    printf("fork()-> 0x%x\n",  value);
#else
    printf(RED "fork()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_sysconf(void*)
{
	uc_err err;
	int value = 0;
	int name = emulator::get_r0();
	
	if(name == 0x27 || name == 0x28)
	{
		value = 0x1000;
	}
	
#ifdef _MSC_VER
		printf("sysconf(0x%x)-> 0x%x\n", name, value);
#else
		printf(RED "sysconf(0x%x)-> 0x%x\n" RESET, name, value);
#endif

	emulator::update_cpu_model();
	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_fopen(void*)
{
	uc_err err;
	int value = 1;
	char path[512] ={0};
	char mode[16] ={0};
    char cur_dir[1024] = {0};
	unsigned int path_addr = emulator::get_r0();
	unsigned int mode_addr = emulator::get_r1();

    if(path_addr)
    {
        for(int i = 0; i < 512; i++)
        {
            err = uc_mem_read(g_uc,path_addr+i,&path[i],1);
            if(path[i] == 0)
                break;
        }
    }

    if(mode_addr)
    {
        for(int i = 0; i < 16; i++)
        {
            err = uc_mem_read(g_uc,mode_addr+i,&mode[i],1);
            if(mode[i] == 0)
                break;
        }
    }

#ifdef _MSC_VER
	GetCurrentDirectory(1024,cur_dir);
#else
	getcwd(cur_dir,1024);
#endif
    if (strncmp(path,"/proc", 5) == 0)
    {
        strcat(cur_dir,path);
        value = (int)fopen(cur_dir,mode);
    }
	else if (strncmp(path, "/system", 7) == 0)
	{
		strcat(cur_dir, path);
		value = (int)fopen(cur_dir, mode);
	}
	else
	{
		value = 0;
	}

#ifdef _MSC_VER
	printf("fopen(\"%s\",\"%s\")-> 0x%x\n",path, mode,  value);
#else
	printf(RED "fopen(\"%s\",\"%s\")-> 0x%x\n" RESET, path, mode, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_fread(void*)
{
	uc_err err;
	int value = 0;

	FILE* fp = (FILE*)emulator::get_r3();
	void* buffer = (void*)emulator::get_r0();
	int size = emulator::get_r1();
	int count = emulator::get_r2();
	if (buffer)
	{
		value = (int)fread(buffer, size, count, fp);
	}
#ifdef _MSC_VER
	printf("fread(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n",  buffer, size, count, fp, value);
#else
	printf(RED "fread(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET, buffer, size, count, fp, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_fseek(void*)
{
	uc_err err;
	int value = 0;

	FILE* fp = (FILE*)emulator::get_r0();
	int offset = emulator::get_r1();
	int start = emulator::get_r2();

	value = (int)fseek(fp, offset, start);

#ifdef _MSC_VER
	printf("fseek(0x%x,0x%x,0x%x)-> 0x%x\n",  fp, offset, start, value);
#else
	printf(RED "fseek(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, fp, offset, start, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_fclose(void*)
{
	uc_err err;
	int value = 0;

    FILE* fd = (FILE*)emulator::get_r0();
    if(fd)
        fclose(fd);

#ifdef _MSC_VER
	printf("fclose(0x%x)-> 0x%x\n", fd, value);
#else
	printf(RED "fclose(0x%x)-> 0x%x\n" RESET,fd, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_time(void*)
{
	uc_err err;
    int value = 0;
    time_t lt;
    long seed = emulator::get_r0();
    if (seed){
        value = time((time_t*)lt);
        uc_mem_write(g_uc,seed, &lt, sizeof(time_t));
    }
	else
    {
        value = time((time_t*)seed);
    }

#ifdef _MSC_VER
	printf("time(%x)-> 0x%x\n", seed, value);
#else
	printf(RED "time(%x)-> 0x%x\n" RESET, seed, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_sbrk(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("sbrk()-> 0x%x\n",  value);
#else
	printf(RED "sbrk()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s__stack_chk_fail(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("__stack_chk_fail()-> 0x%x\n",  value);
#else
	printf(RED "__stack_chk_fail()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s__stack_chk_guard(void*)
{
    return 0;
}


void* libc::s_opendir(void*)
{
    uc_err err;
    int value = 1;
    char buf[512] ={0};
    char cur_dir[1024] ={0};
    unsigned int path_addr = emulator::get_r0();

    if(path_addr)
    {
        for(int i = 0; i < 512; i++)
        {
            err = uc_mem_read(g_uc,path_addr+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }
    
#ifndef _MSC_VER
	getcwd(cur_dir,1024);
#else
	GetCurrentDirectory(1024,cur_dir);
#endif
	strcat(cur_dir,buf);
	DIRX* dir = opendir_(cur_dir);
	value = (unsigned int)dir;

#ifdef _MSC_VER
    printf("opendir(\"%s\")-> 0x%x\n", buf, value);
#else
    printf(RED "opendir(\"%s\")-> 0x%x\n" RESET, buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_readdir(void*)
{
    uc_err err;
    int value = 0;
    unsigned int dirp = emulator::get_r0();


    direntx* dir = readdir_((DIRX*)dirp);
    value = (unsigned int)dir;

#ifdef _MSC_VER
    printf("readdir(0x%x)-> 0x%x\n", dirp, value);
#else
    printf(RED "readdir(0x%x)-> 0x%x\n" RESET, dirp, value);
	//sys_free(dir);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_closedir(void*)
{
    uc_err err;
    int value = 0;

    unsigned int dirp = emulator::get_r0();

	value = closedir_((DIRX*)dirp);

#ifdef _MSC_VER
    printf("closedir(0x%x)-> 0x%x\n", dirp, value);
#else
    printf(RED "closedir(0x%x)-> 0x%x\n" RESET, dirp, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_getppid(void*)
{
    uc_err err;
    int value = 183;

#ifdef _MSC_VER
    printf("getppid()-> 0x%x\n",  value);
#else
    printf(RED "getppid()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_kill(void*)
{
    uc_err err;
    int value = 0;
    int pid = emulator::get_r0();
    int sig = emulator::get_r1();
    //value = kill(pid, sig);

#ifdef _MSC_VER
    printf("kill(0x%x, 0x%x)-> 0x%x\n", pid, sig, value);
#else
    printf(RED "kill(0x%x, 0x%x)-> 0x%x\n" RESET, pid, sig, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_realloc(void*)
{
	uc_err err;
	unsigned int ptr = emulator::get_r0();
	int new_size = emulator::get_r1();
	unsigned int addr = (unsigned int)sys_malloc(new_size);
	unsigned char value = 0;

	if (addr && ptr)
	{
		for (int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc, ptr + i, &value, 1);
			err = uc_mem_write(g_uc, addr + i, &value, 1);
			if (err)
				break;
		}
	}

	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("realloc(0x%x,0x%x) ->0x%x\n", ptr, new_size, addr);
#else
	printf(RED "realloc(0x%x,0x%x) ->0x%x\n" RESET, ptr, new_size, addr);
#endif

	uc_reg_write(g_uc, UC_ARM_REG_R0, &addr);
	return 0;
}

void* libc::s_atoi(void*)
{
    uc_err err;
    int value = 0;
    char buf[64] = {0};

    unsigned int str_addr = emulator::get_r0();

	if(str_addr)
	{
		for(int i = 0; i < 64; i++)
		{
			err = uc_mem_read(g_uc,str_addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

    value = atoi(buf);

#ifdef _MSC_VER
    printf("atoi(\"%s\")-> 0x%x\n", buf, value);
#else
    printf(RED "atoi(\"%s\")-> 0x%x\n" RESET, buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_itoa(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("itoa()-> 0x%x\n",  value);
#else
    printf(RED "itoa()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_stat(void*)
{
    uc_err err;
    int value = 0;
    struct stat st;
    android_stat ast;
    char path[512] = {0};
    char cur_dir[512] = {0};
    char bak_path[512] = {0};
    unsigned int path_addr = emulator::get_r0();
    unsigned int st_addr = emulator::get_r1();
    if (path_addr){
        uc_mem_read(g_uc, path_addr, path, 512);
    }

#ifdef _MSC_VER
    GetCurrentDirectory(1024,cur_dir);
#else
    getcwd(cur_dir,512);
#endif
    strcpy(bak_path,path);
    if (strncmp(path, "/data/data", 10) == 0){
        char tmp[256] ={0};
        char* ch = strtok(path, "/");
        while (ch != NULL) {
            strcpy(tmp, ch);
            ch = strtok(NULL, "/");
        }
        strcpy(cur_dir,"/tmp/");
        strcat(cur_dir,tmp);
        value = (int)stat(cur_dir,&st);
    }
    if (strncmp(path, "/", 1) == 0){
        strcat(cur_dir,path);
        value = (int)stat(cur_dir,&st);
    }

    ast.st_dev = st.st_dev;
    ast.st_mode = st.st_mode;
    ast.st_blksize = st.st_blksize;
    ast.st_blocks = st.st_blocks;
    ast.st_uid = st.st_uid;
    ast.st_size = st.st_size;
    ast.st_ino = st.st_ino;

    err = uc_mem_write(g_uc, st_addr, &ast, sizeof(struct android_stat));

#ifdef _MSC_VER
    printf("stat(\"%s\",0x%x)-> 0x%x\n", path, st_addr, value);
#else
    printf(RED "stat(\"%s\",0x%x)-> 0x%x\n" RESET, path, st_addr, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_munmap(void*)
{
    uc_err err;
    int value = 0;

	unsigned int addr = emulator::get_r0();
	unsigned int size = emulator::get_r1();

#ifdef _MSC_VER
    printf("munmap(0x%x,0x%x)-> 0x%x\n", addr, size, value);
#else
    printf(RED "munmap(0x%x,0x%x)-> 0x%x\n" RESET, addr, size, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_create(void*)
{
    uc_err err;
    int value = 0;
    unsigned int tid = emulator::get_r0();
    unsigned int attr = emulator::get_r1();
    unsigned int func = emulator::get_r2();
    unsigned int arg = emulator::get_r3();
    unsigned int thread_info = (unsigned int)sys_malloc(16);
    unsigned int thread_sp = emulator::get_emulator()->alloc_thread_stack();
    uc_mem_write(g_uc,thread_info,&func,4);
    uc_mem_write(g_uc,thread_info+4,&arg,4);
    uc_mem_write(g_uc,thread_info+8,&thread_sp,4);
    uc_mem_write(g_uc,tid,&thread_info,4);
#ifdef _MSC_VER
    printf("pthread_create(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n",tid,attr,func,arg, value);
#else
    printf(RED "pthread_create(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET, tid,attr,func,arg,value);
#endif
    printf("thread=0x%x func=0x%x arg=0x%x tsp=0x%x\n", thread_info, func, arg, thread_sp);

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_key_create(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("pthread_key_create()-> 0x%x\n",  value);
#else
	printf(RED "pthread_key_create()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_pthread_key_delete(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("pthread_key_delete()-> 0x%x\n",  value);
#else
	printf(RED "pthread_key_delete()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_pthread_getspecific(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("pthread_getspecific()-> 0x%x\n",  value);
#else
	printf(RED "pthread_getspecific()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_pthread_setspecific(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("pthread_setspecific()-> 0x%x\n",  value);
#else
	printf(RED "pthread_setspecific()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_inotify_init(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("inotify_init()-> 0x%x\n",  value);
#else
    printf(RED "inotify_init()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_inotify_add_watch(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("inotify_add_watch()-> 0x%x\n",  value);
#else
    printf(RED "inotify_add_watch()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_inotify_rm_watch(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("inotify_rm_watch()-> 0x%x\n",  value);
#else
	printf(RED "inotify_rm_watch()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_fgets(void*)
{
    uc_err err;
    unsigned int value = 0;
    char buf[4096] ={0};
    FILE* fd = (FILE*)emulator::get_r2();
    int max = emulator::get_r1();
    unsigned int buf_addr = emulator::get_r0();

    max = (max > 4096)?4096:max;

	if(fd)
	{
		char* tmp = fgets(buf,max,fd);
		if(buf_addr && tmp)
		{
			err = uc_mem_write(g_uc,buf_addr,tmp,strlen(tmp)+1) ;
			value = (int)buf_addr;
		}
	}


#ifdef _MSC_VER
    printf("fgets(\"%s\",0x%x,0x%x)-> 0x%x\n", buf,max,fd, value);
#else
    printf(RED "fgets(\"%s\",0x%x,0x%x)-> 0x%x\n" RESET,buf,max,fd, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}


void* libc::s_select(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("select()-> 0x%x\n",  value);
#else
    printf(RED "select()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_strlcpy(void*)
{
    uc_err err;
    int value = 0;
    char dst[1024] = {0};
    char src[1024] = {0};
    unsigned int dst_addr = emulator::get_r0();
    unsigned int src_addr = emulator::get_r1();
    unsigned int siz = emulator::get_r2();

    for(int i = 0; i < 1024; i++)
    {
        err = uc_mem_read(g_uc,dst_addr+i,&dst[i],1);
        if(dst[i] == 0)
            break;
    }

    for(int i = 0; i < 1024; i++)
    {
        err = uc_mem_read(g_uc,src_addr+i,&src[i],1);
        if(src[i] == 0)
            break;
    }
    char *d = dst;
    const char *s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }
    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0) {
        if (siz != 0)
            *d = '\0';		/* NUL-terminate dst */
        while (*s++)
            ;
    }

    value = (s - src - 1);	/* count does not include NUL */
    err = uc_mem_write(g_uc,dst_addr,dst,strlen(dst)+1);

#ifdef _MSC_VER
    printf("strlcpy(\"%s\",\"%s\",0x%x)-> 0x%x\n", dst, src, siz, value);
#else
    printf(RED "strlcpy(\"%s\",\"%s\",0x%x)-> 0x%x\n" RESET, dst, src, siz, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_signal(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("signal()-> 0x%x\n",  value);
#else
	printf(RED "signal()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_rename(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("rename()-> 0x%x\n",  value);
#else
	printf(RED "rename()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_remove(void*)
{
	uc_err err;
    char path[256] = {0};
	unsigned int path_addr = emulator::get_r0();
	int value = 0;
    if (path_addr){
        uc_mem_read(g_uc, path_addr, path, 256);
    }

#ifdef _MSC_VER
	printf("remove(\"%s\")-> 0x%x\n",  path, value);
#else
	printf(RED "remove(\"%s\")-> 0x%x\n" RESET, path, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_strncpy(void*)
{
    uc_err err;
    int value = 0;

    char buf[2048] ={0};
    char buf1[2048] ={0};

    unsigned int dst = emulator::get_r0();
    unsigned int src = emulator::get_r1();
    int n = emulator::get_r2();

    if(src)
    {
        for(int i = 0; i < 2048; i++)
        {
            err = uc_mem_read(g_uc,src+i,&buf1[i],1);
            if(buf1[i] == 0)
                break;
        }
    }

    strncpy(buf,buf1,n);
    err = uc_mem_write(g_uc,dst,buf,strlen(buf));
#ifdef _MSC_VER
    printf("strncpy(\"%s\",\"%s\",0x%x)-> 0x%x\n", buf, buf1, n, dst);
#else
    printf(RED "strncpy(\"%s\",\"%s\",0x%x)-> 0x%x\n" RESET, buf, buf1,n, dst);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&dst);
    return 0;
}

void* libc::s_exit(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("exit()-> 0x%x\n",  value);
#else
	printf(RED "exit()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_getuid(void*)
{
	uc_err err;
	int value = 10028;

#ifdef _MSC_VER
	printf("getuid()-> 0x%x\n",  value);
#else
	printf(RED "getuid()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_unlink(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("unlink()-> 0x%x\n",  value);
#else
	printf(RED "unlink()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_madvise(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("madvise()-> 0x%x\n",  value);
#else
	printf(RED "madvise()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_puts(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("puts()-> 0x%x\n",  value);
#else
	printf(RED "puts()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_pread(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("pread()-> 0x%x\n",  value);
#else
	printf(RED "pread()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_memmove(void*)
{
    uc_err err;
    int size = emulator::get_r2();
    unsigned int src = emulator::get_r1();
    unsigned int dst = emulator::get_r0();

    void* ptr = malloc(size);
    if(ptr)
    {
        err = uc_mem_read(g_uc,src,ptr,size);
        err = uc_mem_write(g_uc,dst,ptr,size);
        free(ptr);
    }

#ifdef _MSC_VER
    printf("memove(0x%x,0x%x,0x%x)-> 0x%x\n",dst,src,size,dst);
#else
    printf(RED "memove(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,dst,src,size,dst);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&dst);
	return 0;
}

void* libc::s_setenv(void*)
{
	uc_err err;
	int value = 0;

    char buf[512] ={0};
    char buf1[512] ={0};

    unsigned int name_addr = emulator::get_r0();
    unsigned int value_addr = emulator::get_r1();
    unsigned int overwrite = emulator::get_r2();

    if(name_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,name_addr+i,&buf[i],1);
            if(buf[i] == 0)
                break;
        }
    }

    if(value_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,value_addr+i,&buf1[i],1);
            if(buf1[i] == 0)
                break;
        }
    }

#ifdef _MSC_VER
	printf("setenv(%s,%s,0x%x)-> 0x%x\n",  buf,buf1,overwrite,value);
#else
	printf(RED "setenv(%s,%s,0x%x)-> 0x%x\n" RESET, buf,buf1,overwrite,value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_getopt(void*)
{
	uc_err err;
	int value = 0;
	int argc = emulator::get_r0();
    int argv = emulator::get_r1();
    int opt = emulator::get_r2();

#ifdef _MSC_VER
	printf("getopt(0x%x, 0x%x, 0x%x)-> 0x%x\n", argc, argv, opt, value);
#else
	printf(RED "getopt(0x%x, 0x%x, 0x%x)-> 0x%x\n" RESET, argc, argv, opt, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
	return 0;
}

void* libc::s__cxa_finalize(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("__cxa_finalize()-> 0x%x\n", value);
#else
	printf(RED "__cxa_finalize()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
	return 0;
}

void* libc::s__cxa_exit(void*)
{
	uc_err err;
	int value = 0;

#ifdef _MSC_VER
	printf("__cxa_exit()-> 0x%x\n", value);
#else
	printf(RED "__cxa_exit()-> 0x%x\n" RESET, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
	return 0;
}

void* libc::s_printf(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("printf()-> 0x%x\n", value);
#else
    printf(RED "printf()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_srand(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("srand()-> 0x%x\n", value);
#else
    printf(RED "srand()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_srand48(void*)
{
    uc_err err;
    int value = 0;
    int seed = emulator::get_r0();
#ifndef _WIN32
    srand48(seed);
#endif

#ifdef _MSC_VER
    printf("srand48(0x%x)\n", seed);
#else
    printf(RED "srand48(0x%x)\n" RESET, seed);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_lrand48(void*)
{
    uc_err err;
	int value = 0;
#ifndef _WIN32
    value = lrand48();
#endif
#ifdef _MSC_VER
    printf("lrand48()-> 0x%x\n", value);
#else
    printf(RED "lrand48()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_strstr(void*)
{
    uc_err err;
	char dst_buf[1024] = {0};
	char src_buf[64] = {0};
	int value = 0;
    unsigned int dst = emulator::get_r0();
    unsigned int src = emulator::get_r1();
	err = uc_mem_read(g_uc, dst, dst_buf, 1024);
	err = uc_mem_read(g_uc, src, src_buf, 64);
	char* ptr = strstr(dst_buf, src_buf);
	if (ptr) {
		value = dst + ptr - dst_buf;
	}

#ifdef _MSC_VER
    printf("strstr(\"%s\", \"%s\")-> 0x%x\n", dst_buf, src_buf, value);
#else
    printf(RED "strstr(\"%s\", \"%s\")-> 0x%x\n" RESET, dst_buf, src_buf, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_strtol(void*)
{
    uc_err err;
    char dst_buf[1024] = {0};
    unsigned int dst = emulator::get_r0();
    unsigned int end_ptr = emulator::get_r1();
    unsigned int base = emulator::get_r2();
    err = uc_mem_read(g_uc, dst, dst_buf, 1024);
    char* end_x = NULL;
    int value = strtol(dst_buf, &end_x, base);

#ifdef _MSC_VER
    printf("strtol(\"%s\", %x, 0x%x)-> 0x%x\n", dst_buf, end_ptr, base, value);
#else
    printf(RED "strtol(\"%s\", %x, 0x%x)-> 0x%x\n" RESET, dst_buf, end_ptr, base, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_getenv(void*)
{
    uc_err err;
    char var[256] = {0};
    char buf[256] = {0};
    unsigned int addr = emulator::get_r0();
    err = uc_mem_read(g_uc, addr, var, 256);
    int len = get_var(var, buf);
    unsigned int value = (unsigned int)sys_malloc(len+1);
    err = uc_mem_write(g_uc, value, buf, len);

#ifdef _MSC_VER
    printf("getenv(\"%s\")-> \"%s\"\n", var, buf);
#else
    printf(RED "getenv(\"%s\")-> \"%s\"\n" RESET, var, buf);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_pthread_cond_broadcast(void*)
{
    uc_err err;
    int value = 0;
    unsigned int addr = emulator::get_r0();


#ifdef _MSC_VER
    printf("pthread_cond_broadcast(0x%x)-> 0x%x\n", addr, value);
#else
    printf(RED "pthread_cond_broadcast(0x%x)-> 0x%x\n" RESET, addr, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_pthread_cond_wait(void*)
{
    uc_err err;
    int value = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int mutex = emulator::get_r1();

#ifdef _MSC_VER
    printf("pthread_cond_wait(0x%x,0x%x)-> 0x%x\n", addr,mutex, value);
#else
    printf(RED "pthread_cond_wait(0x%x,0x%x)-> 0x%x\n" RESET, addr,mutex, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_fwrite(void*)
{
    uc_err err;
    char var[1024] = {0};
    unsigned int buf_addr = emulator::get_r0();
    unsigned int count = emulator::get_r1();
    unsigned int size = emulator::get_r2();
    unsigned int fd = emulator::get_r3();
    uc_mem_read(g_uc, buf_addr, var, 1024);
    //int value = fwrite(var, count, size, (FILE*)fd);
#ifdef _MSC_VER
    printf("fwrite(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n", buf_addr,count,size,fd, size);
#else
    printf(RED "fwrite(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET, buf_addr,count,size,fd,size);
#endif
    print_hex_dump_bytes(var,0x10);
    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &size);
    return 0;
}

void* libc::s_strrchr(void*)
{
    uc_err err;
    char buf[1024] = {0};
    unsigned int buf_addr = emulator::get_r0();
    unsigned int var = emulator::get_r1();

    int value = 0;
    err = uc_mem_read(g_uc, buf_addr, buf, 1024);
    char* ptr = strrchr(buf, var);
    if (ptr) {
        value = buf_addr + ptr - buf;
    }

#ifdef _MSC_VER
    printf("strrchr(\"%s\",0x%x)-> 0x%x\n", buf, var, value);
#else
    printf(RED "strrchr(\"%s\",0x%x)-> 0x%x\n" RESET, buf, var, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_memcmp(void*)
{
    uc_err err;
    int value = 0;
    unsigned char buf[512] ={0};
    unsigned char buf1[512] ={0};

    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();
    int size = emulator::get_r2();

    if(addr)
    {
        err = uc_mem_read(g_uc,addr,buf,512);
    }

    if(addr1)
    {
        err = uc_mem_read(g_uc,addr1,buf1,512);
    }

    value = memcmp(buf,buf1,size);

    emulator::update_cpu_model();
    if (emulator::is_data_printable(buf, size) ||
            emulator::is_data_printable(buf1, size)){
#ifdef _MSC_VER
        printf("memcmp(\"%s\",\"%s\",0x%x)-> %d\n",buf,buf1,size,value);
#else
        printf(RED "memcmp(\"%s\",\"%s\",0x%x)-> %d\n" RESET, buf, buf1, size, value);
#endif
    }
    else {
#ifdef _MSC_VER
        printf("memcmp(0x%x,0x%x,0x%x)-> %d\n",addr,addr1,size,value);
#else
        printf(RED "memcmp(0x%x,0x%x,0x%x)-> %d\n" RESET, addr, addr1, size, value);
        print_hex_dump_bytes(buf,0x10);
        print_hex_dump_bytes(buf1,0x10);
#endif
    }

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s__aeabi_atexit(void*)
{
    int value = 0;

    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();
	unsigned int addr2 = emulator::get_r2();

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("__aeabi_atexit(0x%x,0x%x,0x%x)-> 0x%x\n",addr,addr1,addr2,value);
#else
    printf(RED "__aeabi_atexit(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,addr,addr1,addr2,value);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s__aeabi_memclr4(void*)
{
    uc_err err;
	unsigned char value = 0;
    unsigned int addr = emulator::get_r0();
    int size = emulator::get_r1();

    for(int i = 0; i < size; i++)
    {
        err = uc_mem_write(g_uc,addr+i,&value,1);
    }

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("__aeabi_memclr4(0x%x,0x%x)  -> 0x%x\n",addr,size,addr);
#else
    printf(RED "__aeabi_memclr4(0x%x,0x%x)  -> 0x%x \n" RESET,addr,size,addr);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);

    return 0;
}

void* libc::s__android_log_print(void*)
{
    uc_err err;
    unsigned char value = 1;
    char tag_buffer[64] = {0};
	char fmt_buffer[256] = {0};
    unsigned int level = emulator::get_r0();
    unsigned int tag = emulator::get_r1();
	unsigned int format = emulator::get_r2();
	int data = emulator::get_r3();

    for(int i = 0; i < 64; i++)
    {
        err = uc_mem_read(g_uc,tag+i,&tag_buffer[i],1);
        if(tag_buffer[i] == 0)
            break;
    }

	for(int i = 0; i < 256; i++)
	{
		err = uc_mem_read(g_uc,format+i,&fmt_buffer[i],1);
		if(fmt_buffer[i] == 0)
			break;
	}
    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("__android_log_print(0x%x,\"%s\",\"%s\", 0x%x)  -> 0x%x\n",level,tag_buffer,fmt_buffer, data,value);
#else
    printf(RED "__android_log_print(0x%x,\"%s\",\"%s\", 0x%x)  -> 0x%x \n" RESET,level,tag_buffer,fmt_buffer, data,value);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s_strncasecmp(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("strncasecmp()-> 0x%x\n", value);
#else
    printf(RED "strncasecmp()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}


void* libc::s__assert2(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("__assert2()-> 0x%x\n", value);
#else
    printf(RED "__assert2()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_uncompress(void *)
{
    uc_err err;
    int value = Z_OK;
    uLongf size = 0;
    unsigned int dst_addr = emulator::get_r0();
    unsigned int dst_size_addr = emulator::get_r1();
    unsigned int src_addr = emulator::get_r2();
    unsigned int src_size = emulator::get_r3();

    if (dst_addr){
        err = uc_mem_read(g_uc, dst_size_addr, &size, 4);
        void* buf = malloc(size);
        void* srcbuf = malloc(src_size);
        if (buf && srcbuf){
            err = uc_mem_read(g_uc, src_addr,srcbuf, src_size);
            value = uncompress((Byte*)buf,&size,(Byte*)srcbuf,src_size);
            err = uc_mem_write(g_uc, dst_addr,buf, size);
            free(buf);
            free(srcbuf);
        }
    }


#ifdef _MSC_VER
    printf("uncompress(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n", dst_addr,dst_size_addr,src_addr,src_size,value);
#else
    printf(RED "uncompress(0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET, dst_addr,dst_size_addr,src_addr,src_size,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc, UC_ARM_REG_R0, &value);
    return 0;
}

void* libc::s_mkdir(void*)
{
    uc_err err;
    int value = 0;
    char buf[512] = {0};
    unsigned int dirp = emulator::get_r0();
    int mode = emulator::get_r1();

    for(int i = 0; i < 512; i++)
    {
        err = uc_mem_read(g_uc,dirp+i,&buf[i],1);
        if(buf[i] == 0)
            break;
    }

    //value = mkdir((DIRX*)dirp, mode);

#ifdef _MSC_VER
    printf("mkdir(\"%s\",0x%o)-> 0x%x\n", buf,mode, value);
#else
    printf(RED "mkdir(\"%s\",0x%o)-> 0x%x\n" RESET, buf, mode, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_adler32(void*)
{
    uc_err err;
    int value = 0;

    unsigned int old = emulator::get_r0();
    unsigned int buf_addr = emulator::get_r1();
    unsigned int len = emulator::get_r2();
    unsigned int lr = emulator::get_lr();

#ifdef _MSC_VER
    printf("adler32(0x%x,0x%x,0x%x) -> 0x%x\n", old,buf_addr,len, value);
#else
    printf(RED "adler32(0x%x,0x%x,0x%x) -> 0x%x\n" RESET, old,buf_addr,len, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_chmod(void*)
{
	uc_err err;
	int value = 0;
    char path[256] = {0};

	unsigned int buf_addr = emulator::get_r0();
	unsigned int mode = emulator::get_r1();
    for(int i = 0; i < 256; i++)
    {
        err = uc_mem_read(g_uc,buf_addr+i,&path[i],1);
        if(path[i] == 0)
            break;
    }
#ifdef _MSC_VER
	printf("chmod(\"%s\",%o)-> 0x%x\n", path,mode, value);
#else
	printf(RED "chmod(\"%s\",%o)-> 0x%x\n" RESET, path,mode, value);
#endif

	emulator::update_cpu_model();

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
	return 0;
}

void* libc::s_fcntl(void*)
{
    uc_err err;
    int value = 0;
    struct flock fl;

    unsigned int fd = emulator::get_r0();
    unsigned int cmd = emulator::get_r1();
    unsigned int arg = emulator::get_r2();
    if (cmd == 3)
    {
        value = fcntl(fd, cmd);
    }
    else if(cmd == 6 || cmd == 7){

    }

#ifdef _MSC_VER
    printf("fcntl(0x%x,0x%x,0x%x)-> 0x%x\n", fd,cmd, arg,value);
#else
    printf(RED "fcntl(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, fd,cmd, arg, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_fstat(void*)
{
    uc_err err;
    int value = 0;
    struct stat st;
	android_stat ast;

    unsigned int fd = emulator::get_r0();
    unsigned int st_addr = emulator::get_r1();
    value = fstat(fd, &st);
    ast.st_dev = st.st_dev;
    ast.st_mode = st.st_mode;
    ast.st_blksize = st.st_blksize;
    ast.st_blocks = st.st_blocks;
    ast.st_uid = st.st_uid;
    ast.st_size = 0x0000002000000020;
    ast.st_ino = st.st_ino;

	err = uc_mem_write(g_uc, st_addr, &ast, sizeof(struct android_stat));
#ifdef _MSC_VER
    printf("fstat(0x%x,0x%x)-> 0x%x\n", fd,st_addr,value);
#else
    printf(RED "fstat(0x%x,0x%x)-> 0x%x\n" RESET, fd,st_addr, value);
#endif
    print_hex_dump_bytes(&ast, sizeof(struct android_stat));
    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_ftruncate(void*)
{
    uc_err err;
    int value = 0;

    unsigned int fd = emulator::get_r0();
    unsigned int length = emulator::get_r1();
    //value = ftruncate(fd, length);

#ifdef _MSC_VER
    printf("ftruncate(0x%x,0x%x)-> 0x%x\n", fd,length,value);
#else
    printf(RED "ftruncate(0x%x,0x%x)-> 0x%x\n" RESET, fd,length, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_fsync(void*)
{
    uc_err err;
    int value = 0;

    unsigned int fd = emulator::get_r0();
    value = fsync(fd);

#ifdef _MSC_VER
    printf("fsync(0x%x)-> 0x%x\n", fd,value);
#else
    printf(RED "fsync(0x%x)-> 0x%x\n" RESET, fd, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_flock(void*)
{
    uc_err err;
    int value = 0;

    unsigned int fd = emulator::get_r0();
    unsigned int op = emulator::get_r1();

#ifdef _MSC_VER
    printf("flock(0x%x,0x%x)-> 0x%x\n", fd,op,value);
#else
    printf(RED "flock(0x%x,0x%x)-> 0x%x\n" RESET, fd, op,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_join(void*){
    uc_err err;
    int value = 0;

    unsigned int func = 0;
    unsigned int arg = 0;
    unsigned int tsp = 0;
    unsigned int tid = emulator::get_r0();
    unsigned int data = emulator::get_r1();

    uc_mem_read(g_uc, tid, &func, 4);
    uc_mem_read(g_uc, tid+4, &arg, 4);
    uc_mem_read(g_uc, tid+8, &tsp, 4);

#ifdef _MSC_VER
    printf("pthread_join(0x%x,0x%x)-> 0x%x\n", tid,data,value);
#else
    printf(RED "pthread_join(0x%x,0x%x)-> 0x%x\n" RESET, tid, data,value);
#endif
    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    emulator::get_emulator()->save_register();
    printf("thread=0x%x func=0x%x arg=0x%x tsp=0x%x\n", tid, func, arg, tsp);
    err = uc_reg_write(g_uc,UC_ARM_REG_PC,&func);
    err = uc_reg_write(g_uc,UC_ARM_REG_SP,&tsp);

    emulator::get_emulator()->set_thread_info(1, tsp, 0);
    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&arg);
    //emulator::update_cpu_model();
    return 0;
}

void* libc::s_pow(void*){
    uc_err err;
    int value = 0;

    unsigned int x = emulator::get_r0();
    unsigned int y = emulator::get_r1();
    value = pow(x,y);

#ifdef _MSC_VER
    printf("pow(0x%x,0x%x)-> 0x%x\n", x,y,value);
#else
    printf(RED "pow(0x%x,0x%x)-> 0x%x\n" RESET, x, y, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_memchr(void*){
    uc_err err;
    unsigned int value = 0;
    unsigned char buf[2048] = {0};
    unsigned int addr = emulator::get_r0();
    unsigned int ch = emulator::get_r1();
    int size = emulator::get_r2();
    int new_size = size;
    if (new_size > 2048)
    {
        new_size = 2048;
    }
    uc_mem_read(g_uc, addr, buf, new_size);
    value = (unsigned int)memchr(buf, ch, new_size);
	if (value){
        value -= (unsigned int)buf;
        value = addr + value;
	}

#ifdef _MSC_VER
    printf("memchr(0x%x,0x%x,0x%x)-> 0x%x\n", buf,ch,size,value);
#else
    printf(RED "memchr(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, buf, ch,size, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_mutexattr_init(void*){
    uc_err err;
    unsigned int value = 0;
    unsigned int addr = emulator::get_r0();

#ifdef _MSC_VER
    printf("pthread_mutexattr_init(0x%x)-> 0x%x\n", addr,value);
#else
    printf(RED "pthread_mutexattr_init(0x%x)-> 0x%x\n" RESET, addr, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_mutexattr_settype(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int type = emulator::get_r1();

#ifdef _MSC_VER
    printf("pthread_mutexattr_settype(0x%x,0x%x)-> 0x%x\n", addr,type,value);
#else
    printf(RED "pthread_mutexattr_settype(0x%x,0x%x)-> 0x%x\n" RESET,  addr,type, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pthread_mutexattr_destroy(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int addr = emulator::get_r0();

#ifdef _MSC_VER
    printf("pthread_mutexattr_destroy(0x%x)-> 0x%x\n", addr,value);
#else
    printf(RED "pthread_mutexattr_destroy(0x%x)-> 0x%x\n" RESET, addr, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_tolower(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int ch = emulator::get_r0();
    value = tolower(ch);

#ifdef _MSC_VER
    printf("tolower(0x%x)-> 0x%x\n", ch,value);
#else
    printf(RED "tolower(0x%x)-> 0x%x\n" RESET, ch, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_waitpid(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int pid = emulator::get_r0();
    unsigned int st = emulator::get_r1();
    unsigned int opt = emulator::get_r2();

#ifdef _MSC_VER
    printf("waitpid(0x%x,0x%x,0x%x)-> 0x%x\n", pid,st,opt,value);
#else
    printf(RED "waitpid(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, pid,st,opt,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_popen(void*)
{
    uc_err err;
    unsigned int value = 0;
    char cmd[256] = {0};
    char mode[8] = {0};
    unsigned int cmd_addr = emulator::get_r0();
    unsigned int mode_addr = emulator::get_r1();
    uc_mem_read(g_uc, cmd_addr, cmd, 256);
    uc_mem_read(g_uc, mode_addr, mode, 8);

#ifdef _MSC_VER
    printf("popen(\"%s\",\"%s\")-> 0x%x\n", cmd, mode,value);
#else
    printf(RED "popen(\"%s\",\"%s\")-> 0x%x\n" RESET, cmd, mode, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_pclose(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int fp = emulator::get_r0();
    //fclose(fp);

#ifdef _MSC_VER
    printf("pclose(0x%x)-> 0x%x\n", pid,value);
#else
    printf(RED "pclose(0x%x)-> 0x%x\n" RESET, fp,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_gethostbyname(void*)
{
    uc_err err;
    unsigned int value = 1;
    char buf[256] = {0};
    unsigned int name_addr = emulator::get_r0();
    uc_mem_read(g_uc, name_addr, buf, 256);
    struct hostent* ht, *aht;
	unsigned int addr_list = (unsigned int)sys_malloc(4);
    aht = (hostent*)sys_malloc(sizeof(hostent));
    ht = gethostbyname(buf);

    uc_mem_write(g_uc, (unsigned int)aht, &ht, sizeof(hostent));
    unsigned int ip = *(unsigned int*)ht->h_addr_list[0];
    unsigned int addr = (unsigned int)aht + offsetof(hostent, h_addrtype);
    uc_mem_write(g_uc, addr, &ht->h_addrtype, 4);
    ht->h_length = 4;
    uc_mem_write(g_uc, addr+4, &ht->h_length, 4);

    unsigned int addr_list_ptr = (unsigned int)sys_malloc(4);
    addr = (unsigned int)aht + offsetof(hostent, h_addr_list);
    uc_mem_write(g_uc, addr_list, &ip, 4);
    uc_mem_write(g_uc, addr_list_ptr, &addr_list, 4);
    uc_mem_write(g_uc, addr, &addr_list_ptr, 4);

	value = (unsigned int)aht;
#ifdef _MSC_VER
    printf("gethostbyname(\"%s\")-> 0x%x\n", buf,value);
#else
    printf(RED "gethostbyname(\"%s\")-> 0x%x\n" RESET, buf,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_sigaction(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int sig = emulator::get_r0();
    unsigned int act = emulator::get_r1();
    unsigned int old = emulator::get_r2();

#ifdef _MSC_VER
    printf("sigaction(0x%x,0x%x,0x%x)-> 0x%x\n", sig,act,old,value);
#else
    printf(RED "sigaction(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, sig,act,old,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_setsockopt(void*)
{
    uc_err err;
    unsigned int value = 0;
    unsigned int sock = emulator::get_r0();
    unsigned int level = emulator::get_r1();
    unsigned int name = emulator::get_r2();
    unsigned int opt_value = emulator::get_r3();
    unsigned int len = 0;
    uc_mem_read(g_uc, emulator::get_sp(), &len, 4);

#ifdef _MSC_VER
    printf("setsockopt(0x%x,0x%x,0x%x,0x%x,0x%x)-> 0x%x\n", sock,level,name,opt_value,len,value);
#else
    printf(RED "setsockopt(0x%x,0x%x,0x%x,0x%x,0x%x)-> 0x%x\n" RESET, sock,level,name,opt_value,len,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_socket(void*)
{
    uc_err err;
    int value = 0;
    unsigned int domain = emulator::get_r0();
    unsigned int type = emulator::get_r1();
    unsigned int proto = emulator::get_r2();
    value = socket(domain,type,proto);

#ifdef _MSC_VER
    printf("socket(0x%x,0x%x,0x%x)-> 0x%x\n", domain,type,proto,value);
#else
    printf(RED "socket(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, domain,type,proto,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_connect(void*)
{
    uc_err err;
    int value = 0;
    unsigned int sk = emulator::get_r0();
    unsigned int addr = emulator::get_r1();
    unsigned int len = emulator::get_r2();
    //value = connect(sk,addr,len);

#ifdef _MSC_VER
    printf("connect(0x%x,0x%x,0x%x)-> 0x%x\n", sk,addr,len,value);
#else
    printf(RED "connect(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, sk,addr,len,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_sendto(void*)
{
    uc_err err;
    int value = 0;
    unsigned int sk = emulator::get_r0();
    unsigned int addr = emulator::get_r1();
    unsigned int len = emulator::get_r2();
    value = len;

#ifdef _MSC_VER
    printf("sendto(0x%x,0x%x,0x%x)-> 0x%x\n", sk,addr,len,value);
#else
    printf(RED "sendto(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, sk,addr,len,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_recvfrom(void*)
{
    uc_err err;
    int value = 0;
    unsigned int sk = emulator::get_r0();
    unsigned int addr = emulator::get_r1();
    unsigned int len = emulator::get_r2();
    value = len;

#ifdef _MSC_VER
    printf("recvfrom(0x%x,0x%x,0x%x)-> 0x%x\n", sk,addr,len,value);
#else
    printf(RED "recvfrom(0x%x,0x%x,0x%x)-> 0x%x\n" RESET, sk,addr,len,value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}


symbols g_syms[] = 
{
	{0x46c5242d,"__cxa_finalize",(void*)libc::s__cxa_finalize,0,1},
	{0x4b3bddf7,"__cxa_exit",(void*)libc::s__cxa_exit,0,1},
	{0xa719deaf,"malloc",(void*)libc::s_malloc,0,1},
    {0x1c13e31d,"realloc",(void*)libc::s_realloc,0,1},
    {0x9d13bfdf,"calloc",(void*)libc::s_calloc,0,1,},
	{0x4d2ec1c8,"free",(void*)libc::s_free,0,1},
	{0x8463960a,"memset",(void*)libc::s_memset,0,0},
	{0x7f822dfe,"__aeabi_memset",(void*)libc::s__aeabi_memset,0,1},
	{0x84e4836b,"mmap",(void*)libc::sys_mmap,0,1,0xc0},
	{0x5e85da63,"cacheflush",(void*)libc::sys_cacheflush,0,1,0xf0002},
	{0x2aa01427,"__aeabi_memcpy",(void*)libc::s__aeabi_memcpy,0,1},
	{0xfb512a1b,"dlopen",(void*)libc::sys_dlopen,0,1},
	{0x48800e70,"dlclose",(void*)libc::sys_dlclose,0,1},
	{0x40296778,"dlsym",(void*)libc::sys_dlsym,0,1},
    {0x0dbb3b8a,"dladdr",(void*)libc::sys_dladdr,0,1,},
	{0xed89f56b,"__system_property_get",(void*)libc::s__system_property_get,0,1},
	{0x36437e34,"gettimeofday",(void*)libc::s_gettimeofday,0,1},
	{0xbc836fa7,"strdup",(void*)libc::s_strdup,0,1},
	{0x025d112d,"strlen",(void*)libc::s_strlen,0,1},
	{0x4273782f,"strncmp",(void*)libc::s_strncmp,0,1},
	{0xa47083a4,"open",(void*)libc::s_open,0,1,0x5},
	{0x98574167,"read",(void*)libc::s_read,0,1,3},
    {0x7d6b7a5f,"write",(void*)libc::s_write,0,1},
    {0x130181c4,"close",(void*)libc::s_close,0,1,6},
	{0x2cd5453f,"mprotect",(void*)libc::sys_mprotect,0,1,0x7d},
	{0xbd2f3f6d,"sscanf",(void*)libc::s_sscanf,0,1,},
	{0xa8ae7412,"strchr",(void*)libc::s_strchr,0,1,},
	{0xe2d7f2a6,"strtoul",(void*)libc::s_strtoul,0,1,},
	{0xd141afd3,"memcpy",(void*)libc::s__aeabi_memcpy,0,0,},
    {0xbb444062,"bsd_signal",(void*)libc::s_bsd_signal,0,0,},
    {0xb8669b99,"raise",(void*)libc::s_raise,0,1,},
    {0xbbad4c48,"getpid",(void*)libc::s_getpid,0,1,},
    {0x3bd7e17b,"strcmp",(void*)libc::s_strcmp,0,1},
    {0xbd6735c3,"strcpy",(void*)libc::s_strcpy,0,1},
    {0x54cbeaf6,"__errno",(void*)libc::s__errno,0,1},
    {0x82cf5a84,"strerror",(void*)libc::s_strerror,0,1},
    {0x6692b54,"access",(void*)libc::s_access,0,1},
    {0xfab5b424,"abort",(void*)libc::s_abort,0,1},
    {0x900f6a6e,"strcat",(void*)libc::s_strcat,0,1},
	{0x93f54eea,"pthread_mutex_init",(void*)libc::s_pthread_mutex_init,0,1},
    {0xd20e3190,"pthread_mutex_lock",(void*)libc::s_pthread_mutex_lock,0,1},
    {0xb325080c,"pthread_mutex_unlock",(void*)libc::s_pthread_mutex_unlock,0,1},
    {0x6f9c4cda,"lseek",(void*)libc::s_lseek,0,1,19},
    //{0xffa1e6f0,"snprintf",(void*)libc::s_snprintf,1},
    {0x1f9a630e,"pipe",(void*)libc::s_pipe,0,1},
    {0xbbeb587a,"fork",(void*)libc::s_fork,0,1},
	{0xe704856a,"sysconf",(void*)libc::s_sysconf,0,1},
    {0x55642948,"fopen",(void*)libc::s_fopen,0,1},
    {0x6943eb8b,"fread",(void*)libc::s_fread,0,1},
    {0x252c547b,"fseek",(void*)libc::s_fseek,0,1},
    {0xba4c3b3d,"fclose",(void*)libc::s_fclose,0,1},
	{0x6f949845,"time",(void*)libc::s_time,0,1},
	{0xd4f46c84,"sbrk",(void*)libc::s_sbrk,0,1},
	{0xfb59145a,"__stack_chk_fail",(void*)libc::s__stack_chk_fail,0,1},
	{0x2bd12c2d,"__stack_chk_guard",(void*)libc::s__stack_chk_guard,1,0},//var
    //{0x4b19744b,"_Znaj",(void*)libc::s_malloc,1},
    //{0xc03584f5,"_ZdaPv",(void*)libc::s_free,1},
	{0x247135bc,"opendir",(void*)libc::s_opendir,0,1},
    {0xeb5e1d3e,"readdir",(void*)libc::s_readdir,0,1},
	{0x7992dd03,"closedir",(void*)libc::s_closedir,0,1},
    {0x0e4d97e1,"mkdir",(void*)libc::s_mkdir,0,1},
	{0xe032df8e,"readdir_r",(void*)libc::s_adler32,0,1},
    {0x4638c890,"getppid",(void*)libc::s_getppid,0,1},
    {0x07295669,"kill",(void*)libc::s_kill,0,1},
    {0x272d2162,"atoi",(void*)libc::s_atoi,0,1},
    {0xec4281bf,"itoa",(void*)libc::s_itoa,0,1},
    {0x20b8ff21,"stat",(void*)libc::s_stat,0,1},
    {0x838f2101,"munmap",(void*)libc::s_munmap,0,1,0x5b},
    {0xa7701c0,"pthread_create",(void*)libc::s_pthread_create,0,1},
	{0x212b3e97,"pthread_key_create",(void*)libc::s_pthread_key_create,0,1},
	{0x94efa2eb,"pthread_key_delete",(void*)libc::s_pthread_key_delete,0,1},
	{0x8c79a112,"pthread_getspecific",(void*)libc::s_pthread_getspecific,0,1},
	{0x9256165b,"pthread_setspecific",(void*)libc::s_pthread_setspecific,0,1},
    {0x17b31dd8,"inotify_init",(void*)libc::s_inotify_init,0,1},
    {0xafc21fc6,"inotify_add_watch",(void*)libc::s_inotify_add_watch,0,1},
	{0xc2002e91,"inotify_rm_watch",(void*)libc::s_inotify_rm_watch,0,1},
    {0xbae22ff5,"fgets",(void*)libc::s_fgets,0,1},
    {0x4bf2eac0,"select",(void*)libc::s_select,0,1},
    {0x6eca641c,"strlcpy",(void*)libc::s_strlcpy,0,1},
	{0x740c95f5,"signal",(void*)libc::s_signal,0,1,},
	{0xd99d544e,"rename",(void*)libc::s_rename,0,1},
	{0x68801d30,"remove",(void*)libc::s_remove,0,1,},
    {0xc4c3ac97,"strncpy",(void*)libc::s_strncpy,0,1,},
	{0x9409840e,"exit",(void*)libc::s_exit,0,1,},
	{0xbd668ea3,"getuid",(void*)libc::s_getuid,0,1,},
	{0xdbc7b210,"unlink",(void*)libc::s_unlink,0,1,},
	{0xe88f4f24,"madvise",(void*)libc::s_madvise,0,1,},
	{0x9a1e494f,"puts",(void*)libc::s_puts,0,1,},
	{0x86e389a9,"pread",(void*)libc::s_pread,0,1,},
	{0x80ec372a,"memmove",(void*)libc::s_memmove,0,1,},
    {0x3ac7e094,"__aeabi_memmove",(void*)libc::s_memmove,0,1,},
	{0x85ff8ad1,"setenv",(void*)libc::s_setenv,0,1,},
	{0x2a601179,"getopt",(void*)libc::s_getopt,0,1,},
    {0xd21739f1,"printf",(void*)libc::s_printf,0,1,0xff},
	{0x41d2476a,"srand",(void*)libc::s_srand,0,1,},
    {0xa55b4f5a,"srand48",(void*)libc::s_srand48,0,1,},
    {0x34cda37d,"lrand48",(void*)libc::s_lrand48,0,1,},
    {0x52ff8a3f,"strstr",(void*)libc::s_strstr,0,1,},
	{0x04896a43,"strtol",(void*)libc::s_strtol,0,1,},
    {0x1db8ca5c,"getenv",(void*)libc::s_getenv,0,1,},
    {0xca3b2c4d,"pthread_cond_broadcast",(void*)libc::s_pthread_cond_broadcast,0,1,},
    {0xeaa33ee8,"pthread_cond_wait",(void*)libc::s_pthread_cond_wait,0,1,},
	{0xcbe2d39a,"pthread_self",(void*)libc::s_adler32,1},
	{0x5625b23b,"pthread_detach",(void*)libc::s_adler32,1},
	{0xc3e99929,"pthread_once",(void*)libc::s_adler32,0,1},
    {0xd426c0a6,"fwrite",(void*)libc::s_fwrite,0,1,},
    {0xcbc50561,"strrchr",(void*)libc::s_strrchr,0,1,},
    {0x57f17b6b,"memcmp",(void*)libc::s_memcmp,0,1,},
    {0x3094dbb5,"__aeabi_atexit",(void*)libc::s__aeabi_atexit,0,1,},
    {0x5747d5ed,"atol",(void*)libc::s_atoi,0,1,},
    {0x5b0590f2,"__aeabi_memclr",(void*)libc::s__aeabi_memclr4,0,1,},
    {0xa05e8d98,"__aeabi_memclr4",(void*)libc::s__aeabi_memclr4,0,1,},
	{0xa9e8c1b3,"__aeabi_memclr8",(void*)libc::s__aeabi_memclr4,0,1,},
    {0x6c845282,"__android_log_print",(void*)libc::s__android_log_print,0,1,},
    {0x01e88f3f,"__assert2",(void*)libc::s__assert2,0,1,},
    {0x46e76e7e,"___cxa_atexit",(void*)libc::s__cxa_exit,0,1},
    {0xe96aff20,"strncasecmp",(void*)libc::s_strncasecmp,0,1},
    {0x4b5d70ec,"uncompress",(void*)libc::s_uncompress,0,1},
	{0x183bc090,"adler32",(void*)libc::s_adler32,0,1},
	{0xa95113d1,"strtoull",(void*)libc::s_adler32,0,1},
	{0x98c8322e,"pthread_join",(void*)libc::s_pthread_join,0,1},
	{0xdd3f5f96,"fsync",(void*)libc::s_fsync,0,1},
	{0x769b31e2,"flock",(void*)libc::s_flock,0,1},
	{0xad65d22c,"waitpid",(void*)libc::s_waitpid,0,1},
	{0xb4dd5ab6,"fcntl",(void*)libc::s_fcntl,0,1},
	{0xd1ac55cd,"fstat",(void*)libc::s_fstat,0,1},
	{0x05e568bb,"socket",(void*)libc::s_socket,0,1,0x119},
	{0xccc0c5b8,"setpgid",(void*)libc::s_adler32,0,1},
	{0xbc14749f,"execve",(void*)libc::s_adler32,0,1},
	{0x228dadab,"creat",(void*)libc::s_adler32,0,1},
	{0x74cff91f,"connect",(void*)libc::s_connect,0,1,0x11b},
	{0xa7733acd,"send",(void*)libc::s_adler32,0,1},
	{0x59d852ad,"recv",(void*)libc::s_adler32,0,1},
    {0x6fdf0506,"sendto",(void*)libc::s_sendto,0,1,0x122},
    {0x32753c31,"recvfrom",(void*)libc::s_recvfrom,0,1,0x124},
    {0x46ccf353,"bind",(void*)libc::s_adler32,0,1,0x11a},
    {0xed514704,"setsockopt",(void*)libc::s_setsockopt,0,1},
	{0x377545a2,"gethostbyname",(void*)libc::s_gethostbyname,0,1},
	{0x887739b8,"uname",(void*)libc::s_adler32,0,1},
	{0x0e97af36,"chmod",(void*)libc::s_chmod,0,1},
	{0xd7bcfd99,"fmod",(void*)libc::s_adler32,0,1},
	{0xbe45d62e,"floor",(void*)libc::s_adler32,0,1},
	{0xffd7bcfd,"fmodf",(void*)libc::s_adler32,0,1},
	{0xb6f073a7,"ceil",(void*)libc::s_adler32,0,1},
	{0x87b422b5,"pow",(void*)libc::s_pow,0,1},
	{0xf17d3769,"ftruncate",(void*)libc::s_ftruncate,0,1},
	{0xedcc388d,"lseek64",(void*)libc::s_adler32,0,1},
	{0xf75d4228,"ftell",(void*)libc::s_adler32,0,1},
	{0x6b5b291a,"_ctype_",(void*)libc::s_adler32,1,1},
	{0xad0d2424,"_toupper_tab_",(void*)libc::s_adler32,1,1},
	{0x0f0a0abf,"_tolower_tab_",(void*)libc::s_adler32,1,1},
	{0xc488ee02,"memchr",(void*)libc::s_memchr,0,1},
	{0xbc7554df,"strcasecmp",(void*)libc::s_adler32,0,1},
	{0x9aedffe0,"strtok",(void*)libc::s_adler32,0,1},
	{0xafabd35e,"crc32",(void*)libc::s_adler32,0,1},
	{0xe2b06b05,"deflate",(void*)libc::s_adler32,0,1},
	{0xe9a0fa06,"inflate",(void*)libc::s_adler32,0,1},
	{0x3268dd21,"deflateInit_",(void*)libc::s_adler32,0,1},
	{0xe91ec2be,"deflateBound",(void*)libc::s_adler32,0,1},
	{0x7312b6b8,"deflateEnd",(void*)libc::s_adler32,0,1},
	{0xfb8dfa93,"fnmatch",(void*)libc::s_adler32,0,1},
	{0x85d3513b,"nanosleep",(void*)libc::s_adler32,0,1},
	{0xea2b6159,"readlink",(void*)libc::s_adler32,0,1},
    {0x189c2293,"pthread_mutexattr_init",(void*)libc::s_pthread_mutexattr_init,0,1},
    {0x26c19962,"pthread_mutexattr_settype",(void*)libc::s_pthread_mutexattr_settype,0,1},
    {0xe3965a02,"pthread_mutexattr_destroy",(void*)libc::s_pthread_mutexattr_destroy,0,1},
    {0xb4511af7,"tolower",(void*)libc::s_tolower,0,1},
    {0xbac44b6a,"popen",(void*)libc::s_popen,0,1},
    {0x6fc3dabb,"pclose",(void*)libc::s_pclose,0,1},
    {0x589c0f74,"sigaction",(void*)libc::s_sigaction,0,1},
    {0x4d998ede,"ptrace",(void*)libc::s_adler32,0,1},
};


int g_sym_cnt = sizeof(g_syms)/sizeof(g_syms[0]);