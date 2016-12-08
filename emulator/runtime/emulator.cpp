#include "emulator.h"
#include "../crc32.h"
#include "../../capstone/include/capstone.h"
#include "string.h"
//#include "dlfcn.h"
#include "../engine.h"
#include "../jvm/jvm.h"
#include "runtime.h"
#include "../dlfcn.h"
#include <map>
using namespace std;


#ifdef _MSC_VER
#include "windows.h"
#else
#include <sys/mman.h>
#endif


Elf32_Sym emulator::sym ;
symbols_map* emulator::symbol_map=0;

std::map<int,void*> g_svc_map;

extern uc_engine* g_uc;
extern int g_sym_cnt;
extern func_info g_invoke_func[];
extern func_info g_native_func[];
extern symbols g_syms[];
extern soinfo* solist;

uc_engine* emulator::uc = 0;
uc_context* emulator::context = 0;
soinfo* emulator::helper_info = 0;
int emulator::show_disasm = 0;

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

emulator* emulator::get_emulator(uc_mode mode)
{
    if(instance == 0)
    {
        instance = new emulator(mode);
    }

    return instance;
}

emulator::~emulator()
{
    uc_close(uc);

	if(c)
		delete c;
}

emulator::emulator(uc_mode mode)
{
    trace_code = 0;
    trace_inter = 0;
	trace_unmap = 0;
    JNIEnv = 0;

    uint64_t addr = (uint64_t)FUNCTION_VIRTUAL_ADDRESS;
    uc_err  err = uc_open(UC_ARCH_ARM, mode, &uc);
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

    //set return address
    uintptr_t lr = EMULATOR_PAUSE_ADDRESS;
    lr |= 1;
    err=uc_reg_write(uc, UC_ARM_REG_LR, &lr);
    err=uc_reg_write(uc, UC_ARM_REG_R0, &JNIEnv);
    //void* object = sys_malloc(0x1000);
    //err=uc_reg_write(uc, UC_ARM_REG_R1, &object);
    //err = uc_context_alloc(uc, &context);
}

int emulator::dispose()
{
    soinfo* si = solist;
    while(si)
    {
        if(si)
        {
            if(si->tmp_strtab)
                free(si->tmp_strtab);
            if(si->tmp_bucket)
                free(si->tmp_bucket);
            if(si->tmp_chain)
                free(si->tmp_chain);
            if(si->tmp_symtab)
                free(si->tmp_symtab);
            if(si->tmp_dynamic)
                free(si->tmp_dynamic);
			if(si->tmp_needed)
				free(si->tmp_needed);
        }
        si = si->next;
    }

	if(symbol_map)
		free(symbol_map);

    return 1;
}

int emulator::init_emulator()
{
    init_stack();
    load_library();
    init_ret_stub();
	return 1;
}

int emulator::update_cpu_model()
{
    uc_err err = uc_reg_read(uc,UC_ARM_REG_LR,&v_lr);
    if (v_lr & 1)
    {
        v_lr-=1;
        v_cpsr |= 0x20;
        err = uc_reg_write(uc,UC_ARM_REG_CPSR,&v_cpsr);
    }
    else
    {
        v_cpsr &= ~(1 << 5);
        err = uc_reg_write(uc,UC_ARM_REG_CPSR,&v_cpsr);
    }

    err = uc_reg_write(uc,UC_ARM_REG_PC,&v_lr);
    return 1;
}

int emulator::init_symbols()
{
    int svc_thumb = 0xdf00df00;
    int svc_arm = 0xef000000;
    uc_err err ;
    symbol_map = (symbols_map*)malloc(g_sym_cnt*sizeof(symbols_map));
    memset(symbol_map,0,g_sym_cnt*sizeof(symbols_map));

    qsort(g_syms,g_sym_cnt,sizeof(symbols),hash_compare);

    for(int i = 0; i < g_sym_cnt;i++)
    {
        g_syms[i].vaddr = (FUNCTION_VIRTUAL_ADDRESS+i*4);
        symbol_map[i].vaddr=(FUNCTION_VIRTUAL_ADDRESS+i*4);
		symbol_map[i].func= (void* (*)(void*))g_syms[i].func;
        if(g_syms[i].model)
        {
            err=uc_mem_write(uc,g_syms[i].vaddr,&svc_thumb, 4);
        }
        else
        {
            err=uc_mem_write(uc,g_syms[i].vaddr,&svc_arm, 4);
        }

        g_syms[i].vaddr |= g_syms[i].model;
        symbol_map[i].vaddr |= g_syms[i].model;
        g_svc_map.insert(std::make_pair(g_syms[i].number,g_syms[i].func));
    }

    qsort(symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);

    return 1;
}

