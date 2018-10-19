// bang_unpack.cpp : Defines the entry point for the console application.
//

#include "../include/unicorn/unicorn.h"
#include "../capstone/include/capstone.h"
#include "string.h"
#include "dlfcn.h"
#include "linker.h"
#include "engine.h"
#include "jvm/jvm.h"
#include "runtime/emulator.h"
#include "runtime/runtime.h"
#include "ctype.h"
#ifndef _WIN32
#include <sys/mman.h>
#endif
//#include "vld.h"
#pragma comment(lib,"unicorn_staload.lib")
//#pragma comment(lib,"capstone.lib")
uc_engine* g_uc;

int g_armmode = 0;
int g_emu_init = 0;
int g_show_ins = 0;

static void hook_unmap(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	int r0,r1,r2,r3,r4,r5,r6,r7,pc,lr,sp;
	uc_err err=uc_reg_read(g_uc, UC_ARM_REG_PC, &pc);
	err=uc_reg_read(g_uc, UC_ARM_REG_LR, &lr);
	err=uc_reg_read(g_uc, UC_ARM_REG_SP, &sp);
	err=uc_reg_read(g_uc, UC_ARM_REG_R0, &r0);
	err=uc_reg_read(g_uc, UC_ARM_REG_R1, &r1);
	err=uc_reg_read(g_uc, UC_ARM_REG_R2, &r2);
	err=uc_reg_read(g_uc, UC_ARM_REG_R3, &r3);
	err=uc_reg_read(g_uc, UC_ARM_REG_R4, &r4);
	err=uc_reg_read(g_uc, UC_ARM_REG_R5, &r5);
	err=uc_reg_read(g_uc, UC_ARM_REG_R6, &r6);
	err=uc_reg_read(g_uc, UC_ARM_REG_R7, &r7);
	printf(">>> Tracing unmap at 0x%llx, block size = 0x%x\n", address, size);
	printf("pc %x lr %x sp %x r0 %x r1 %x r2 %x r3 %x r4 %x r5 %x r6 %x r7 %x\n",pc,lr,sp,r0,r1,r2,r3,r4,r5,r6,r7);
}


static void hook_block(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	//printf(">>> Tracing basic block at 0x%llx, block size = 0x%x\n", address, size);
}

