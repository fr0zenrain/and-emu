// bang_unpack.cpp : Defines the entry point for the console application.
//

#include "../include/unicorn/unicorn.h"
#include "dlfcn.h"
#include "linker.h"


#pragma comment(lib,"unicorn_staload.lib")


static void hook_block(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing basic block at 0x%llx, block size = 0x%x\n", address, size);
}

static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	int x = 0;
	printf(">>> Tracing instruction at 0x%llx, instruction size = 0x%x\n", address, size);
}


static void hook_instr(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing basic block at 0x%llx, block size = 0x%x\n", address, size);
}

static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing instruction at 0x%llx, instruction size = 0x%x\n", address, size);
}


soinfo* load_android_so(const char* path)
{
	soinfo* handle = (soinfo*)s_dlopen(path,0);

	return handle;
}

unsigned char g_sp[1024*1024];

int start_vm(soinfo* si,void* JNI_OnLoad)
{
	uc_hook trace1, trace2;
	uc_hook trace3, trace4;

	uintptr_t sp = (uintptr_t)g_sp;  
	uintptr_t lr = 0xabcdedab;  
	uintptr_t pc = (uintptr_t)JNI_OnLoad;     // R2 register
	int cpsr=0x600f0010;

	uc_engine* uc;
	uc_err  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);

	if(err != UC_ERR_OK)
	{
		return 0;
	}

	if (err) {
		printf("Failed on uc_open() with error returned: %u (%s)\n",
			err, uc_strerror(err));
		return 0;
	}

	// map 2MB memory for this emulation
	err=uc_mem_map(uc, si->base, 64 * 1024 * 1024, UC_PROT_ALL);

	// write machine code to be emulated to memory
	err=uc_mem_write(uc, si->base, (void*)si->base, si->size);

	err=uc_reg_write(uc, UC_ARM_REG_SP, &sp);
	err=uc_reg_write(uc, UC_ARM_REG_PC, &pc);
	err=uc_reg_write(uc, UC_ARM_REG_CPSR, &cpsr);
	err=uc_reg_write(uc, UC_ARM_REG_LR, &pc);

	// tracing all basic blocks with customized callback
	err=uc_hook_add(uc, &trace1, UC_HOOK_BLOCK, (void*)hook_block, NULL, 1, 0);

	// tracing one instruction at ADDRESS with customized callback
	err=uc_hook_add(uc, &trace2, UC_HOOK_CODE, (void*)hook_code, NULL, si->base, si->base);

	err=uc_hook_add(uc, &trace3, UC_HOOK_INSN, (void*)hook_instr, NULL, si->base, si->base);

	err=uc_hook_add(uc, &trace4, UC_HOOK_INTR, (void*)hook_inter, NULL, si->base, si->base);

	if(si->init_func)
	{
		uc_err err = uc_emu_start(uc, (uint64_t)si->init_func, (uint64_t)si->init_func+10, 0, 0);
		if (err) {
			printf("Failed on uc_emu_start() with error returned: %u\n", err);
			return 0;
		}
	}

	if(JNI_OnLoad)
	{
		uc_err err = uc_emu_start(uc, (uint64_t)JNI_OnLoad, (uint64_t)JNI_OnLoad+20, 0, 0);
		if (err) {
			printf("Failed on uc_emu_start() with error returned: %u\n", err);
			return 0;
		}
	}
	uc_close(uc);
	return 1;
}


int main(int argc, char* argv[])
{
	void* h = s_dlopen("libc.so",0);
	void* h2 = s_dlopen("libm.so",0);
	void* h3 = s_dlopen("liblog.so",0);
	void* h4 = s_dlopen("libz.so",0);
	void* h1 = s_dlopen("libstdc++.so",0);
	soinfo* si = load_android_so("libsgmainso-6.0.71.so");
	void* JNI_OnLoad = s_dlsym(si,"JNI_OnLoad");

	start_vm(si,(void*)((int)JNI_OnLoad));
	

	return 0;
}

