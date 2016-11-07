#ifndef __RUNTIME_H__
#define __RUNTIME_H__
#include "../linker.h"


#define FUNCTION_VIRTUAL_ADDRESS  0x10000000

typedef struct _symbols{
	unsigned int hash;
	const char * name;
	void* func;
	unsigned int model;
	unsigned int vaddr;
}symbols;

typedef struct _symbols_map{
	unsigned int vaddr;
    void* (*func)(void*);
}symbols_map;


struct soinfo;



class libc
{

public:
	libc();
	~libc();

	static int init_symbols();
	static Elf32_Sym* get_symbols(const char* name,unsigned int hash);	
	static int dispatch();
	void init();

private:
	static Elf32_Sym sym;
	static soinfo* si;
    static int sym_cnt;
	static int v_pc;
	static int v_lr;
	static int v_sp;
	static int v_r0;
	static int v_r1;
	static int v_r2;
	static int v_cpsr;
	static int v_spsr;
public:
	static void* s_malloc(void*);
	static void* s_memset(void*);
	static void* s__aeabi_memset(void*);
};



#endif