unsigned char jmp_tbl[] =
{
	0x33, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x74, 0x02, 
	0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xA8, 0x02, 0x00, 0x00, 
	0xB8, 0x02, 0x00, 0x00, 0xD4, 0x02, 0x00, 0x00, 0xF0, 0x02, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x10, 0x03, 0x00, 0x00, 
	0x28, 0x03, 0x00, 0x00, 0x40, 0x03, 0x00, 0x00, 0x68, 0x03, 
	0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x98, 0x03, 0x00, 0x00, 
	0xBC, 0x03, 0x00, 0x00, 0xD8, 0x03, 0x00, 0x00, 0xE8, 0x03, 
	0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 
	0x30, 0x04, 0x00, 0x00, 0x44, 0x04, 0x00, 0x00, 0x54, 0x04, 
	0x00, 0x00, 0x64, 0x04, 0x00, 0x00, 0x74, 0x04, 0x00, 0x00, 
	0x84, 0x04, 0x00, 0x00, 0xAC, 0x04, 0x00, 0x00, 0xBC, 0x04, 
	0x00, 0x00, 0xD4, 0x04, 0x00, 0x00, 0xFC, 0x04, 0x00, 0x00, 
	0x1C, 0x05, 0x00, 0x00, 0x44, 0x05, 0x00, 0x00, 0x54, 0x05, 
	0x00, 0x00, 0x70, 0x05, 0x00, 0x00, 0x88, 0x05, 0x00, 0x00, 
	0x98, 0x05, 0x00, 0x00, 0xA8, 0x05, 0x00, 0x00, 0xC4, 0x05, 
	0x00, 0x00, 0xE8, 0x05, 0x00, 0x00, 0xF8, 0x05, 0x00, 0x00, 
	0x14, 0x06, 0x00, 0x00, 0x34, 0x06, 0x00, 0x00, 0x48, 0x06, 
	0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x78, 0x06, 0x00, 0x00, 
	0x94, 0x06, 0x00, 0x00, 0xA4, 0x06, 0x00, 0x00, 0xB4, 0x06, 
	0x00, 0x00, 0xD0, 0x06, 0x00, 0x00, 0xE0, 0x06, 0x00, 0x00, 
	0xF8, 0x06, 0x00, 0x00, 0x14, 0x07, 0x00, 0x00, 0x54, 0x07, 
	0x00, 0x00, 0x68, 0x07, 0x00, 0x00, 0x7C, 0x07, 0x00, 0x00, 
	0x90, 0x07, 0x00, 0x00, 0xB8, 0x07, 0x00, 0x00, 0xE0, 0x07, 
	0x00, 0x00, 0x18, 0x08, 0x00, 0x00, 0x50, 0x08, 0x00, 0x00, 
	0x94, 0x08, 0x00, 0x00, 0xCC, 0x08, 0x00, 0x00, 0xF4, 0x08, 
	0x00, 0x00, 0x34, 0x09, 0x00, 0x00, 0x74, 0x09, 0x00, 0x00, 
	0xA0, 0x09, 0x00, 0x00, 0xD0, 0x09, 0x00, 0x00, 0xFC, 0x09, 
	0x00, 0x00, 0x3C, 0x0A, 0x00, 0x00, 0x64, 0x0A, 0x00, 0x00, 
	0x7C, 0x0A, 0x00, 0x00, 0x94, 0x0A, 0x00, 0x00, 0xB4, 0x0A, 
	0x00, 0x00, 0xE4, 0x0A, 0x00, 0x00, 0x14, 0x0B, 0x00, 0x00, 
	0x3C, 0x0B, 0x00, 0x00, 0x5C, 0x0B, 0x00, 0x00, 0x88, 0x0B, 
	0x00, 0x00, 0xC0, 0x0B, 0x00, 0x00, 0xF0, 0x0B, 0x00, 0x00, 
	0x18, 0x0C, 0x00, 0x00, 0x50, 0x0C, 0x00, 0x00, 0x7C, 0x0C, 
	0x00, 0x00, 0xA8, 0x0C, 0x00, 0x00, 0xBC, 0x0C, 0x00, 0x00, 
	0xE4, 0x0C, 0x00, 0x00, 0x08, 0x0D, 0x00, 0x00, 0x44, 0x0D, 
	0x00, 0x00, 0x70, 0x0D, 0x00, 0x00, 0x9C, 0x0D, 0x00, 0x00, 
	0xC0, 0x0D, 0x00, 0x00, 0xE0, 0x0D, 0x00, 0x00, 0x08, 0x0E, 
	0x00, 0x00, 0x30, 0x0E, 0x00, 0x00, 0x50, 0x0E, 0x00, 0x00, 
	0x6C, 0x0E, 0x00, 0x00, 0x98, 0x0E, 0x00, 0x00, 0xC0, 0x0E, 
	0x00, 0x00, 0xD4, 0x0E, 0x00, 0x00, 0xE8, 0x0E, 0x00, 0x00, 
	0x08, 0x0F, 0x00, 0x00, 0x20, 0x0F, 0x00, 0x00, 0x38, 0x0F, 
	0x00, 0x00, 0x5C, 0x0F, 0x00, 0x00, 0x98, 0x0F, 0x00, 0x00, 
	0xB8, 0x0F, 0x00, 0x00, 0xEC, 0x0F, 0x00, 0x00, 0x14, 0x10, 
	0x00, 0x00, 0x54, 0x10, 0x00, 0x00, 0x74, 0x10, 0x00, 0x00, 
	0xA0, 0x10, 0x00, 0x00, 0xC8, 0x10, 0x00, 0x00, 0x0C, 0x11, 
	0x00, 0x00, 0x34, 0x11, 0x00, 0x00, 0x54, 0x11, 0x00, 0x00, 
	0x68, 0x11, 0x00, 0x00, 0xAC, 0x11, 0x00, 0x00, 0xD4, 0x11, 
	0x00, 0x00, 0xE8, 0x11, 0x00, 0x00, 0x10, 0x12, 0x00, 0x00, 
	0x38, 0x12, 0x00, 0x00, 0x4C, 0x12, 0x00, 0x00, 0x74, 0x12, 
	0x00, 0x00, 0xA4, 0x12, 0x00, 0x00, 0xB8, 0x12, 0x00, 0x00, 
	0xD0, 0x12, 0x00, 0x00, 0x14, 0x13, 0x00, 0x00, 0x38, 0x13, 
	0x00, 0x00, 0x78, 0x13, 0x00, 0x00, 0xB0, 0x13, 0x00, 0x00, 
	0xDC, 0x13, 0x00, 0x00, 0x04, 0x14, 0x00, 0x00, 0x44, 0x14, 
	0x00, 0x00, 0x84, 0x14, 0x00, 0x00, 0xAC, 0x14, 0x00, 0x00, 
	0xD4, 0x14, 0x00, 0x00, 0xFC, 0x14, 0x00, 0x00, 0x3C, 0x15, 
	0x00, 0x00, 0x6C, 0x15, 0x00, 0x00, 0xA4, 0x15, 0x00, 0x00, 
	0xC4, 0x15, 0x00, 0x00, 0xD8, 0x15, 0x00, 0x00, 0x04, 0x16, 
	0x00, 0x00, 0x40, 0x16, 0x00, 0x00, 0x68, 0x16, 0x00, 0x00, 
	0xAC, 0x16, 0x00, 0x00, 0xD8, 0x16, 0x00, 0x00, 0x1C, 0x17, 
	0x00, 0x00, 0x40, 0x17, 0x00, 0x00, 0x60, 0x17, 0x00, 0x00
};

