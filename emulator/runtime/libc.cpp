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

#ifdef _MSC_VER
#include "io.h"
#else
#include <sys/mman.h>
#include <fcntl.h>
#endif


extern uc_engine* g_uc;


soinfo* libc::si =0;


void libc::init(emulator* emu)
{
    si = soinfo_alloc("libc.so");
    si->emu = 1;
    si->emulator = emu;
    si->load_bias = FUNCTION_VIRTUAL_ADDRESS;
    si->flags |= FLAG_LINKED;
}

void* libc::s_malloc(void*)
{
    int size = emulator::get_r0();
	//void* addr = s_mmap(0,v_r0,PROT_NONE,MAP_PRIVATE,-1,0);
	void* addr = sys_malloc(size);

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("malloc(0x%x) ->0x%x\n",size,addr);
#else
    printf(RED "malloc(0x%x) ->0x%p\n" RESET,size,addr);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	return 0;
}

void* libc::s_free(void*)
{
    unsigned int ptr = emulator::get_r0();

	emulator::update_cpu_model();
	
	if(ptr)
		sys_free((void*)ptr);
		//uc_mem_unmap(g_uc,v_r0,0x1000);
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
	unsigned int addr = emulator::get_r0();
	int value =emulator::get_r2();
    int size = emulator::get_r1();
	for(int i = 0; i < size; i++)
	{
		uc_mem_write(g_uc,addr+i,&value,1);
	}

    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("s__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x\n",addr,size,value,addr);
#else
    printf(RED "s__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x \n" RESET,addr,size,value,addr);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);

	return 0;
}

void* libc::s__aeabi_memcpy(void*)
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

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("s__aeabi_memcpy(0x%x,0x%x,0x%x)\n",dst,src,size);
#else
	printf(RED "s__aeabi_memcpy(0x%x,0x%x,0x%x)\n" RESET,dst,src,size);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&size);

	return 0;
}

