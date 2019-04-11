#include "java.h"
#include "jvm.h"
#include "../runtime/runtime.h"

extern uc_engine* g_uc;

unsigned int virtual_app::ctx = 0;
unsigned int virtual_app::mgr = 0;

virtual_app::virtual_app() {
    ctx = 0;
    mgr = 0;
}

virtual_app::~virtual_app() {

}

int virtual_app::init(void* obj){
    ctx = get_app_context(obj);
    mgr = get_pkg_mgr(obj);
    return 1;
}

unsigned int virtual_app::get_app_context(void* obj){
    if (ctx){
        return ctx;
    }
    int size = 4;
    int tp = JTYPE_OBJECT;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr == 0){
        return 0;
    }

    unsigned int data_addr = (unsigned int)sys_malloc(size);
    if (data_addr){
        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &size, 4);
        uc_mem_write(g_uc,data_addr, &obj, 4);
        unsigned int obj_addr = (unsigned int)make_object("android.content.Context", data_addr);
        if (addr){
            uc_mem_write(g_uc,addr+8, &obj_addr, 4);
        }
    }
    ctx = addr;
    return addr;
}

unsigned int virtual_app::get_sdk_int(){
    return 19;
}

unsigned int virtual_app::get_null(){
    return 0;
}

unsigned int virtual_app::get_true(){
    return 1;
}

unsigned int virtual_app::get_pkg_name(){
    return emulator::get_pkg_name();
}

unsigned int virtual_app::get_pkg_mgr(void* p){
    int size = 4;
    if (mgr){
        return mgr;
    }
    int tp = JTYPE_OBJECT;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr == 0){
        return 0;
    }

    unsigned int data_addr = (unsigned int)sys_malloc(size);
    if (data_addr){
        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &size, 4);
        uc_mem_write(g_uc,data_addr, &p, 4);
        unsigned int obj_addr = (unsigned int)make_object("android.content.pm.PackageInfo", data_addr);
        if (addr){
            uc_mem_write(g_uc,addr+8, &obj_addr, 4);
        }
    }
    mgr = addr;
    return addr;
}