unsigned int emulator::get_helper_symbols(const char* name)
{
	unsigned int addr = 0;
	if(helper_info)
	{
		addr = (unsigned int)s_dlsym((void*)helper_info,name);
		return addr;
	}

	return 0;
}


Elf32_Sym* emulator::get_symbols(const char* name,unsigned int hash)
{
    int len = strlen(name);
    int crc32 = getcrc32(name,len);
	if(crc32 == 0xffa1e6f0 || crc32 ==0xbd2f3f6d || crc32 == 0x23398d9a)//snprint sscanf sprintf
	{
		unsigned int sym_addr = get_helper_symbols(name);
		if(sym_addr)
		{
			sym.st_size = len;
			sym.st_name =1;
			sym.st_value = sym_addr - FUNCTION_VIRTUAL_ADDRESS;
		}
	}
	else
	{

		symbols* s = (symbols*)bsearch(&crc32,g_syms,g_sym_cnt,sizeof(symbols),hash_compare);
		if(s)
		{
			sym.st_size = len;
			sym.st_name =1;
			sym.st_value = s->vaddr - FUNCTION_VIRTUAL_ADDRESS;
		}
	}

    return &sym;
}

int emulator::dispatch()
{
    unsigned int addr = 0;

    uc_err err=uc_reg_read(uc, UC_ARM_REG_PC, &v_pc);
    err=uc_reg_read(uc, UC_ARM_REG_CPSR, &v_cpsr);
    err=uc_reg_read(uc, UC_ARM_REG_SPSR, &v_spsr);
    err=uc_reg_read(uc, UC_ARM_REG_LR, &v_lr);
    err=uc_reg_read(uc, UC_ARM_REG_SP, &v_sp);
    err=uc_reg_read(uc, UC_ARM_REG_R0, &v_r0);
    err=uc_reg_read(uc, UC_ARM_REG_R1, &v_r1);
    err=uc_reg_read(uc, UC_ARM_REG_R2, &v_r2);
    err=uc_reg_read(uc, UC_ARM_REG_R3, &v_r3);
    err=uc_reg_read(uc, UC_ARM_REG_R4, &v_r4);
    err=uc_reg_read(uc, UC_ARM_REG_R5, &v_r5);
    err=uc_reg_read(uc, UC_ARM_REG_R6, &v_r6);
    err=uc_reg_read(uc, UC_ARM_REG_R7, &v_r7);
    err=uc_reg_read(uc, UC_ARM_REG_R8, &v_r8);

    printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x r7 %x cpsr %x\n",v_pc,v_lr,v_sp,v_r0,v_r1,v_r2, v_r3,v_r7, v_cpsr);

    get_emulator()->process_breakpoint();

	if((v_pc & 0xf0000000) == FUNCTION_VIRTUAL_ADDRESS)
    {
		//assume thumb
		addr = v_pc -1;
        symbols_map*  s = (symbols_map*)bsearch(&addr,symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);
		if(s == 0)
		{
			//try arm
			addr = v_pc -4;
			s = (symbols_map*)bsearch(&addr,symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);
		}
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
        err=uc_mem_read(uc, v_sp, &v_r7,4);
        err=uc_reg_write(uc, UC_ARM_REG_R7, &v_r7);
        v_sp += 4;
        err=uc_reg_write(uc, UC_ARM_REG_SP, &v_sp);
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
        err=uc_mem_read(uc, v_sp, &v_r7,4);
        err=uc_reg_write(uc, UC_ARM_REG_R7, &v_r7);
        v_sp += 4;
        err=uc_reg_write(uc, UC_ARM_REG_SP, &v_sp);
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
        //uc_emu_stop(uc);
        //uc_context_save(uc,context);
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

    /*if(!show_disasm)
    {
        return ;
    }*/

  /*  csh handle;
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

			 cs_free(insn, count);
        }
        cs_close(&handle);
    }*/
}

void emulator::hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    dispatch();
}

void emulator::hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    int r0,r1,r2,r3,r4,r5,r6,r7,pc,lr,sp;
    uc_err err=uc_reg_read(uc, UC_ARM_REG_PC, &pc);
    err=uc_reg_read(uc, UC_ARM_REG_LR, &lr);
    err=uc_reg_read(uc, UC_ARM_REG_SP, &sp);
    err=uc_reg_read(uc, UC_ARM_REG_R0, &r0);
    err=uc_reg_read(uc, UC_ARM_REG_R1, &r1);
    err=uc_reg_read(uc, UC_ARM_REG_R2, &r2);
    err=uc_reg_read(uc, UC_ARM_REG_R3, &r3);
    err=uc_reg_read(uc, UC_ARM_REG_R4, &r4);
    err=uc_reg_read(uc, UC_ARM_REG_R5, &r5);
    err=uc_reg_read(uc, UC_ARM_REG_R6, &r6);
    err=uc_reg_read(uc, UC_ARM_REG_R7, &r7);
    printf(">>> Tracing unmap at 0x%llx, block size = 0x%x\n", address, size);
    printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x r4 %x r5 %x r6 %x r7 %x\n",pc,lr,sp,r0,r1,r2,r3,r4,r5,r6,r7);
}


