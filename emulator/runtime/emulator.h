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

class libc;
struct soinfo;


typedef void* (*ff)(void*);

typedef struct _symbols_map{
    unsigned int vaddr;
    ff func;
}symbols_map;

class emulator{

public:
    emulator(uc_mode mode);
    ~emulator();

    static int init_symbols();
	static const char* get_symbols(int vaddr);
    static Elf32_Sym* get_symbols(const char* name,unsigned int hash);
    static int dispatch();
    static emulator* get_emulator(uc_mode mode = UC_MODE_THUMB);
	int dispose();

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
    static int show_disasm;
    static int update_cpu_model();

public:
    static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    static void hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
    void start_emulator(unsigned int pc, soinfo * si);
    static void build_proc_self_maps();
    int init_jvm();
    unsigned int get_jvm_jnienv(){ return JNIEnv;}
    unsigned int get_jvm(){ return JVM;}
    int save_signal_handler(int sig,void* handler);
    int process_signal(int sig);
	int set_breakpoint(int addr);
	int process_breakpoint();
    int save_cpu_status();
    int restore_cpu_status();
    static int get_module_pid(){return main_pid;}
    static soinfo* get_main_module(){return module_info;}
    static soinfo* get_fake_solist(){return fake_solist;}
    static int init_fake_soinfolist();
    unsigned int get_global_jnienv(){return JNIEnv;}
	void* get_app_object(){return app_object;}
private:
    static emulator* instance;
    static uc_context* context;
    static uc_engine* uc;
	static soinfo* helper_info;
    static symbols_map* symbol_map;
    unsigned int JNIEnv;
	unsigned int JVM;
	void* app_object;
    int init_emulator();
    int init_vectors();
    int init_stack();
    int load_library();
    int init_env_func(void* invoke, void* addr);
    int init_ret_stub();
	static int get_module_base(int addr);
	static unsigned int get_helper_symbols(const char* name);

    std::map<unsigned int,void*> signal_map;
	std::map<unsigned int,unsigned int> bp_list;

    uc_hook trace_code;
    uc_hook trace_inter;
    uc_hook trace_unmap;
	libc* c;
	static int main_pid;
	static soinfo* module_info;
	static soinfo* fake_solist;
};

#endif