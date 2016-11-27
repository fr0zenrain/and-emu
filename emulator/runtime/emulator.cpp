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

symbols_map* g_symbol_map = 0;
std::map<int,void*> g_svc_map;

extern uc_engine* g_uc;
extern int g_sym_cnt;
extern func_info g_invoke_func[];
extern func_info g_native_func[];
extern symbols g_syms[];

uc_engine* emulator::uc = 0;
soinfo* emulator::helper_info = 0;

unsigned int emulator::v_eip =0;
unsigned int emulator::v_eax =0;
unsigned int emulator::v_ecx =0;
unsigned int emulator::v_ebx =0;
unsigned int emulator::v_edx =0;
unsigned int emulator::v_esi =0;
unsigned int emulator::v_edi =0;
unsigned int emulator::v_esp =0;
unsigned int emulator::v_ebp =0;
unsigned int emulator::v_eflags =0;

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
}

emulator::emulator(uc_mode mode)
{
    trace_code = 0;
    trace_inter = 0;
	trace_unmap = 0;
    unsigned int eflags=0x246;

    uint64_t addr = (uint64_t)FUNCTION_VIRTUAL_ADDRESS;
    uc_err  err = uc_open(UC_ARCH_X86, mode, &uc);
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


    err=uc_reg_write(uc, UC_X86_REG_EFLAGS, &eflags);
    //set return address
    //uintptr_t lr = EMULATOR_PAUSE_ADDRESS;
    //lr |= 1;
    //err=uc_reg_write(uc, UC_X86_REG_ESP, &lr);
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
            err=uc_mem_write(uc,g_syms[i].vaddr,&svc_thumb, 4);
        }
        else
        {
            err=uc_mem_write(uc,g_syms[i].vaddr,&svc_arm, 4);
        }

        g_syms[i].vaddr |= g_syms[i].model;
        g_symbol_map[i].vaddr |= g_syms[i].model;
        g_svc_map.insert(std::make_pair(g_syms[i].number,g_syms[i].func));
    }

    qsort(g_symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);

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
	
	if(crc32 == 0xffa1e6f0 || crc32 ==0xbd2f3f6d || crc32 ==0x23398d9a)//special func
	{
		unsigned int sym_addr = get_helper_symbols(name);
		if(sym_addr)
		{
			sym.st_size = len;
			sym.st_name =1;
			sym.st_value = sym_addr;
		}
	}
	else
	{

		symbols* s = (symbols*)bsearch(&crc32,g_syms,g_sym_cnt,sizeof(symbols),hash_compare);
		if(s)
		{
			sym.st_size = len;
			sym.st_name =1;
			sym.st_value = s->vaddr;
		}
	}

    return &sym;
}