unsigned char jmp_tbl2[] =
{
	0x68, 0x00, 0x00, 0x00, 0xF4, 0x04, 0x00, 0x00, 0x0C, 0x05, 
	0x00, 0x00, 0x1C, 0x05, 0x00, 0x00, 0x38, 0x05, 0x00, 0x00, 
	0x4C, 0x05, 0x00, 0x00, 0x64, 0x05, 0x00, 0x00, 0x74, 0x05, 
	0x00, 0x00, 0x84, 0x05, 0x00, 0x00, 0xA0, 0x05, 0x00, 0x00, 
	0xB8, 0x05, 0x00, 0x00, 0xC8, 0x05, 0x00, 0x00, 0xF4, 0x05, 
	0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x1C, 0x06, 0x00, 0x00, 
	0x2C, 0x06, 0x00, 0x00, 0x50, 0x06, 0x00, 0x00, 0x6C, 0x06, 
	0x00, 0x00, 0x84, 0x06, 0x00, 0x00, 0x94, 0x06, 0x00, 0x00, 
	0xA4, 0x06, 0x00, 0x00, 0xBC, 0x06, 0x00, 0x00, 0xCC, 0x06, 
	0x00, 0x00, 0xE8, 0x06, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 
	0x14, 0x07, 0x00, 0x00, 0x2C, 0x07, 0x00, 0x00, 0x48, 0x07, 
	0x00, 0x00, 0x64, 0x07, 0x00, 0x00, 0x88, 0x07, 0x00, 0x00, 
	0x98, 0x07, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0xD0, 0x07, 
	0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 
	0x24, 0x08, 0x00, 0x00, 0x48, 0x08, 0x00, 0x00, 0x70, 0x08, 
	0x00, 0x00, 0x80, 0x08, 0x00, 0x00, 0x90, 0x08, 0x00, 0x00, 
	0xA0, 0x08, 0x00, 0x00, 0xBC, 0x08, 0x00, 0x00, 0xD8, 0x08, 
	0x00, 0x00, 0xE8, 0x08, 0x00, 0x00, 0xF8, 0x08, 0x00, 0x00, 
	0x08, 0x09, 0x00, 0x00, 0x18, 0x09, 0x00, 0x00, 0x28, 0x09, 
	0x00, 0x00, 0x44, 0x09, 0x00, 0x00, 0x54, 0x09, 0x00, 0x00, 
	0x64, 0x09, 0x00, 0x00, 0x7C, 0x09, 0x00, 0x00, 0x98, 0x09, 
	0x00, 0x00, 0xB0, 0x09, 0x00, 0x00, 0xCC, 0x09, 0x00, 0x00, 
	0xE4, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x18, 0x0A, 
	0x00, 0x00, 0x30, 0x0A, 0x00, 0x00, 0x40, 0x0A, 0x00, 0x00, 
	0x50, 0x0A, 0x00, 0x00, 0x60, 0x0A, 0x00, 0x00, 0x70, 0x0A, 
	0x00, 0x00, 0x94, 0x0A, 0x00, 0x00, 0xBC, 0x0A, 0x00, 0x00, 
	0xCC, 0x0A, 0x00, 0x00, 0xE8, 0x0A, 0x00, 0x00, 0xF8, 0x0A, 
	0x00, 0x00, 0x08, 0x0B, 0x00, 0x00, 0x18, 0x0B, 0x00, 0x00, 
	0x30, 0x0B, 0x00, 0x00, 0x48, 0x0B, 0x00, 0x00, 0x58, 0x0B, 
	0x00, 0x00, 0x68, 0x0B, 0x00, 0x00, 0x78, 0x0B, 0x00, 0x00, 
	0x88, 0x0B, 0x00, 0x00, 0xA4, 0x0B, 0x00, 0x00, 0xB4, 0x0B, 
	0x00, 0x00, 0xCC, 0x0B, 0x00, 0x00, 0xE4, 0x0B, 0x00, 0x00, 
	0xF4, 0x0B, 0x00, 0x00, 0x04, 0x0C, 0x00, 0x00, 0x2C, 0x0C, 
	0x00, 0x00, 0x48, 0x0C, 0x00, 0x00, 0x68, 0x0C, 0x00, 0x00, 
	0x84, 0x0C, 0x00, 0x00, 0xA0, 0x0C, 0x00, 0x00, 0xB0, 0x0C, 
	0x00, 0x00, 0xC0, 0x0C, 0x00, 0x00, 0xDC, 0x0C, 0x00, 0x00, 
	0x00, 0x0D, 0x00, 0x00, 0x10, 0x0D, 0x00, 0x00, 0x2C, 0x0D, 
	0x00, 0x00, 0x48, 0x0D, 0x00, 0x00, 0x60, 0x0D, 0x00, 0x00, 
	0x74, 0x0D, 0x00, 0x00, 0x90, 0x0D, 0x00, 0x00, 0xA0, 0x0D, 
	0x00, 0x00, 0xB0, 0x0D, 0x00, 0x00, 0xCC, 0x0D, 0x00, 0x00, 
	0xE0, 0x0D, 0x00, 0x00, 0xF4, 0x0D, 0x00, 0x00, 0x28, 0x0E, 
	0x00, 0x00, 0x44, 0x0E, 0x00, 0x00, 0x54, 0x0E, 0x00, 0x00, 
	0x68, 0x0E, 0x00, 0x00, 0x7C, 0x0E, 0x00, 0x00, 0xB0, 0x0E, 
	0x00, 0x00, 0xE4, 0x0E, 0x00, 0x00, 0x0C, 0x0F, 0x00, 0x00, 
	0x38, 0x0F, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0xB0, 0x0F, 
	0x00, 0x00, 0xE8, 0x0F, 0x00, 0x00, 0x2C, 0x10, 0x00, 0x00, 
	0x54, 0x10, 0x00, 0x00, 0x8C, 0x10, 0x00, 0x00, 0xC8, 0x10, 
	0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x28, 0x11, 0x00, 0x00, 
	0x60, 0x11, 0x00, 0x00, 0xA0, 0x11, 0x00, 0x00, 0xC8, 0x11, 
	0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x38, 0x12, 0x00, 0x00, 
	0x6C, 0x12, 0x00, 0x00, 0xA8, 0x12, 0x00, 0x00, 0xC0, 0x12, 
	0x00, 0x00, 0xE8, 0x12, 0x00, 0x00, 0x10, 0x13, 0x00, 0x00, 
	0x44, 0x13, 0x00, 0x00, 0x7C, 0x13, 0x00, 0x00, 0xA4, 0x13, 
	0x00, 0x00, 0xD8, 0x13, 0x00, 0x00, 0x18, 0x14, 0x00, 0x00, 
	0x40, 0x14, 0x00, 0x00, 0x78, 0x14, 0x00, 0x00, 0xA8, 0x14, 
	0x00, 0x00, 0xDC, 0x14, 0x00, 0x00, 0x04, 0x15, 0x00, 0x00, 
	0x44, 0x15, 0x00, 0x00, 0x80, 0x15, 0x00, 0x00, 0xAC, 0x15, 
	0x00, 0x00, 0xE0, 0x15, 0x00, 0x00, 0x24, 0x16, 0x00, 0x00, 
	0x4C, 0x16, 0x00, 0x00, 0x74, 0x16, 0x00, 0x00, 0xA0, 0x16, 
	0x00, 0x00, 0xB4, 0x16, 0x00, 0x00, 0xEC, 0x16, 0x00, 0x00, 
	0x18, 0x17, 0x00, 0x00, 0x44, 0x17, 0x00, 0x00, 0x6C, 0x17, 
	0x00, 0x00, 0x94, 0x17, 0x00, 0x00, 0xC0, 0x17, 0x00, 0x00, 
	0xEC, 0x17, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x74, 0x18, 
	0x00, 0x00, 0x94, 0x18, 0x00, 0x00, 0xD0, 0x18, 0x00, 0x00, 
	0xE4, 0x18, 0x00, 0x00, 0x28, 0x19, 0x00, 0x00, 0x58, 0x19, 
	0x00, 0x00, 0x78, 0x19, 0x00, 0x00, 0xA0, 0x19, 0x00, 0x00, 
	0xCC, 0x19, 0x00, 0x00, 0xFC, 0x19, 0x00, 0x00, 0x28, 0x1A, 
	0x00, 0x00, 0x50, 0x1A, 0x00, 0x00, 0x74, 0x1A, 0x00, 0x00, 
	0x9C, 0x1A, 0x00, 0x00, 0xB0, 0x1A, 0x00, 0x00, 0xD4, 0x1A, 
	0x00, 0x00, 0xFC, 0x1A, 0x00, 0x00, 0x24, 0x1B, 0x00, 0x00, 
	0x4C, 0x1B, 0x00, 0x00, 0x78, 0x1B, 0x00, 0x00, 0xA0, 0x1B, 
	0x00, 0x00, 0xB8, 0x1B, 0x00, 0x00, 0xF0, 0x1B, 0x00, 0x00, 
	0x28, 0x1C, 0x00, 0x00, 0x4C, 0x1C, 0x00, 0x00, 0x78, 0x1C, 
	0x00, 0x00, 0x8C, 0x1C, 0x00, 0x00, 0xB4, 0x1C, 0x00, 0x00, 
	0xEC, 0x1C, 0x00, 0x00, 0x34, 0x1D, 0x00, 0x00, 0x60, 0x1D, 
	0x00, 0x00, 0x88, 0x1D, 0x00, 0x00, 0xB0, 0x1D, 0x00, 0x00, 
	0xF0, 0x1D, 0x00, 0x00, 0x24, 0x1E, 0x00, 0x00, 0x5C, 0x1E, 
	0x00, 0x00, 0x70, 0x1E, 0x00, 0x00, 0x84, 0x1E, 0x00, 0x00, 
	0xA8, 0x1E, 0x00, 0x00, 0xE4, 0x1E, 0x00, 0x00, 0x0C, 0x1F, 
	0x00, 0x00, 0x68, 0x1F, 0x00, 0x00, 0x90, 0x1F, 0x00, 0x00, 
	0xD0, 0x1F, 0x00, 0x00, 0x0C, 0x20, 0x00, 0x00, 0x34, 0x20, 
	0x00, 0x00, 0x6C, 0x20, 0x00, 0x00, 0xA8, 0x20, 0x00, 0x00, 
	0xE0, 0x20, 0x00, 0x00, 0x08, 0x21, 0x00, 0x00, 0x40, 0x21, 
	0x00, 0x00, 0x9C, 0x22, 0x00, 0x00, 0xC4, 0x22, 0x00, 0x00, 
	0x0C, 0x23, 0x00, 0x00, 0x44, 0x23, 0x00, 0x00, 0x84, 0x23, 
	0x00, 0x00, 0xB8, 0x23, 0x00, 0x00, 0xEC, 0x23, 0x00, 0x00, 
	0x20, 0x24, 0x00, 0x00, 0x58, 0x24, 0x00, 0x00, 0x70, 0x24, 
	0x00, 0x00, 0x84, 0x24, 0x00, 0x00, 0x9C, 0x24, 0x00, 0x00, 
	0xB4, 0x24, 0x00, 0x00, 0xDC, 0x24, 0x00, 0x00, 0xFC, 0x24, 
	0x00, 0x00, 0x34, 0x25, 0x00, 0x00, 0x78, 0x25, 0x00, 0x00, 
	0x98, 0x25, 0x00, 0x00, 0xD4, 0x25, 0x00, 0x00, 0xF8, 0x25, 
	0x00, 0x00, 0x10, 0x26, 0x00, 0x00, 0x48, 0x26, 0x00, 0x00, 
	0x74, 0x26, 0x00, 0x00, 0x9C, 0x26, 0x00, 0x00, 0xDC, 0x26, 
	0x00, 0x00, 0x14, 0x27, 0x00, 0x00, 0x44, 0x27, 0x00, 0x00, 
	0x78, 0x27, 0x00, 0x00, 0x8C, 0x27, 0x00, 0x00, 0xB4, 0x27, 
	0x00, 0x00, 0xDC, 0x27, 0x00, 0x00, 0x14, 0x28, 0x00, 0x00, 
	0x38, 0x28, 0x00, 0x00, 0x60, 0x28, 0x00, 0x00, 0x98, 0x28, 
	0x00, 0x00, 0xD0, 0x28, 0x00, 0x00, 0x08, 0x29, 0x00, 0x00, 
	0x30, 0x29, 0x00, 0x00, 0x64, 0x29, 0x00, 0x00, 0x78, 0x29, 
	0x00, 0x00, 0xB0, 0x29, 0x00, 0x00, 0xDC, 0x29, 0x00, 0x00, 
	0x20, 0x2A, 0x00, 0x00, 0x58, 0x2A, 0x00, 0x00, 0x80, 0x2A, 
	0x00, 0x00, 0xC0, 0x2A, 0x00, 0x00, 0xE8, 0x2A, 0x00, 0x00, 
	0x00, 0x2B, 0x00, 0x00, 0x2C, 0x2B, 0x00, 0x00, 0x54, 0x2B, 
	0x00, 0x00, 0x6C, 0x2B, 0x00, 0x00, 0x98, 0x2B, 0x00, 0x00, 
	0xD8, 0x2B, 0x00, 0x00, 0xF0, 0x2B, 0x00, 0x00, 0x28, 0x2C, 
	0x00, 0x00, 0x50, 0x2C, 0x00, 0x00, 0x68, 0x2C, 0x00, 0x00, 
	0xA0, 0x2C, 0x00, 0x00, 0xE0, 0x2C, 0x00, 0x00, 0x08, 0x2D, 
	0x00, 0x00, 0x40, 0x2D, 0x00, 0x00, 0x54, 0x2D, 0x00, 0x00, 
	0x68, 0x2D, 0x00, 0x00, 0x7C, 0x2D, 0x00, 0x00, 0xBC, 0x2D, 
	0x00, 0x00, 0xFC, 0x2D, 0x00, 0x00, 0x3C, 0x2E, 0x00, 0x00, 
	0x64, 0x2E, 0x00, 0x00, 0x90, 0x2E, 0x00, 0x00, 0xC8, 0x2E, 
	0x00, 0x00, 0xE4, 0x2E, 0x00, 0x00, 0x24, 0x2F, 0x00, 0x00, 
	0x50, 0x2F, 0x00, 0x00, 0x7C, 0x2F, 0x00, 0x00, 0xAC, 0x2F, 
	0x00, 0x00, 0xC4, 0x2F, 0x00, 0x00, 0xEC, 0x2F, 0x00, 0x00, 
	0x14, 0x30, 0x00, 0x00, 0x40, 0x30, 0x00, 0x00, 0x74, 0x30, 
	0x00, 0x00, 0xAC, 0x30, 0x00, 0x00, 0xEC, 0x30, 0x00, 0x00, 
	0x14, 0x31, 0x00, 0x00, 0x28, 0x31, 0x00, 0x00, 0x40, 0x31, 
	0x00, 0x00, 0x5C, 0x31, 0x00, 0x00, 0x80, 0x31, 0x00, 0x00, 
	0xB0, 0x31, 0x00, 0x00, 0xCC, 0x31, 0x00, 0x00, 0xF0, 0x31, 
	0x00, 0x00, 0x10, 0x32, 0x00, 0x00, 0x4C, 0x32, 0x00, 0x00, 
	0x6C, 0x32, 0x00, 0x00, 0x80, 0x32, 0x00, 0x00, 0x94, 0x32, 
	0x00, 0x00, 0xA8, 0x32, 0x00, 0x00, 0xBC, 0x32, 0x00, 0x00, 
	0xE8, 0x32, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x14, 0x33, 
	0x00, 0x00, 0x4C, 0x33, 0x00, 0x00, 0x68, 0x33, 0x00, 0x00, 
	0x7C, 0x33, 0x00, 0x00, 0xA0, 0x33, 0x00, 0x00, 0xB4, 0x33, 
	0x00, 0x00, 0xC8, 0x33, 0x00, 0x00, 0xF4, 0x33, 0x00, 0x00, 
	0x20, 0x34, 0x00, 0x00
};
int start =1;
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
	
	if(*(unsigned int*)buf == 0)
	{
		uc_emu_stop(uc);
		return ;
	}

    int r0,r1,r2,r3,r4,r5,r6,r7;
	uc_reg_read(g_uc, UC_ARM_REG_R0, &r0);
	uc_reg_read(g_uc, UC_ARM_REG_R1, &r1);
	uc_reg_read(g_uc, UC_ARM_REG_R2, &r2);
	uc_reg_read(g_uc, UC_ARM_REG_R3, &r3);
	uc_reg_read(g_uc, UC_ARM_REG_R4, &r4);
	uc_reg_read(g_uc, UC_ARM_REG_R5, &r5);
	uc_reg_read(g_uc, UC_ARM_REG_R6, &r6);
	uc_reg_read(g_uc, UC_ARM_REG_R7, &r7);

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
			if(offset == 0xA330)
			{
				printf("");
			}
