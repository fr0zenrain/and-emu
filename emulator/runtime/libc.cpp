#include "../jvm/jvm.h"
#include "runtime.h"
#include "string.h"
#include "stdlib.h"
#include "../type.h"
#include <map>
#include "../engine.h"
#include "../crc32.h"
#include "../../capstone/include/capstone.h"

#ifdef _MSC_VER
#include "windows.h"
#include "time.h"
#include "io.h"
#else
#include <sys/mman.h>
#include <fcntl.h>
#endif

symbols_map* g_symbol_map = 0;
std::map<int,void*> g_svc_map;

extern uc_engine* g_uc;
extern func_info g_invoke_func[];
extern func_info g_native_func[];

soinfo* libc::si =0;
unsigned int libc::v_pc =0;
unsigned int libc::v_lr =0;
unsigned int libc::v_sp =0;
unsigned int libc::v_cpsr =0;
unsigned int libc::v_spsr =0;

unsigned int libc::v_r0 =0;
unsigned int libc::v_r1 =0;
unsigned int libc::v_r2 =0;
unsigned int libc::v_r3 =0;
unsigned int libc::v_r4 =0;
unsigned int libc::v_r5 =0;
unsigned int libc::v_r6 =0;
unsigned int libc::v_r7 =0;
unsigned int libc::v_r8 =0;

int libc::sym_cnt =0;
Elf32_Sym libc::sym ;

void* libc::s_malloc(void*)
{
	//void* addr = s_mmap(0,v_r0,PROT_NONE,MAP_PRIVATE,-1,0);
	void* addr = sys_malloc(v_r0);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	if(libc::v_lr &1)
		libc::v_lr-=1;
#ifdef _MSC_VER
    printf("malloc(0x%x) ->0x%x\n",v_r0,addr);
#else
    printf(RED "malloc(0x%x) ->0x%x\n" RESET,v_r0,addr);
#endif
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	//uc_reg_write(g_uc,UC_ARM_REG_CPSR,&libc::v_spsr);
	return addr;
}

void* libc::s_free(void*)
{
	if(libc::v_lr &1)
		libc::v_lr-=1;
	
	if(v_r0)
		sys_free((void*)v_r0);
		//uc_mem_unmap(g_uc,v_r0,0x1000);
#ifdef _MSC_VER
	printf("free(0x%x)\n",v_r0);
#else
	printf(RED "free(0x%x)\n" RESET,v_r0);
#endif
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	return 0;
}

void* libc::s_memset(void*)
{
	unsigned int addr = v_r0;
	int value =v_r1;
	for(int i = 0; i < v_r2; i++)
	{
		uc_mem_write(g_uc,addr,&value,1);
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("memset(0x%x,0x%x,0x%x) -> 0x%x\n",v_r0,v_r1,v_r2, v_r0);
#else
	printf(RED "memset(0x%x,0x%x,0x%x) -> 0x%x\n" RESET,v_r0,v_r1,v_r2,v_r0);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&libc::v_r0);

	return 0;
}

void* libc::s__aeabi_memset(void*)
{
	unsigned int addr = v_r0;
	int value =v_r2;
	for(int i = 0; i < v_r1; i++)
	{
		uc_mem_write(g_uc,addr+i,&value,1);
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
    printf("s__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x\n",v_r0,v_r1,v_r2,v_r0);
#else
    printf(RED "s__aeabi_memset(0x%x,0x%x,0x%x)  -> 0x%x \n" RESET,v_r0,v_r1,v_r2,v_r0);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&libc::v_r0);

	return 0;
}

void* libc::s__aeabi_memcpy(void*)
{
	void* ptr = malloc(v_r2);
	if(ptr)
	{
		uc_mem_read(g_uc,v_r1,ptr,v_r2);
		uc_mem_write(g_uc,v_r0,ptr,v_r2);
		free(ptr);
	}

	
	if(libc::v_lr &1)
		libc::v_lr-=1;
#ifdef _MSC_VER
	printf("s__aeabi_memcpy(0x%x,0x%x,0x%x)\n",v_r0,v_r1,v_r2);
#else
	printf(RED "s__aeabi_memcpy(0x%x,0x%x,0x%x)\n" RESET,v_r0,v_r1,v_r2);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&v_r2);
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

	return 0;
}

