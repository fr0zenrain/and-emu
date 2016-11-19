#include "emulator.h"
#include "../crc32.h"
#include "../../capstone/include/capstone.h"
#include "string.h"
//#include "dlfcn.h"
#include "../engine.h"
#include "../jvm/jvm.h"
#include "runtime.h"
#include <map>
using namespace std;


#ifdef _MSC_VER
#include "windows.h"
#else
#include <sys/mman.h>
#endif


Elf32_Sym emulator::sym ;

symbols_map* g_symbol_map = 0;
std::map<int,void*> g_svc_map;

extern uc_engine* g_uc;
extern int g_sym_cnt;

extern func_info g_invoke_func[];
extern func_info g_native_func[];
extern symbols g_syms[];

unsigned int emulator::v_pc =0;
unsigned int emulator::v_lr =0;
unsigned int emulator::v_sp =0;
unsigned int emulator::v_cpsr =0;
unsigned int emulator::v_spsr =0;

unsigned int emulator::v_r0 =0;
unsigned int emulator::v_r1 =0;
unsigned int emulator::v_r2 =0;
unsigned int emulator::v_r3 =0;
unsigned int emulator::v_r4 =0;
unsigned int emulator::v_r5 =0;
unsigned int emulator::v_r6 =0;
unsigned int emulator::v_r7 =0;
unsigned int emulator::v_r8 =0;

emulator* emulator::instance = 0;

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

emulator* emulator::get_emulator()
{
    if(instance == 0)
    {
        return new emulator();
    }

    return instance;
}

emulator::emulator()
{
    uint64_t addr = (uint64_t)FUNCTION_VIRTUAL_ADDRESS;
    uc_err  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
    if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
    int mem_size = 4*1024*1024;
    g_uc = uc;

    //mmap svc stub
    err=uc_mem_map(uc,addr,mem_size,UC_PROT_ALL);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_mem_map() with error returned: %u\n", err);
    }

    memset(&sym,0,sizeof(Elf32_Sym));
    sym.st_value = FUNCTION_VIRTUAL_ADDRESS +0x1000;
    init_symbols();
    init_emulator();
}


int emulator::init_emulator()
{
    init_stack();
    init_jvm();
    load_library();
    init_ret_stub();
	return 1;
}

int emulator::update_cpu_model()
{
    if (v_lr & 1)
    {
        v_lr-=1;
        v_cpsr |= 0x20;
        uc_reg_write(g_uc,UC_ARM_REG_CPSR,&v_cpsr);
    }

    uc_reg_write(g_uc,UC_ARM_REG_PC,&v_lr);
    return 1;
}

int emulator::init_symbols()
{
    int svc_thumb = 0xdf00df00;
    int svc_arm = 0xef000000;
    uc_err err ;
    g_symbol_map = (symbols_map*)malloc(g_sym_cnt*sizeof(symbols_map));
    memset(g_symbol_map,0,g_sym_cnt*sizeof(symbols_map));

    qsort(g_syms,g_sym_cnt,sizeof(symbols),hash_compare);

    for(int i = 0; i < g_sym_cnt;i++)
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

    qsort(g_symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);

    return 1;
}

Elf32_Sym* emulator::get_symbols(const char* name,unsigned int hash)
{
    int len = strlen(name);
    int crc32 = getcrc32(name,len);
    symbols*  s= (symbols*)bsearch(&crc32,g_syms,g_sym_cnt,sizeof(symbols),hash_compare);
    if(s)
    {
        sym.st_size = len;
        sym.st_name =1;
        sym.st_value = s->vaddr;
    }

    return &sym;
}

int emulator::dispatch()
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

    if((v_cpsr >> 5) & 1)
        addr = v_pc -1;
    else
        addr = v_pc - 4;

    printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x cpsr %x\n",v_pc,v_lr,v_sp,v_r0,v_r1,v_r2, v_r3,v_cpsr);

    if((v_pc & 0xf0000000) == FUNCTION_VIRTUAL_ADDRESS)
    {
        symbols_map*  s= (symbols_map*)bsearch(&addr,g_symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);
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

void emulator::hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
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

    if(*(unsigned int*)buf == 0)
    {
        uc_emu_stop(uc);
        return ;
    }

    csh handle;
    cs_insn *insn;
    cs_mode mode = size == 2? CS_MODE_THUMB:CS_MODE_ARM;
    cs_err err = cs_open(CS_ARCH_ARM, mode, &handle);
    if(err == CS_ERR_OK)
    {
        cs_option(handle, CS_OPT_SYNTAX, 0);
        cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

        int count = cs_disasm(handle,(unsigned char*) buf, 4, address, 0, &insn);
        if(count)
        {
            int offset = (int)insn->address-si->base;
            if(insn->size == 2)
                printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,offset,*(unsigned short*)buf, insn->mnemonic, insn->op_str);
            else
                printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,offset,*(unsigned int*)buf, insn->mnemonic, insn->op_str);
        }
    }
}

void emulator::hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    dispatch();
}

