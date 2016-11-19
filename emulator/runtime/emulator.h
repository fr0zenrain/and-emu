#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include "../linker.h"
#include "../../include/unicorn/unicorn.h"


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
#define EMULATOR_MEMORY_START 0x40000000
#define EMULATOR_PAUSE_ADDRESS  0x80000000

struct soinfo;

class emulator{

public:
    emulator(uc_mode mode);
    ~emulator();

    static int init_symbols();
    static Elf32_Sym* get_symbols(const char* name,unsigned int hash);
    static int dispatch();
    static emulator* get_emulator(uc_mode mode = UC_MODE_THUMB);

private:
    static Elf32_Sym sym;
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

    static int update_cpu_model();

public:
    static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    void start_emulator(unsigned int pc, soinfo * si);
    static void build_proc_self_maps();

private:
    static emulator* instance;
    uc_engine* uc;
    unsigned int JNIEnv;
    int init_emulator();
    int init_stack();
    int load_library();
    int init_env_func(void* invoke, void* addr);
    int init_jvm();
    int init_ret_stub();
};

#endif