/*
			if(insn->size == 2)
				printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,(int)address-si->base,*(unsigned short*)buf, insn->mnemonic, insn->op_str);
			else
				printf("%08x[0x%04x]:\t%x\t%s\t%s\n", (int)address,(int)address-si->base,*(unsigned int*)buf, insn->mnemonic, insn->op_str);
*/

	/*		if(insn->size == 4)
			{
				int v = 0;
				char buf2[8] ={0};
				unsigned int* jmp_ptr = (unsigned int*)jmp_tbl;
				if(*(unsigned short*)buf == 0xf7ff || *(unsigned short*)buf == 0xf7fe || *(unsigned short*)buf == 0xf7fc)
				{
					uc_mem_read(uc,address-2,buf2,2);
					if(*(unsigned short*)buf2 == 0x4801)
					{
						uc_reg_read(uc,UC_ARM_REG_R0,&v);
						//printf("index 0x%x,offset 0x%x, addr 0x%x\n",v,jmp_ptr[v+1],jmp_ptr[v+1]+0x5B9C);//0x7780
						start = 0;
					}
				}
				char*p = &insn->op_str[11];
				offset = strtol(&insn->op_str[3],&p,16)-si->base;
			}
			if(offset == 0x6b7dc || offset == 0x6b7ec || offset == 0x6b7fc || offset == 0x5b98)
			{
				start = 0;
			}

			if(start && *(unsigned short*)buf != 0xb503 && *(unsigned short*)buf != 0x4801)
			{
				if(*(unsigned short*)buf == 0xdd04 || *(unsigned short*)buf == 0xdd05 || *(unsigned short*)buf == 0xd004 ||
					*(unsigned short*)buf == 0xd005 || *(unsigned short*)buf == 0xd105 || *(unsigned short*)buf == 0xd104)
				{
					char buf[64]={0};
					char*p = &insn->op_str[11];
					offset = strtol(&insn->op_str[3],&p,16)-si->base;
					sprintf(buf,"loc_%04x",offset);
					printf("loc_%04x:\t%s\t%s\n", offset,insn->mnemonic, buf);
				}
				else
				{
					if(insn->size == 2)
						printf("loc_%04x:\t%s\t%s\n", offset, insn->mnemonic, insn->op_str);
					else
						printf("loc_%04x:\t%s\t%s\n", offset, insn->mnemonic, insn->op_str);
				}
			}

			if(offset == 0x2fb0 || offset == 0x2fdc)
			{
				start = 1;
			}*/
		}
	
		cs_close(&handle);

	}
}


