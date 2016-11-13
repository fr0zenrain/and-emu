
#include "runtime.h"
#include "string.h"
#include "stdlib.h"
#include "../engine.h"
#include "../crc32.h"

#ifdef _MSC_VER
#include "windows.h"
#else
#include <sys/mman.h>
#endif

symbols_map* g_symbol_map = 0;
extern uc_engine* g_uc;
soinfo* libc::si =0;
int libc::v_pc =0;
int libc::v_lr =0;
int libc::v_sp =0;
int libc::v_cpsr =0;
int libc::v_spsr =0;

int libc::v_r0 =0;
int libc::v_r1 =0;
int libc::v_r2 =0;

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
	{0x8463960a,"memset",(void*)libc::s_memset,1},
	{0x7f822dfe,"__aeabi_memset",(void*)libc::s__aeabi_memset,1},

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
	int addr = 0;
	uc_err err=uc_reg_read(g_uc, UC_ARM_REG_PC, &v_pc);
	err=uc_reg_read(g_uc, UC_ARM_REG_CPSR, &v_cpsr);
	err=uc_reg_read(g_uc, UC_ARM_REG_SPSR, &v_spsr);
	err=uc_reg_read(g_uc, UC_ARM_REG_LR, &v_lr);
	err=uc_reg_read(g_uc, UC_ARM_REG_SP, &v_sp);
	err=uc_reg_read(g_uc, UC_ARM_REG_R0, &v_r0);
	err=uc_reg_read(g_uc, UC_ARM_REG_R1, &v_r1);
	err=uc_reg_read(g_uc, UC_ARM_REG_R2, &v_r2);
	if(v_lr&1)
		addr = v_pc -1;
	else
		addr = v_pc -4;

	printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x cpsr %x\n",v_pc,v_lr,v_sp,v_r0,v_r1,v_r2,v_cpsr);
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

	return 0;
}