void* libc::sys_mmap(int type)
{
    uc_err err;
	void* addr = 0;
	if(v_r0 == 0)
	{
		addr = s_mmap(0,v_r1,PROT_NONE,MAP_PRIVATE,-1,0);
		err = uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	}
	else
	{
        err = uc_reg_write(g_uc,UC_ARM_REG_R0,&v_r0);
		addr = (void*)v_r0;
	}

    if(libc::v_lr &1)
        libc::v_lr-=1;

#ifdef _MSC_VER
	printf("mmap(0x%x,0x%x,0x%x)-> 0x%x\n",v_r0,v_r1,v_r2,addr);
#else
	printf(RED "mmap(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,v_r2,addr);
#endif

    err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

	return 0;
}

void* libc::sys_cacheflush(int type)
{
    int value = 0;
	if(type == 0)
		uc_reg_write(g_uc,UC_ARM_REG_R0,&v_r0);
#ifdef _MSC_VER
	printf("cacheflush (0x%x,0x%x,0x%x)-> 0x%x\n",v_r0,v_r1,v_r2,value);
#else
	printf(RED "cacheflush(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,v_r2,value);
#endif

	return 0;
}

void* libc::sys_dlopen(void*)
{
	int value = 0;
	soinfo* si = 0;
	char buf[256] ={0};

	if(v_r0)
	{
		uc_mem_read(g_uc,v_r0,buf,256);
		si = (soinfo*)s_dlopen(buf,0);
	}
	
#ifdef _MSC_VER
	printf("dlopen (%s,0x%x)-> 0x%x\n",buf,v_r1,value);
#else
	printf(RED "dlopen(%s,0x%x)-> 0x%x\n" RESET,buf,v_r1,value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	if(libc::v_lr &1)
		libc::v_lr-=1;

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

	return 0;
}

void* libc::s__system_property_get(void*)
{
	int ret = 0;
	char name[32] ={0};
	char value[92] ={0};

	if(v_r0 && v_r1)
	{
		uc_mem_read(g_uc,v_r0,name,32);
		ret = get_prop(name,value);
		if(ret)
		{
			uc_mem_write(g_uc,v_r1,value,32);
		}
	}
#ifdef _MSC_VER
	printf("__system_property_get (%s,%s)\n",name,value);
#else
	printf(RED "__system_property_get(%s,%s)\n" RESET,name,value);
#endif

	if(libc::v_lr &1)
		libc::v_lr-=1;

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return 0;
}

void* libc::s_gettimeofday(void*)
{
	struct s_timeval tv;
	int value = 0;

	if(v_r0)
	{
		tv.tv_sec = time(0);
		tv.tv_usec = 960256;

		uc_mem_write(g_uc,v_r0,&tv,sizeof(s_timeval));
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("gettimeofday (%x,0x%x)-> 0x%x\n",v_r0,v_r1,value);
#else
	printf(RED "gettimeofday(%x,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,value);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strdup(void*)
{
	uc_err err;
	char buf[256] ={0};
	int value = 0;

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}
	int len = strlen(buf);
	value = (int)sys_malloc(len+1);
	err = uc_mem_write(g_uc,value,buf,len+1);

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("strdup (\"%s\")-> 0x%x\n",buf,value);
#else
	printf(RED "strdup(\"%s\")-> 0x%x\n" RESET,buf,value);
#endif
	
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strlen(void*)
{
	uc_err err;
	int i = 0;
	char buf[256] ={0};

	if(v_r0)
	{
		for(i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("strlen (\"%s\")-> 0x%x\n",buf,i);
#else
	printf(RED "strlen(\"%s\")-> 0x%x\n" RESET,buf,i);
#endif
	
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&i);

	return 0;
}

void* libc::s_strncmp(void*)
{
	uc_err err;
	int value = 0;
	char buf[256] ={0};
	char buf1[256] ={0};

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	if(v_r1)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r1+i,&buf1[i],1);
			if(buf1[i] == 0)
				break;
		}
	}

	value = strncmp(buf,buf1,v_r3);

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("strncmp (%s,%s,0x%x)-> 0x%x\n",buf,buf1,v_r3,value);
#else
	printf(RED "strlen(%s,%s,0x%x)-> 0x%x\n" RESET,buf,buf1,v_r3,value);
#endif
	
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_open(void*)
{
	uc_err err;
	char buf[256] ={0};
	int value = 1;

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

#ifdef _MSC_VER
	printf("open (\"%s\")-> 0x%x\n",buf,value);
#else
	printf(RED "open(\"%s\")-> 0x%x\n" RESET,buf,value);
#endif

    value = open("maps.txt",O_RDONLY);
	
	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_read(void*)
{
	uc_err err;
	char buf[4096] ={0};
	int value = 0;

    if(v_r1)
    {
        read(v_r0,buf,v_r2);
        err = uc_mem_write(g_uc,v_r1,buf,v_r2);
    }

    if(libc::v_lr &1)
        libc::v_lr-=1;

#ifdef _MSC_VER
	printf("read (%x,%x,%x)-> 0x%x\n",v_r0,v_r1,v_r2,v_r2);
#else
	printf(RED "read(%x,%x,%x)-> 0x%x\n" RESET,v_r0,v_r1,v_r2,v_r2);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&v_r2);

	return 0;
}

void* libc::sys_mprotect(void*)
{
	uc_err err;
	int value = 0;
	
#ifdef _MSC_VER
	printf("mprotect (%x,%x,%x)-> 0x%x\n",v_r0,v_r1,v_r2,value);
#else
	printf(RED "mprotect(%x,%x,%x)-> 0x%x\n" RESET,v_r0,v_r1,v_r2,value);
#endif

	if(libc::v_lr &1)
		libc::v_lr-=1;

	err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
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

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	if(v_r1)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r1+i,&format[i],1);
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
		err = uc_mem_read(g_uc,v_sp+i*4,&stack[i],4);
		if(err != UC_ERR_OK)
			break;
	}

	value = sscanf(buf,format,arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

	err = uc_mem_write(g_uc,v_r2,arg0,strlen(arg0));

	if(libc::v_lr &1)
		libc::v_lr-=1;

	err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strchr(void*)
{
	uc_err err ;
	char buf[256] ={0};
	char ch = (char)v_r1;
	unsigned int value = 0;

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	char* ptr = strchr(buf,ch);
	if(ptr)
	{
		value = (unsigned int)ptr - (int)buf + v_r0;
	}

#ifdef _MSC_VER
	printf("strchr (\"%s\",\"%c\")-> 0x%x\n",buf,ch,value);
#else
	printf(RED "strchr(\"%s\",\"%c\")-> 0x%x\n" RESET,buf,ch,value);
#endif

	if(libc::v_lr &1)
		libc::v_lr-=1;

	err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);
	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);

	return 0;
}

void* libc::s_strtoul(void*)
{
	uc_err err;
	char buf[256]={0};

	if(v_r0)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,v_r0+i,&buf[i],1);
			if(buf[i] == 0)
				break;
		}
	}

	const char *nptr = buf;
	char* tp = &buf[256];
	char **endptr = &tp;
	register int base = v_r2;
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;


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
	printf("strtoul (\"%s\",0x%x,0x%x)-> 0x%x\n",buf,v_r1,v_r2,acc);
#else
	printf(RED "strtoul(\"%s\",0x%x)-> 0x%x\n" RESET,buf,v_r1,v_r2,acc);
#endif

	if(libc::v_lr &1)
		libc::v_lr-=1;

	err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

	err = uc_reg_write(g_uc,UC_ARM_REG_R0,&acc);
	return 0;
}

