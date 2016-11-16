#include "../jvm/jvm.h"
#include "runtime.h"
#include "string.h"
#include "stdlib.h"
#include <map>
#include "../engine.h"
#include "../crc32.h"

#ifdef _MSC_VER
#include "windows.h"
#include "time.h"
#else
#include <sys/mman.h>
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
	void* addr = s_mmap(0,v_r0,PROT_NONE,MAP_PRIVATE,-1,0);
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
		uc_mem_unmap(g_uc,v_r0,0x1000);
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
	int addr = v_r0;
	int value =v_r1;
	for(int i = 0; i < v_r2; i++)
	{
		uc_mem_write(g_uc,addr,&value,1);
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

	return 0;
}

void* libc::s__aeabi_memset(void*)
{
	int addr = v_r0;
	int value =v_r2;
	for(int i = 0; i < v_r1; i++)
	{
		uc_mem_write(g_uc,addr,&value,1);
	}

	if(libc::v_lr &1)
		libc::v_lr-=1;
#ifdef _MSC_VER
    printf("s__aeabi_memset(0x%x,0x%x,0x%x)\n",v_r0,v_r1,v_r2);
#else
    printf(RED "s__aeabi_memset(0x%x,0x%x,0x%x)\n" RESET,v_r0,v_r1,v_r2);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

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
	void* addr = 0;
	if(v_r0 == 0)
	{
		void* addr = s_mmap(0,v_r1,PROT_NONE,MAP_PRIVATE,-1,0);
		uc_reg_write(g_uc,UC_ARM_REG_R0,&addr);
	}
	else
	{
		uc_reg_write(g_uc,UC_ARM_REG_R0,&v_r0);
		addr = (void*)v_r0;
	}
#ifdef _MSC_VER
	printf("mmap(0x%x,0x%x,0x%x)-> 0x%x\n",v_r0,v_r1,v_r2,addr);
#else
	printf(RED "mmap(0x%x,0x%x,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,v_r2,addr);
#endif

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
	char buf[256] ={0};

	if(v_r0)
	{
		uc_mem_read(g_uc,v_r0,buf,256);
		value = (int)s_dlopen(buf,0);
	}
	
#ifdef _MSC_VER
	printf("dlopen (%s,0x%x)-> 0x%x\n",buf,v_r1,value);
#else
	printf(RED "dlopen(%s,0x%x)-> 0x%x\n" RESET,v_r0,v_r1,value);
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

#ifdef _MSC_VER
	printf("strdup (%s)-> 0x%x\n",buf,value);
#else
	printf(RED "strdup(%s)-> 0x%x\n" RESET,v_r0,value);
#endif


	uc_reg_write(g_uc,UC_ARM_REG_PC,&libc::v_lr);

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
	{0x8463960a,"memset",(void*)libc::s_memset,1},
	{0x7f822dfe,"__aeabi_memset",(void*)libc::s__aeabi_memset,1},
	{0x84e4836b,"mmap",(void*)libc::sys_mmap,1,0xc0},
	{0x5e85da63,"cacheflush",(void*)libc::sys_cacheflush,1,0xf0002},
	{0x2aa01427,"__aeabi_memcpy",(void*)libc::s__aeabi_memcpy,1},
	{0xfb512a1b,"dlopen",(void*)libc::sys_dlopen,1},
	{0xed89f56b,"__system_property_get",(void*)libc::s__system_property_get,1},
	{0x36437e34,"gettimeofday",(void*)libc::s_gettimeofday,1},
	{0xbc836fa7,"strdup",(void*)libc::s_strdup,1},
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
		addr = v_pc -4;

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
	else if((v_pc & 0xffffff00) == JVM_INTERFACE_ADDRESS)
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
	else
	{	
		std::map<int,void*>::iterator iter = g_svc_map.find(v_r7);
		if(iter != g_svc_map.end())
		{
			f func = (f)iter->second;
			func(0);
		}
		
	}

	return 0;
}
