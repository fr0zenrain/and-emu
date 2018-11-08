#include "unpack.h"
#include "emulator.h"
#include "../engine.h"

extern uc_engine* g_uc;

int qihoo_jiagu_1375_patch(int address){
    int r0;
    if (address == 0x4062f926 || address == 0x4062f9e2 ||
        address == 0x40626692){
        r0 = 1;
        uc_reg_write(g_uc, UC_ARM_REG_R0, &r0);
    }

    /*if (address == 0x40614cb2){
        emulator::get_emulator()->dump_register();
        emulator::update_cpu_model();
    }*/
    return 0;
}