void* libc::s_bsd_signal(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("bsd_signal(0x%x,0x%x)-> 0x%x\n",v_r0,v_r1, value);
#else
    printf(RED "bsd_signal(0x%x,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,value);
#endif

    if(libc::v_lr &1)
        libc::v_lr-=1;

    err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

void* libc::s_raise(void*)
{
    uc_err err;
    int value = 0;

#ifdef _MSC_VER
    printf("raise(0x%x)-> 0x%x\n",v_r0,  value);
#else
    printf(RED "raise(0x%x)-> 0x%x\n" RESET,v_r0, value);
#endif

    if(libc::v_lr &1)
        libc::v_lr-=1;

    err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

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

    if(libc::v_lr &1)
        libc::v_lr-=1;

    err = uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

    err = uc_reg_write(g_uc,UC_ARM_REG_R0,&value);
    return 0;
}

int hash_compare(const void *a,const void *b) 
{ 
	const unsigned int *va = (unsigned int *) a;
	const unsigned int *vb = (unsigned int *) b;
	if(*va < *vb)
		return -1;
	else if(*va > *vb)
		return 1;
	else
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
};


libc::libc()
{
	memset(&sym,0,sizeof(Elf32_Sym));
	sym.st_value = FUNCTION_VIRTUAL_ADDRESS +0x1000;
}


int libc::init_symbols()
{
	int svc_thumb = 0xdf00df00;
	int svc_arm = 0xef000000;
	uc_err err ;
    sym_cnt = sizeof(g_syms)/sizeof(g_syms[0]);
    g_symbol_map = (symbols_map*)malloc(sym_cnt*sizeof(symbols_map));
	memset(g_symbol_map,0,sym_cnt*sizeof(symbols_map));

	qsort(g_syms,sym_cnt,sizeof(symbols),hash_compare);

	for(int i = 0; i < sizeof(g_syms)/sizeof(g_syms[0]);i++)
	{
		g_syms[i].vaddr = (FUNCTION_VIRTUAL_ADDRESS+i*4);
		g_symbol_map[i].vaddr=(FUNCTION_VIRTUAL_ADDRESS+i*4);
		g_symbol_map[i].func= (void* (*)(void*))g_syms[i].func;
		if(g_syms[i].model)
		{
			err=uc_mem_write(g_uc,g_syms[i].vaddr,&svc_thumb, 4);
		}
		else
		{
			err=uc_mem_write(g_uc,g_syms[i].vaddr,&svc_arm, 4);
		}
		
		g_syms[i].vaddr |= g_syms[i].model;
		g_symbol_map[i].vaddr |= g_syms[i].model;
		g_svc_map.insert(std::make_pair(g_syms[i].number,g_syms[i].func));
	}

    qsort(g_symbol_map,sym_cnt,sizeof(symbols_map),hash_compare);

    return 1;
}

void libc::init()
{
	si = soinfo_alloc("libc.so");
	si->emu = 1;
	si->runtime = this;
	si->load_bias = 0x10000;
	init_symbols();

	si->flags |= FLAG_LINKED;

}

Elf32_Sym* libc::get_symbols(const char* name,unsigned int hash)
{
	int len = strlen(name);
	int crc32 = getcrc32(name,len);
	symbols*  s= (symbols*)bsearch(&crc32,g_syms,sym_cnt,sizeof(symbols),hash_compare);
	if(s)
	{
		sym.st_size = len;
		sym.st_name =1;
		sym.st_value = s->vaddr;
	}

	return &sym;
}

int libc::dispatch()
{
	unsigned int addr = 0;

	uc_err err=uc_reg_read(g_uc, UC_ARM_REG_PC, &v_pc);
	err=uc_reg_read(g_uc, UC_ARM_REG_CPSR, &v_cpsr);
	err=uc_reg_read(g_uc, UC_ARM_REG_SPSR, &v_spsr);
	err=uc_reg_read(g_uc, UC_ARM_REG_LR, &v_lr);
	err=uc_reg_read(g_uc, UC_ARM_REG_SP, &v_sp);
	err=uc_reg_read(g_uc, UC_ARM_REG_R0, &v_r0);
	err=uc_reg_read(g_uc, UC_ARM_REG_R1, &v_r1);
	err=uc_reg_read(g_uc, UC_ARM_REG_R2, &v_r2);
	err=uc_reg_read(g_uc, UC_ARM_REG_R3, &v_r3);
	err=uc_reg_read(g_uc, UC_ARM_REG_R4, &v_r4);
	err=uc_reg_read(g_uc, UC_ARM_REG_R5, &v_r5);
	err=uc_reg_read(g_uc, UC_ARM_REG_R6, &v_r6);
	err=uc_reg_read(g_uc, UC_ARM_REG_R7, &v_r7);
	err=uc_reg_read(g_uc, UC_ARM_REG_R8, &v_r8);

	if(v_lr&1)
		addr = v_pc -1;
	else
		addr = v_pc - 4;

	printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x cpsr %x\n",v_pc,v_lr,v_sp,v_r0,v_r1,v_r2, v_r3,v_cpsr);

	if((v_pc & 0xf0000000) == FUNCTION_VIRTUAL_ADDRESS)
	{
		symbols_map*  s= (symbols_map*)bsearch(&addr,g_symbol_map,sizeof(g_syms)/sizeof(g_syms[0]),sizeof(symbols_map),hash_compare);
		if(s)
		{
#ifdef _MSC_VER
			CONSOLE_SCREEN_BUFFER_INFO Info;
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
			GetConsoleScreenBufferInfo(h, &Info);
			SetConsoleTextAttribute(h, FOREGROUND_RED);
			s->func(s);
			SetConsoleTextAttribute(h, Info.wAttributes );
#else
			s->func(s);
#endif
		}
	}
    else if((v_pc & 0xffffff00) == JVM_INVOKE_ADDRESS)
    {
		//restore r7
		int index = v_r7;
		err=uc_mem_read(g_uc, v_sp, &v_r7,4);
		err=uc_reg_write(g_uc, UC_ARM_REG_R7, &v_r7);
		v_sp += 4;
		err=uc_reg_write(g_uc, UC_ARM_REG_SP, &v_sp);
#ifdef _MSC_VER
		CONSOLE_SCREEN_BUFFER_INFO Info;
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
		GetConsoleScreenBufferInfo(h, &Info);
		SetConsoleTextAttribute(h, FOREGROUND_RED);
		g_invoke_func[index].f();
		SetConsoleTextAttribute(h, Info.wAttributes );
#else
		g_invoke_func[index].f();
#endif
    }
	else if((v_pc & 0xfffff000) == JVM_INTERFACE_ADDRESS)
	{
		//restore r7
		int index = v_r7;
		err=uc_mem_read(g_uc, v_sp, &v_r7,4);
		err=uc_reg_write(g_uc, UC_ARM_REG_R7, &v_r7);
		v_sp += 4;
		err=uc_reg_write(g_uc, UC_ARM_REG_SP, &v_sp);
#ifdef _MSC_VER
		CONSOLE_SCREEN_BUFFER_INFO Info;
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
		GetConsoleScreenBufferInfo(h, &Info);
		SetConsoleTextAttribute(h, FOREGROUND_RED);
		g_native_func[index].f();
		SetConsoleTextAttribute(h, Info.wAttributes );
#else
		g_native_func[index].f();
#endif
	}
	else if((v_pc & 0xffffff00) == EMULATOR_PAUSE_ADDRESS)
	{
		err = uc_emu_stop(g_uc);
        //printf("emulator pause\n");
	}
	else
	{	
		std::map<int,void*>::iterator iter = g_svc_map.find(v_r7);
		if(iter != g_svc_map.end())
		{
			f func = (f)iter->second;
#ifdef _MSC_VER
			CONSOLE_SCREEN_BUFFER_INFO Info;
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
			GetConsoleScreenBufferInfo(h, &Info);
			SetConsoleTextAttribute(h, FOREGROUND_RED);
			func(0);
			SetConsoleTextAttribute(h, Info.wAttributes );
#else
			func(0);
#endif
		}
		
	}

	return 0;
}

void libc::hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	unsigned char buf[8] = {0};
	soinfo* si = (soinfo*)user_data;
	//printf(">>> Tracing instruction at 0x%llx, instruction size = 0x%x\n", address, size);
	if (uc_mem_read(uc, address, buf, 4) != UC_ERR_OK) {
		printf("not ok - uc_mem_read fail during hook_code callback, addr: 0x%llx\n", address);
		if (uc_emu_stop(uc) != UC_ERR_OK) {
			printf("not ok - uc_emu_stop fail during hook_code callback, addr: 0x%llx\n", address);
			_exit(-1);
		}        
	}

	csh handle;
	cs_insn *insn;
    cs_mode mode = size == 2? CS_MODE_THUMB:CS_MODE_ARM;
	cs_err err = cs_open(CS_ARCH_ARM, mode, &handle);
	if(err == CS_ERR_OK)
	{
		cs_option(handle, CS_OPT_SYNTAX, 0);
		cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

		/*int count = cs_disasm(handle,(unsigned char*) buf, 4, address, 0, &insn);
		if(count)
		{
			int offset = (int)insn->address-si->base;
			if(insn->size == 2)
				printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,offset,*(unsigned short*)buf, insn->mnemonic, insn->op_str);
			else
				printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,offset,*(unsigned int*)buf, insn->mnemonic, insn->op_str);
		}*/
	}
}

void libc::hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	libc::dispatch();
}

void libc::start_emulator(unsigned int pc, soinfo * si)
{

    uc_err err;
    uc_hook trace1,trace2;

    uintptr_t lr = EMULATOR_PAUSE_ADDRESS;
    err=uc_reg_write(g_uc, UC_ARM_REG_PC, &pc);
    err=uc_reg_write(g_uc, UC_ARM_REG_LR, &lr);


#ifdef _MSC_VER
    err=uc_hook_add(g_uc, &trace1, UC_HOOK_CODE, (void*)libc::hook_code, (void*)si, 1,1);
#else
    err=uc_hook_add(g_uc, &trace1, UC_HOOK_CODE, (void*)libc::hook_code, (void*)si, 1,0);
#endif
    err=uc_hook_add(g_uc, &trace2, UC_HOOK_INTR, (void*)libc::hook_inter, (void*)si, 1, 0);

    err = uc_emu_start(g_uc,(uint64_t)pc,pc+0xffff,0,0);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_emu_start() with error returned: %u\n", err);
    }
}