static void hook_instr(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	printf(">>> Tracing instr at 0x%llx, block size = 0x%x\n", address, size);
}

static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	//printf(">>> Tracing inter at 0x%llx, instruction size = 0x%x\n", address, size);

	emulator::dispatch();

}

static int hook_mem_access(uc_engine *uc, uc_mem_type type, uint64_t addr, int size, int64_t value, void *user_data)
{
	//printf(">>> Tracing mem at 0x%llx, instruction size = 0x%x\n", addr, size);
	return 1;
}

int generate_proc_info(){
    char buf[512] ={0};
    char cur_dir[1024] ={0};
	static const char* status = "Name:   com.baidu.search\n"
			"State:  R (running)\n"
			"Tgid:   7910\n"
			"Pid:    1234\n"
			"PPid:   510\n"
			"TracerPid:      0\n"
			"Uid:    2000    2000    2000    2000\n"
			"Gid:    2000    2000    2000    2000\n"
			"FDSize: 32\n"
			"Groups: 1004 1007 1011 1015 1028 3001 3002 3003 3006\n"
			"VmPeak:     3240 kB\n"
			"VmSize:     3240 kB\n"
			"VmLck:         0 kB\n"
			"VmPin:         0 kB\n"
			"VmHWM:      1268 kB\n"
			"VmRSS:      1268 kB\n"
			"VmData:     1152 kB\n"
			"VmStk:       136 kB\n"
			"VmExe:       160 kB\n"
			"VmLib:      1412 kB\n"
			"VmPTE:         6 kB\n"
			"VmSwap:        0 kB\n"
			"Threads:        1\n"
			"SigQ:   0/11693\n"
			"SigPnd: 0000000000000000\n"
			"ShdPnd: 0000000000000000\n"
			"SigBlk: 0000000000000000\n"
			"SigIgn: 0000000000380000\n"
			"SigCgt: 000000000801f4ff\n"
			"CapInh: 0000000000000000\n"
			"CapPrm: 0000000000000000\n"
			"CapEff: 0000000000000000\n"
			"CapBnd: fffffff0000000c0\n"
			"Cpus_allowed:   f\n"
			"Cpus_allowed_list:      0-3\n"
			"voluntary_ctxt_switches:        36\n"
			"nonvoluntary_ctxt_switches:     13";

#ifndef _MSC_VER
    getcwd(cur_dir,1024);
#else
    GetCurrentDirectory(1024,cur_dir);
#endif
    strcat(cur_dir,"/proc/self/status");

	FILE* fd = fopen(cur_dir, "wb");
	if (fd)
	{
		fwrite(status, 1, strlen(status), fd);
		fclose(fd);
	}

    return 1;
}