void* libc::sys_mmap(int type)
{
    uc_err err;
    unsigned int addr = emulator::get_r0();
    int size = emulator::get_r1();
    int prot = emulator::get_r2();

	if(size == 0)
	{
		addr = (unsigned int)s_mmap(0,size,PROT_NONE,MAP_PRIVATE,-1,0);
	}

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("mmap(0x%x,0x%x,0x%x)-> 0x%x\n",addr,size,prot,addr);
#else
	printf(RED "mmap(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,addr,size,prot,addr);
#endif


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

	return 0;
}

void* libc::sys_dlopen(void*)
{
	int value = 0;
	soinfo* si = 0;
	char buf[256] ={0};
    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		uc_mem_read(g_uc,addr,buf,256);
		si = (soinfo*)s_dlopen(buf,0);
	}
	
#ifdef _MSC_VER
	printf("dlopen(%s,0x%x)-> 0x%x\n",buf,emulator::get_r1(),value);
#else
	printf(RED "dlopen(%s,0x%x)-> 0x%x\n" RESET,buf,emulator::get_r1(),value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    emulator::update_cpu_model();

	return 0;
}

void* libc::s__system_property_get(void*)
{
	int ret = 0;
	char name[32] ={0};
	char value[92] ={0};
    unsigned int name_addr = emulator::get_r0();
    unsigned int value_addr = emulator::get_r1();

	if(name_addr && value_addr)
	{
		uc_mem_read(g_uc,name_addr,name,32);
		ret = get_prop(name,value);
		if(ret)
		{
			uc_mem_write(g_uc,value_addr,value,32);
		}
	}
#ifdef _MSC_VER
	printf("__system_property_get(%s,%s)\n",name,value);
#else
	printf(RED "__system_property_get(%s,%s)\n" RESET,name,value);
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

	if(addr)
	{
		tv.tv_sec = time(0);
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
	char buf[256] ={0};
    unsigned int addr = emulator::get_r0();

	if(addr)
	{
		for(i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("strlen(\"%s\")-> 0x%x\n",buf,i);
#else
	printf(RED "strlen(\"%s\")-> 0x%x\n" RESET,buf,i);
#endif

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
	printf("strncmp(%s,%s,0x%x)-> 0x%x\n",buf,buf1,size,value);
#else
	printf(RED "strlen(%s,%s,0x%x)-> 0x%x\n" RESET,buf,buf1,size,value);
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
    printf("strcmp(%s,%s)-> 0x%x\n",buf,buf1,value);
#else
    printf(RED "strcmp(%s,%s)-> 0x%x\n" RESET,buf,buf1,value);
#endif

    uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

    return 0;
}

void* libc::s_open(void*)
{
	uc_err err;
	char buf[256] ={0};
	int value = 1;

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

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("open(\"%s\")-> 0x%x\n",buf,value);
#else
	printf(RED "open(\"%s\")-> 0x%x\n" RESET,buf,value);
#endif

    value = open("maps.txt",O_RDONLY);

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

void* libc::sys_mprotect(void*)
{
	uc_err err;
	int value = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int addr1 = emulator::get_r1();
    unsigned int size = emulator::get_r2();
	
#ifdef _MSC_VER
	printf("mprotect(%x,%x,%x)-> 0x%x\n",addr,addr1,size,value);
#else
	printf(RED "mprotect(%x,%x,%x)-> 0x%x\n" RESET,addr,addr1,size,value);
#endif

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
	char arg0[256]={0};
	char arg1[256]={0};
	char arg2[256]={0};
	char arg3[256]={0};
	char arg4[256]={0};
	char arg5[256]={0};
	char arg6[256]={0};
	char arg7[256]={0};

	int value = 0;
    unsigned int addr = emulator::get_r0();
    unsigned int addr_format = emulator::get_r1();

	if(addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,addr+i,&buf[i],1);
			if(buf[i] == 0)
				break;
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
		if(err != UC_ERR_OK)
			break;
	}

	value = sscanf(buf,format,arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

	err = uc_mem_write(g_uc,emulator::get_r2(),arg0,strlen(arg0));

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
    unsigned int addr = emulator::get_r0();

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

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_getpid(void*)
{
    uc_err err;
    int value = 1234;

#ifdef _MSC_VER
    printf("getpid()-> 0x%x\n",  value);
#else
    printf(RED "getpid()-> 0x%x\n" RESET, value);
#endif

    emulator::update_cpu_model();

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

symbols g_syms[] = 
{
	{0x46c5242d,"__cxa_finalize",0},
	{0x4b3bddf7,"__cxa_exit",0},
	{0xa719deaf,"malloc",(void*)libc::s_malloc,1},
	{0x4d2ec1c8,"free",(void*)libc::s_free,1},
	{0x8463960a,"memset",(void*)libc::s_memset,0},
	{0x7f822dfe,"__aeabi_memset",(void*)libc::s__aeabi_memset,1},
	{0x84e4836b,"mmap",(void*)libc::sys_mmap,1,0xc0},
	{0x5e85da63,"cacheflush",(void*)libc::sys_cacheflush,1,0xf0002},
	{0x2aa01427,"__aeabi_memcpy",(void*)libc::s__aeabi_memcpy,1},
	{0xfb512a1b,"dlopen",(void*)libc::sys_dlopen,1},
	{0xed89f56b,"__system_property_get",(void*)libc::s__system_property_get,1},
	{0x36437e34,"gettimeofday",(void*)libc::s_gettimeofday,1},
	{0xbc836fa7,"strdup",(void*)libc::s_strdup,1},
	{0x025d112d,"strlen",(void*)libc::s_strlen,1},
	{0x4273782f,"strncmp",(void*)libc::s_strncmp,1},
	{0xa47083a4,"open",(void*)libc::s_open,1,0x5},
	{0x98574167,"read",(void*)libc::s_read,1},
	{0x2cd5453f,"mprotect",(void*)libc::sys_mprotect,1,0x7d},
	{0xbd2f3f6d,"sscanf",(void*)libc::s_sscanf,1,},
	{0xa8ae7412,"strchr",(void*)libc::s_strchr,1,},
	{0xe2d7f2a6,"strtoul",(void*)libc::s_strtoul,1,},
	{0xd141afd3,"memcpy",(void*)libc::s__aeabi_memcpy,0,},
    {0xbb444062,"bsd_signal",(void*)libc::s_bsd_signal,0,},
    {0xb8669b99,"raise",(void*)libc::s_raise,1,},
    {0xbbad4c48,"getpid",(void*)libc::s_getpid,0,},
    {0x3bd7e17b,"strcmp",(void*)libc::s_strcmp,0},
};


int g_sym_cnt = sizeof(g_syms)/sizeof(g_syms[0]);