void emulator::start_emulator(unsigned int pc, soinfo * si)
{
    uc_err err;
    uc_hook trace1,trace2;

    uintptr_t lr = EMULATOR_PAUSE_ADDRESS;
    err=uc_reg_write(g_uc, UC_ARM_REG_PC, &pc);
    err=uc_reg_write(g_uc, UC_ARM_REG_LR, &lr);


#ifdef _MSC_VER
    err=uc_hook_add(g_uc, &trace1, UC_HOOK_CODE, (void*)hook_code, (void*)si, 1,1);
#else
    err=uc_hook_add(g_uc, &trace1, UC_HOOK_CODE, (void*)hook_code, (void*)si, 1,0);
#endif
    err=uc_hook_add(g_uc, &trace2, UC_HOOK_INTR, (void*)hook_inter, (void*)si, 1, 0);

    err = uc_emu_start(g_uc,(uint64_t)pc,pc+0xffff,0,0);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_emu_start() with error returned: %u\n", err);
    }
}

int emulator::init_stack()
{
    unsigned int stack_base = 0xbeb00000;
    unsigned int sp = 0xbef00000;
    //0xbeb00000--0xbef00000
    uc_err err = uc_mem_map(uc,stack_base,sp - stack_base,UC_PROT_READ|UC_PROT_WRITE);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_mem_map() with error returned: %u\n", err);
        return 0;
    }

    err=uc_reg_write(uc, UC_ARM_REG_SP, &sp);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_mem_map() with error returned: %u\n", err);
        return 0;
    }

    printf("mmap stack ,base %x size %dM\n",stack_base,(sp - stack_base)/(1024*1024));

    return 1;
}

int emulator::load_library()
{
    libc* c = new libc();
    c->init(this);
    void* h1 = s_dlopen("libm.so",0);
    void* h2 = s_dlopen("libstdc++.so",0);
    void* h3 = s_dlopen("liblog.so",0);
    void* h4 = s_dlopen("libz.so",0);
    //void* h5 = s_dlopen("libdvm.so",0);

    return 1;
}

int emulator::init_env_func(void* invoke, void* addr)
{
    unsigned int thumb_addr = 0;
    unsigned int svc_thumb = 0xdf00df00;
    uc_err err = uc_mem_map(uc,JVM_INVOKE_ADDRESS,0x1000,UC_PROT_ALL);
    if(err != UC_ERR_OK)
    {
        return 0;
    }
    unsigned int func = JVM_INVOKE_ADDRESS;
    for(int i = 0; i < 8 ; i++)
    {
        unsigned int bak =  i| 0x2700;
        bak <<= 16;
        bak |= 0xb480;
        thumb_addr = func | 1;//add thumb tag
        err = uc_mem_write(uc,(int)invoke+i*4,&thumb_addr,4);
        err = uc_mem_write(uc,(int)func,&bak,4);
        err = uc_mem_write(uc,(int)func+4,&svc_thumb,4);
        func += 8;
    }

    err = uc_mem_map(uc,JVM_INTERFACE_ADDRESS,0x8000,UC_PROT_ALL);
    if(err != UC_ERR_OK)
    {
        return 0;
    }

    func = JVM_INTERFACE_ADDRESS;
    for(int i = 0; i < 0x1000/8 ; i++)
    {
        unsigned int bak =  i| 0x2700;
        bak <<= 16;
        bak |= 0xb480;
        thumb_addr = func | 1;//add thumb tag
        err = uc_mem_write(uc,(int)addr+i*4,&thumb_addr,4);
        err = uc_mem_write(uc,(int)func,&bak,4);
        err = uc_mem_write(uc,(int)func+4,&svc_thumb,4);
        func += 8;
    }

    return 1;
}

int emulator::init_jvm()
{
    //alloc javavm
    JNIEnvExt* jvm= (JNIEnvExt*)s_mmap(0,0x1000,PROT_NONE,MAP_PRIVATE,-1,0);
    if(jvm == 0)
    {
        return 0;
    }
    void* invoke_interface = s_mmap(0,0x1000,PROT_NONE,MAP_PRIVATE,-1,0);
    if(invoke_interface == 0)
    {
        return 0;
    }

    unsigned int addr = (int)jvm + offsetof(JavaVMExt,funcTable);
    uc_mem_write(uc,addr,&invoke_interface,4);
    //jnienv
    void* env = s_mmap(0,0x1000,PROT_NONE,MAP_PRIVATE,-1,0);
    if(env == 0)
    {
        return 0;
    }

    void* native_interface = s_mmap(0,0x1000,PROT_NONE,MAP_PRIVATE,-1,0);
    if(native_interface == 0)
    {
        return 0;
    }

    addr = (int)env + offsetof(JNIEnvExt,funcTable);
    uc_mem_write(uc,addr,&native_interface,4);
    init_env_func(invoke_interface,native_interface);
    uc_reg_write(uc, UC_ARM_REG_R0, &jvm);
    uc_reg_write(uc, UC_ARM_REG_R1, &env);
    JNIEnv = (unsigned int)env;

    return 1;
}

int emulator::init_ret_stub()
{
    unsigned int svc_thumb = 0xdf00df00;

    uc_err  err=uc_mem_map(uc,EMULATOR_PAUSE_ADDRESS,0x1000,UC_PROT_ALL);

    err = uc_mem_write(uc,(uint64_t)EMULATOR_PAUSE_ADDRESS,&svc_thumb,4);

    return 1;
}