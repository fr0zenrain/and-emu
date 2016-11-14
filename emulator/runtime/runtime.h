#ifndef __RUNTIME_H__
#define __RUNTIME_H__
#include "../linker.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define FUNCTION_VIRTUAL_ADDRESS  0x10000000

typedef void* (*f)(void*);

typedef struct _symbols{
	unsigned int hash;
	const char * name;
	void* func;
	unsigned int model;
	int number;
	unsigned int vaddr;
}symbols;

typedef struct _symbols_map{
	unsigned int vaddr;
    f func;
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
	static int v_r6;
	static int v_r7;
	static int v_cpsr;
	static int v_spsr;
public:
	static void* s_malloc(void*);
	static void* s_memset(void*);
	static void* s__aeabi_memset(void*);
	static void* sys_mmap(int type);
	static void* sys_cacheflush(int type);
};



#endif