soinfo* load_android_so(const char* path)
{
    uc_err err;

	soinfo* handle = (soinfo*)s_dlopen(path,0);
    printf("[+] dump got\n");
    int * buf = (int*)malloc(handle->plt_rel_count*4);
    err = uc_mem_read(g_uc, (uint64_t)handle->plt_got, buf, handle->plt_rel_count*4);
    for (int i = 0; i < handle->plt_rel_count; i++){
        printf("%x: 0x%x -> %s\n", (int)handle->plt_got+i*4, buf[i], emulator::get_symbols(buf[i]));
    }
    free(buf);
    generate_proc_info();
	return handle;
}

int make_export_func_call(emulator* emu, soinfo* si, const char* name){

    unsigned int func = (unsigned int)s_dlsym(si, name);
	unsigned int env = emu->get_global_jnienv();
    uc_err err=uc_reg_write(g_uc, UC_ARM_REG_R0, &env);

    unsigned int obj = (unsigned int)emu->get_app_object();
    uc_reg_write(g_uc, UC_ARM_REG_R1, &obj);

    unsigned int arg2 = (unsigned int)sys_malloc(64);

    if (arg2){
        uc_mem_write(g_uc,arg2, "arg1", 8);
        uc_reg_write(g_uc, UC_ARM_REG_R2, &arg2);
    }

    unsigned int arg3 = (unsigned int)sys_malloc(1024);
    if (arg3){
        uc_mem_write(g_uc,arg3, "arg3", 8);
        uc_reg_write(g_uc, UC_ARM_REG_R2, &arg3);
    }
	unsigned int arg4 = 1;
	uc_reg_write(g_uc, UC_ARM_REG_R4, &arg4);

    emu->start_emulator((unsigned int)func,si);

	return 1;
}