void emulator::start_emulator(unsigned int pc, soinfo * si)
{
    uc_err err;
    err=uc_reg_write(uc, UC_ARM_REG_PC, &pc);

    if(trace_code)
    {
        uc_hook_del(uc,trace_code);
        trace_code = 0;
    }
    else
    {
		err = uc_hook_add(uc, &trace_code, UC_HOOK_CODE, (void *) hook_code, (void *) si, 1, 0);
    }

    if(trace_inter)
    {
        uc_hook_del(uc,trace_inter);
        trace_inter = 0;
    }
    else
    {
        err=uc_hook_add(uc, &trace_inter, UC_HOOK_INTR, (void*)hook_inter, (void*)si, 1, 0);
    }

    if(trace_unmap)
    {
        uc_hook_del(uc,trace_unmap);
        trace_unmap = 0;
    }
    else
    {
        err=uc_hook_add(uc, &trace_unmap, UC_HOOK_MEM_FETCH_INVALID| UC_HOOK_MEM_INVALID,
                        (void*)hook_unmap, (void*)si, 1, 0);
    }


    int r0,r1,r2,r3,r4,r5,r6,r7,lr,sp,cpsr;
    err=uc_reg_read(uc, UC_ARM_REG_LR, &lr);
    err=uc_reg_read(uc, UC_ARM_REG_SP, &sp);
    err=uc_reg_read(uc, UC_ARM_REG_R0, &r0);
    err=uc_reg_read(uc, UC_ARM_REG_R1, &r1);
    err=uc_reg_read(uc, UC_ARM_REG_R2, &r2);
    err=uc_reg_read(uc, UC_ARM_REG_R3, &r3);
    err=uc_reg_read(uc, UC_ARM_REG_R4, &r4);
    err=uc_reg_read(uc, UC_ARM_REG_R5, &r5);
    err=uc_reg_read(uc, UC_ARM_REG_R6, &r6);
    err=uc_reg_read(uc, UC_ARM_REG_R7, &r7);
    err=uc_reg_read(uc, UC_ARM_REG_CPSR, &cpsr);

    printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x r7 %x cpsr %x\n",pc,lr,sp,r0,r1,r2,r3,r7,cpsr);

    err = uc_emu_start(uc,(uint64_t)pc,pc+0xfffff,0,0);
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
    uc_err err = uc_mem_map(uc,stack_base,sp - stack_base + 0x100000,UC_PROT_READ|UC_PROT_WRITE);
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
    c = new libc();
    c->init(this);
	helper_info = (soinfo*)s_dlopen("libhelper.so",0);
    void* h1 = s_dlopen("libm.so",0);
    void* h2 = s_dlopen("libstdc++.so",0);
    void* h3 = s_dlopen("liblog.so",0);
    void* h4 = s_dlopen("libz.so",0);
	/*void* h5 = s_dlopen("libutils.so",0);
	void* h6 = s_dlopen("libinput.so",0);
	void* h7 = s_dlopen("libgccdemangle.so",0);
	void* h8 = s_dlopen("libbinder.so",0);
	void* h9 = s_dlopen("libandroid_runtime.so",0);
	void* h10 = s_dlopen("libandroidfw.so",0);
	void* h11 = s_dlopen("libui.so",0);
	void* h12 = s_dlopen("libhardware.so",0);
	void* h13 = s_dlopen("libsync.so",0);
	void* h14 = s_dlopen("libgui.so",0);
	void* h15 = s_dlopen("libEGL.so",0);
	void* h16 = s_dlopen("libGLES_trace.so",0);
	void* h17 = s_dlopen("libGLESv2.so",0);
	void* h19 = s_dlopen("libmemtrack.so",0);
	void* h20 = s_dlopen("libexpat.so",0);
	void* h21 = s_dlopen("libnetutils.so",0);
	void* h22 = s_dlopen("libcamera_client.so",0);
	void* h23 = s_dlopen("libcamera_metadata.so",0);
	void* h24 = s_dlopen("libskia.so",0);
	void* h25 = s_dlopen("libft2.so",0);
	void* h26 = s_dlopen("libpng.so",0);
	void* h27 = s_dlopen("libjpeg.so",0);
	void* h28 = s_dlopen("libicuuc.so",0);
	void* h29 = s_dlopen("libgabi++.so",0);
	void* h30 = s_dlopen("libicui18n.so",0);
	void* h31 = s_dlopen("libsqlite.so",0);
	void* h32 = s_dlopen("libGLESv1_CM.so",0);
	void* h33 = s_dlopen("libETC1.so",0);
	void* h34 = s_dlopen("libhardware_legacy.so",0);
	void* h35 = s_dlopen("libwpa_client.so",0);
	void* h36 = s_dlopen("libsonivox.so",0);
	void* h37 = s_dlopen("libcrypto.so",0);
	void* h38 = s_dlopen("libandroid.so",0);
	void* h39 = s_dlopen("libssl.so",0);
	void* h40 = s_dlopen("libmedia.so",0);
	void* h41 = s_dlopen("libstagefright_foundation.so",0);
	void* h42 = s_dlopen("libaudioutils.so",0);
	void* h43 = s_dlopen("libspeexresampler.so",0);
	void* h44 = s_dlopen("libusbhost.so",0);
	void* h45 = s_dlopen("libharfbuzz_ng.so",0);
	void* h46 = s_dlopen("libhwui.so",0);
	void* h47 = s_dlopen("libRS.so",0);
	void* h48 = s_dlopen("libbcc.so",0);
	void* h49 = s_dlopen("libbcinfo.so",0);
	void* h50 = s_dlopen("libLLVM.so",0);
	void* h51 = s_dlopen("libRScpp.so",0);
    void* h52 = s_dlopen("libdvm.so",0);*/

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
    if(JNIEnv)
        return 1;

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
    JVM = (unsigned int)jvm;

    return 1;
}

