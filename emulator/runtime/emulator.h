#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include "../linker.h"
#include "../../include/unicorn/unicorn.h"
#include <map>

using namespace std;


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
    static unsigned int v_eip;
    static unsigned int v_esp;
    static unsigned int v_ebp;
    static unsigned int v_eax;
    static unsigned int v_ecx;
    static unsigned int v_ebx;
    static unsigned int v_edx;
    static unsigned int v_esi;
    static unsigned int v_edi;
    static unsigned int v_eflags;

public:
    static unsigned int get_eip(){return v_eip;}
    static unsigned int get_esp(){return v_esp;}
    static unsigned int get_ebp(){return v_ebp;}
    static unsigned int get_eax(){return v_eax;}
    static unsigned int get_ecx(){return v_ecx;}
    static unsigned int get_ebx(){return v_ebx;}
    static unsigned int get_edx(){return v_edx;}
    static unsigned int get_esi(){return v_esi;}
    static unsigned int get_edi(){return v_edi;}
    static int update_cpu_model();

	static unsigned int get_r0(){return 0;}
    static unsigned int get_r1(){return 0;}
    static unsigned int get_r2(){return 0;}
    static unsigned int get_r3(){return 0;}
    static unsigned int get_r4(){return 0;}
    static unsigned int get_r5(){return 0;}
    static unsigned int get_sp(){return 0;}
    static unsigned int get_pc(){return 0;}
    static unsigned int get_lr(){return 0;}

public:
    static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    static void hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    void start_emulator(unsigned int pc, soinfo * si);
    static void build_proc_self_maps();
    unsigned int get_jvm_jnienv(){ return JNIEnv;}
    int save_signal_handler(int sig,void* handler);
    int process_signal(int sig);

private:
    static emulator* instance;
    static uc_engine* uc;
	static soinfo* helper_info;
    unsigned int JNIEnv;
    int init_emulator();
    int init_stack();
    int load_library();
    int init_env_func(void* invoke, void* addr);
    int init_jvm();
    int init_ret_stub();
	static unsigned int get_helper_symbols(const char* name);

    std::map<unsigned int,void*> signal_map;

    uc_hook trace_code;
    uc_hook trace_inter;
    uc_hook trace_unmap;
};

#endif