int start_vm(uc_engine* uc,soinfo* si,void* JNI_OnLoad)
{
	uc_hook trace1, trace2;
	uc_hook trace3, trace4;
	uc_hook trace5,trace6;
    uc_hook trace7;

	uintptr_t lr = EMULATOR_MEMORY_START;
	uintptr_t pc = (uintptr_t)JNI_OnLoad;     // R2 register
	int cpsr=0x800d0030;

	uc_err err;

	err=uc_reg_write(g_uc, UC_ARM_REG_PC, &pc);

	err=uc_reg_write(uc, UC_ARM_REG_LR, &lr);
	//err=uc_reg_write(uc, UC_ARM_REG_CPSR, &cpsr);
	if(err != UC_ERR_OK) { printf("uc error %d\n",err);}
	//err=uc_reg_write(uc, UC_ARM_REG_SPSR, &cpsr);

	// tracing all basic blocks with customized callback
	//err=uc_hook_add(uc, &trace1, UC_HOOK_BLOCK, (void*)hook_block, NULL, JNI_OnLoad, 0);

	// tracing one instruction at ADDRESS with customized callback
	//err=uc_hook_add(uc, &trace2, UC_HOOK_CODE, (void*)hook_code, si, 1,1);
	//err=uc_hook_add(uc, &trace4, UC_HOOK_INTR, (void*)hook_inter, NULL, JNI_OnLoad, 0);

	/*err=uc_hook_add(uc, &trace5, UC_HOOK_MEM_UNMAPPED|UC_HOOK_MEM_FETCH_INVALID, (void*)hook_unmap, NULL, JNI_OnLoad, 0);

	err=uc_hook_add(uc, &trace6, UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, (void*)hook_mem_access, NULL, JNI_OnLoad, 0);

    err=uc_hook_add(uc, &trace7, UC_HOOK_MEM_INVALID, (void*)hook_mem_access, NULL, JNI_OnLoad, 0);*/

    /*if(si->init_func)
	{
		uc_err err = uc_emu_start(uc, (uint64_t)si->init_func, (uint64_t)si->init_func+10, 0, 0);
		if (err) {
			printf("Failed on uc_emu_start() with error returned: %u\n", err);
			return 0;
		}
	}
*/
	if(JNI_OnLoad)
	{
		err = uc_emu_start(uc, (uint64_t)JNI_OnLoad, (uint64_t)JNI_OnLoad+0xffffff, 0, 0);
		if (err) {
			printf("Failed on uc_emu_start() with error returned: %u\n", err);
			return 0;
		}
	}

	
	uc_close(uc);
	return 1;
}

