// bang_unpack.cpp : Defines the entry point for the console application.
//

#include "../include/unicorn/unicorn.h"
#include "../capstone/include/capstone.h"
#include "dlfcn.h"
#include "linker.h"

#pragma comment(lib,"unicorn_staload.lib")
//#pragma comment(lib,"capstone.lib")
uc_engine* g_uc;

static void hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing unmap at 0x%llx, block size = 0x%x\n", address, size);
}


static void hook_block(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing basic block at 0x%llx, block size = 0x%x\n", address, size);
}

static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
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

	csh handle;
	cs_insn *insn;
	cs_err err = cs_open(CS_ARCH_ARM, CS_MODE_THUMB, &handle);
	if(err == CS_ERR_OK)
	{
		cs_option(handle, CS_OPT_SYNTAX, 0);
		cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

		int count = cs_disasm(handle,(unsigned char*) buf, 4, address, 0, &insn);
		if(count)
			printf("0x%llx[%08x]:\t%s\t%s\n", insn->address,(int)insn->address-si->base, insn->mnemonic, insn->op_str);
		cs_close(&handle);

	}
}


static void hook_instr(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing instr at 0x%llx, block size = 0x%x\n", address, size);
}

static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing inter at 0x%llx, instruction size = 0x%x\n", address, size);
}

static void hook_mem_access(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing mem at 0x%llx, instruction size = 0x%x\n", address, size);
}



soinfo* load_android_so(const char* path)
{
	soinfo* handle = (soinfo*)s_dlopen(path,0);

	return handle;
}

int start_vm(uc_engine* uc,soinfo* si,void* JNI_OnLoad)
{
	uc_hook trace1, trace2;
	uc_hook trace3, trace4;
	uc_hook trace5,trace6;

	uintptr_t lr = 0xabcdedab;  
	uintptr_t pc = (uintptr_t)JNI_OnLoad;     // R2 register
	int cpsr=0x800d0030;

	uc_err err;
	//uc_err  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
	uint64_t sp = 0xbef00000;
	err=uc_mem_map(uc,sp-0x200000,4*1024*1024,UC_PROT_READ|UC_PROT_WRITE);
	//uc_mem_write(uc,sp+0x100000,);
	int r0=0;
	int r1=0;
	int r2=0;
	int r3=0;
	int r4=0;
	int r5=0;
	int r6=0;

	err=uc_reg_write(uc, UC_ARM_REG_R0, &r0);
	err=uc_reg_write(uc, UC_ARM_REG_R1, &r1);
	err=uc_reg_write(uc, UC_ARM_REG_R2, &r2);
	err=uc_reg_write(uc, UC_ARM_REG_R3, &r3);
	err=uc_reg_write(uc, UC_ARM_REG_R4, &r4);
	err=uc_reg_write(uc, UC_ARM_REG_R5, &r5);

	err=uc_reg_write(uc, UC_ARM_REG_SP, &sp);
	err=uc_reg_write(uc, UC_ARM_REG_PC, &pc);
	err=uc_reg_write(uc, UC_ARM_REG_CPSR, &cpsr);
	err=uc_reg_write(uc, UC_ARM_REG_LR, &pc);

	// tracing all basic blocks with customized callback
	err=uc_hook_add(uc, &trace1, UC_HOOK_BLOCK, (void*)hook_block, NULL, JNI_OnLoad, JNI_OnLoad);

	// tracing one instruction at ADDRESS with customized callback
	err=uc_hook_add(uc, &trace2, UC_HOOK_CODE, (void*)hook_code, si, 1, 1);

	err=uc_hook_add(uc, &trace3, UC_HOOK_INSN, (void*)hook_instr, NULL, JNI_OnLoad, JNI_OnLoad);

	err=uc_hook_add(uc, &trace4, UC_HOOK_INTR, (void*)hook_inter, NULL, JNI_OnLoad, JNI_OnLoad);

	err=uc_hook_add(uc, &trace5, UC_HOOK_MEM_UNMAPPED|UC_HOOK_MEM_INVALID|UC_HOOK_MEM_FETCH_INVALID, (void*)hook_unmap, NULL, JNI_OnLoad, 0);

	err=uc_hook_add(uc, &trace6, UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, (void*)hook_mem_access, NULL, JNI_OnLoad, 0);

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
		uc_err err = uc_emu_start(uc, (uint64_t)JNI_OnLoad, (uint64_t)JNI_OnLoad+10000, 0, 0);
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
	uc_engine* uc;
	uc_err  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
	g_uc = uc;

	void* h = s_dlopen("libc.so",0);
	void* h1 = s_dlopen("libm.so",0);
	void* h2 = s_dlopen("libstdc++.so",0);
	void* h3 = s_dlopen("liblog.so",0);
	void* h4 = s_dlopen("libz.so",0);
	soinfo* si = load_android_so("libsgmainso-6.0.71.so");
	void* JNI_OnLoad = s_dlsym(si,"JNI_OnLoad");

	//start_vm(g_uc,si,(void*)((int)si->base+0x772c));
	start_vm(g_uc,si,(void*)((int)JNI_OnLoad-1));

	return 0;
}