int emulator::init_ret_stub()
{
    unsigned int svc_thumb = 0xdf00df00;

    uc_err  err=uc_mem_map(uc,EMULATOR_PAUSE_ADDRESS,0x1000,UC_PROT_ALL);

    err = uc_mem_write(uc,(uint64_t)EMULATOR_PAUSE_ADDRESS,&svc_thumb,4);

    return 1;
}

int emulator::save_signal_handler(int sig,void* handler)
{
    signal_map.insert(std::make_pair(sig,handler));
    return 1;
}

int emulator::process_signal(int sig)
{
    std::map<unsigned int,void*>::iterator iter = signal_map.begin();

    while (iter != signal_map.end())
    {
        void* addr = iter->second;
        if(iter->first == sig)
        {
            v_lr = (unsigned int)addr;
			uc_reg_write(uc,UC_ARM_REG_LR,&v_lr);
            update_cpu_model();
        }
        ++iter;
    }
    return 1;
}

int emulator::set_breakpoint(int addr)
{
	uc_err err;
	unsigned int arm_bkpt=0xe1200070;
	unsigned short thumb_bkpt=0xbe00;
	unsigned int insns = 0;

	if(addr & 1)
	{
		err = uc_mem_read(uc,addr-1,&insns,2);
		err = uc_mem_write(uc,addr-1,&thumb_bkpt,2);
	}
	else
	{
		err = uc_mem_read(uc,addr,&insns,4);
		err = uc_mem_write(uc,addr,&arm_bkpt,4);
	}

	bp_list.insert(std::make_pair(addr,insns));
	
	return err == UC_ERR_OK;
}

int emulator::process_breakpoint()
{
    uc_err err = UC_ERR_OK;
    unsigned int addr = 0;
    unsigned int insns = 0;
    unsigned int cpsr = 0;
    std::map<unsigned int,unsigned int>::iterator iter;

    iter = get_emulator()->bp_list.find(v_pc);
    if (iter == get_emulator()->bp_list.end())
    {
        iter = get_emulator()->bp_list.find(v_pc+1);
        if (iter != get_emulator()->bp_list.end())
        {
            addr = iter->first;
            insns = iter->second;
            err = uc_mem_write(uc,addr,&insns,2);
			err = uc_reg_read(uc,UC_ARM_REG_CPSR,&v_cpsr);
			v_cpsr |= 0x20;
			err = uc_reg_write(uc,UC_ARM_REG_CPSR,&v_cpsr);
            err = uc_reg_write(uc,UC_ARM_REG_PC,&addr);
        }
    }
    else
    {
        addr = iter->first;
        insns = iter->second;
        err = uc_mem_write(uc,addr,&insns,4);
        err = uc_reg_write(uc,UC_ARM_REG_PC,&v_pc);
    }

    return err == UC_ERR_OK;
}

int emulator::save_cpu_status()
{
    //uc_context_save(uc,context);
    return 1;
}

int emulator::restore_cpu_status()
{
    //uc_context_restore(uc,context);
    return 1;
}