void usage()
{
    printf("usage: emulator sopath (arm|thumb)\n");
}

int baidu_protect_init(soinfo* si)
{
	uc_err err;
	int size = 1000;
	int base = si->base;

	void* addr = sys_malloc(size);
	err = uc_mem_write(g_uc, base+0x14874, &addr, 4);

	addr = sys_malloc(size);
	err = uc_mem_write(g_uc, base + 0x14878, &addr, 4);

	addr = sys_malloc(size);
	err = uc_mem_write(g_uc, base + 0x14884, &addr, 4);

	err = uc_mem_write(g_uc, base + 0x1487c, &size, 4);
	err = uc_mem_write(g_uc, base + 0x14888, &size, 4);

	return 1;
}

int main(int argc, char* argv[])
{
    const char* p = 0;
    char path[260]={0};

    unsigned int clock = 0;
    unsigned int seconds =0;
    unsigned int mseconds = 0; 
    unsigned int minutes = 0;

    if(argc < 2 )
    {
        usage();
        return 0;
    }

    for (int i = 1; i < argc; i++ ) 
    {
        p = argv[i];

        if (*p == '/' || *p == '-' ) 
        {
            p++;

            switch (toupper(*p)) 
            {
            case 'A':
            case 'a':
                g_armmode = true;
                break;
            case 'V':
            case 'v':
                g_show_ins = true;
                break;
            case 'I':
            case 'i':
                g_emu_init = true;
                break;
            default:
                usage();
               // return 0;
            }
        }
    }

    for (int i = 1; i < argc; i ++) 
    {
        p = argv[i];
        if (*p == '-')	// skip option
            continue;	
        strncpy(path,p,256);
    }

    emulator* emu = emulator::get_emulator(g_armmode?UC_MODE_ARM:UC_MODE_THUMB);

    //soinfo* si = load_android_so("libsgmainso-6.0.71.so");
    //soinfo* si = load_android_so("libsgmainso-5.1.38.so");
    //soinfo* si = load_android_so("libsecuritysdk-2.6.24.so");
    soinfo* si = load_android_so(path);
    //soinfo* si = load_android_so("libutil.so");
    //soinfo* si = load_android_so("libjiagu.so");
    //soinfo* si = load_android_so("libbaiduprotect.so");
    //soinfo* si = load_android_so("libsgsecuritybodyso-5.1.15.so");
  
    //make_export_func_call(emu, si, "Java_com_aliyun_security_yunceng_android_sdk_YunCeng_initExRaw");

    unsigned int JNI_OnLoad = (unsigned int)s_dlsym(si,"JNI_OnLoad");
    JNI_OnLoad = (g_armmode && JNI_OnLoad&1)? JNI_OnLoad-1:JNI_OnLoad;
    uint64_t addr =0 ;
    // uc_virt_to_phys(g_uc,(uint64_t*)&addr,(uint64_t)si->base);

	baidu_protect_init(si);

    emu->start_emulator((unsigned int)si->base+0x10c78,si);
    emu->dispose();
    delete emu;

    return 1;
}

