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
#define JVM_INVOKE_ADDRESS  0x20000000
#define JVM_INTERFACE_ADDRESS  0x20001000

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
	static unsigned int v_pc;
	static unsigned int v_lr;
	static unsigned int v_sp;
	static unsigned int v_r0;
	static unsigned int v_r1;
	static unsigned int v_r2;
	static unsigned int v_r3;
	static unsigned int v_r4;
	static unsigned int v_r5;
	static unsigned int v_r6;
	static unsigned int v_r7;
	static unsigned int v_r8;
	static unsigned int v_cpsr;
	static unsigned int v_spsr;
public:
	static void* s_malloc(void*);
	static void* s_memset(void*);
	static void* s__aeabi_memset(void*);
	static void* sys_mmap(int type);
	static void* sys_cacheflush(int type);

public:
	static unsigned int get_r0(){return v_r0;}
	static unsigned int get_r1(){return v_r1;}
	static unsigned int get_r2(){return v_r2;}
	static unsigned int get_r3(){return v_r3;}
	static unsigned int get_r4(){return v_r4;}
	static unsigned int get_r5(){return v_r5;}
	static unsigned int get_r6(){return v_r6;}
	static unsigned int get_r7(){return v_r7;}
	static unsigned int get_r8(){return v_r7;}

	static unsigned int get_pc(){return v_pc;}
	static unsigned int get_lr(){return v_lr;}
	static unsigned int get_sp(){return v_sp;}

};



#endif