int emulator::dispatch(void* user_data)
{
    unsigned int addr = 0;

    uc_err err=uc_reg_read(uc, UC_X86_REG_EIP, &v_eip);
    err=uc_reg_read(uc, UC_X86_REG_EFLAGS, &v_eflags);
    err=uc_reg_read(uc, UC_X86_REG_ESP, &v_esp);
    err=uc_reg_read(uc, UC_X86_REG_EBP, &v_ebp);
    err=uc_reg_read(uc, UC_X86_REG_EAX, &v_eax);
    err=uc_reg_read(uc, UC_X86_REG_ECX, &v_ecx);
    err=uc_reg_read(uc, UC_X86_REG_EBX, &v_ebx);
    err=uc_reg_read(uc, UC_X86_REG_EDX, &v_edx);
    err=uc_reg_read(uc, UC_X86_REG_ESI, &v_esi);
    err=uc_reg_read(uc, UC_X86_REG_EDI, &v_esi);

    printf("eip %x eax %x ecx %x edx %x ebx %x esp %x ebp %x esi %x edi %x eflags %x\n",
           v_eip,v_eax,v_ecx,v_edx,v_ebx,v_esp,v_ebp,v_esi,v_edi,v_eflags);

    if(v_eip == 0x4004ac3b || v_eip == 0x4004b1c5)
    {
        unsigned int value = 0x40046000+0x14;
        uc_reg_write(uc,UC_X86_REG_EAX,&value);
        v_eip += 7;
        uc_reg_write(uc,UC_X86_REG_EIP,&v_eip);
        v_eflags = 0x246;
        uc_reg_write(uc,UC_X86_REG_EFLAGS,&v_eflags);
    }

    if((v_eip & 0xf0000000) == FUNCTION_VIRTUAL_ADDRESS)
    {
		addr = v_eip;
        symbols_map*  s = (symbols_map*)bsearch(&addr,g_symbol_map,g_sym_cnt,sizeof(symbols_map),hash_compare);
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
    else if((v_eip & 0xffffff00) == JVM_INVOKE_ADDRESS)
    {
        //restore r7
        int index = v_eax;
        /*  err=uc_mem_read(uc, v_sp, &v_r7,4);
          err=uc_reg_write(uc, UC_ARM_REG_R7, &v_r7);
          v_esp += 4;
          err=uc_reg_write(uc, UC_ARM_REG_SP, &v_sp);*/
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
    else if((v_eip & 0xfffff000) == JVM_INTERFACE_ADDRESS)
    {
        //restore r7
        int index = v_eax;
        /* err=uc_mem_read(uc, v_sp, &v_r7,4);
         err=uc_reg_write(uc, UC_ARM_REG_R7, &v_r7);
         v_sp += 4;
         err=uc_reg_write(uc, UC_ARM_REG_SP, &v_sp);*/
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
    else if((v_eip & 0xffffff00) == EMULATOR_PAUSE_ADDRESS)
    {
        //printf("emulator pause\n");
    }
    else
    {
        std::map<int,void*>::iterator iter = g_svc_map.find(v_eax);
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
    unsigned char buf[16] = {0};
    soinfo* si = (soinfo*)user_data;
    if (uc_mem_read(uc, address, buf, size) != UC_ERR_OK) {
        printf("not ok - uc_mem_read fail during hook_code callback, addr: 0x%llx\n", address);
        if (uc_emu_stop(uc) != UC_ERR_OK) {
            printf("not ok - uc_emu_stop fail during hook_code callback, addr: 0x%llx\n", address);
            _exit(-1);
        }
    }

    unsigned int eax,ecx,edx,ebx,ebp,esp,esi,edi,eip,eflags;
    uc_reg_read(uc, UC_X86_REG_EFLAGS, &eflags);
    uc_reg_read(uc, UC_X86_REG_EIP, &eip);
    uc_reg_read(uc, UC_X86_REG_EAX, &eax);
    uc_reg_read(uc, UC_X86_REG_ECX, &ecx);
    uc_reg_read(uc, UC_X86_REG_EDX, &edx);
    uc_reg_read(uc, UC_X86_REG_EBX, &ebx);
    uc_reg_read(uc, UC_X86_REG_ESP, &esp);
    uc_reg_read(uc, UC_X86_REG_EBP, &ebp);
    uc_reg_read(uc, UC_X86_REG_ESI, &esi);
    uc_reg_read(uc, UC_X86_REG_EDI, &edi);

    //printf("eip %x eax %x ecx %x edx %x ebx %x esp %x ebp %x esi %x edi %x eflags %x\n",eip,eax,ecx,edx,ebx,esp,ebp,esi,edi,eflags);
    char stack[32]={0};
    for(int i =0; i< 32;i++)
    {
        uc_mem_read(uc,esp+i,&stack[i],1);
    }

    //print_hex_dump_bytes(stack,32);

    if(*(unsigned int*)buf == 0)
    {
        uc_emu_stop(uc);
        return ;
    }
    unsigned int offset = address - si->base;
    csh handle;
    cs_insn *insn;
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
    if(err == CS_ERR_OK)
    {
        cs_option(handle, CS_OPT_SYNTAX, 0);
        cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

        int count = cs_disasm(handle,(unsigned char*) buf, size, address, 0, &insn);
        if(count)
        {
            printf(RED "%08x[0x%04x]:\t%x\t%s\t%s\n" RESET, (int)address,offset,*(unsigned int*)buf, insn->mnemonic, insn->op_str);
        }
    }
}

void emulator::hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    dispatch(user_data);
}

void emulator::hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    unsigned int eax,ecx,edx,ebx,ebp,esp,esi,edi,eip,eflags;
    uc_err err=uc_reg_read(uc, UC_X86_REG_EFLAGS, &eflags);
    err=uc_reg_read(uc, UC_X86_REG_EIP, &eip);
    err=uc_reg_read(uc, UC_X86_REG_EAX, &eax);
    err=uc_reg_read(uc, UC_X86_REG_ECX, &ecx);
    err=uc_reg_read(uc, UC_X86_REG_EDX, &edx);
    err=uc_reg_read(uc, UC_X86_REG_EBX, &ebx);
    err=uc_reg_read(uc, UC_X86_REG_ESP, &esp);
    err=uc_reg_read(uc, UC_X86_REG_EBP, &ebp);
    err=uc_reg_read(uc, UC_X86_REG_ESI, &esi);
    err=uc_reg_read(uc, UC_X86_REG_EDI, &edi);

    printf(">>> Tracing unmap at 0x%llx, block size = 0x%x\n", address, size);
    printf("eip %x eax %x ecx %x edx %x ebx %x esp %x ebp %x esi %x edi %x eflags %x\n",eip,eax,ecx,edx,ebx,esp,ebp,esi,edi,eflags);
}


void emulator::start_emulator(unsigned int pc, soinfo * si)
{
    uc_err err;
    err=uc_reg_write(uc, UC_X86_REG_EIP, &pc);

    if(trace_code)
    {
        uc_hook_del(uc,trace_code);
        trace_code = 0;
    }
    else
    {
#ifdef _MSC_VER
        err=uc_hook_add(uc, &trace_code, UC_HOOK_CODE, (void*)hook_code, (void*)si, 1,1);
#else
        err = uc_hook_add(uc, &trace_code, UC_HOOK_CODE, (void *) hook_code, (void *) si, 1, 0);
#endif
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
    uc_err err = uc_mem_map(uc,stack_base,sp - stack_base+0x100000,UC_PROT_READ|UC_PROT_WRITE);
    if(err != UC_ERR_OK)
    {
        printf("Failed on uc_mem_map() with error returned: %u\n", err);
        return 0;
    }

    err=uc_reg_write(uc, UC_X86_REG_ESP, &sp);
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
    //uc_reg_write(uc, UC_ARM_REG_R0, &jvm);
    //uc_reg_write(uc, UC_ARM_REG_R1, &env);
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
            v_eip = (unsigned int)addr;
            update_cpu_model();
        }
        ++iter;
    }
    return 1;
}