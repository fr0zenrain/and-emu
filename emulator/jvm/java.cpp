#include "java.h"
#include <map>
#include <string>
#include "jvm.h"
#include "../runtime/runtime.h"
#include "../crc32.h"

extern uc_engine* g_uc;
std::map<std::string, void*> g_class_map;

const char* java_lang_class = "java/lang/Class";
const char* com_taobao_wireless_security_adapter_datacollection_DeviceInfoCapturer =
        "com/taobao/wireless/security/adapter/datacollection/DeviceInfoCapturer";
const char* com_taobao_wireless_security_adapter_JNICLibrary = "com/taobao/wireless/security/adapter/JNICLibrary";
const char* com_taobao_wireless_security_adapter_datareport_DataReportJniBridge =
        "com/taobao/wireless/security/adapter/datareport/DataReportJniBridge";
const char* com_taobao_wireless_security_adapter_dynamicupdatelib_DynamicUpdateLibAdapter =
        "com/taobao/wireless/security/adapter/dynamicupdatelib/DynamicUpdateLibAdapter";
const char* com_taobao_wireless_security_adapter_common_HttpUtil =
        "com/taobao/wireless/security/adapter/common/HttpUtil";
const char* com_taobao_wireless_security_adapter_common_SPUtility2 =
        "com/taobao/wireless/security/adapter/common/SPUtility2";
class_method java_lang_class_method[]= {
        {0x2551d904, "getName()Ljava/lang/String;",(void*)java_class::java_lang_class_get_name},
};


java_class_type java_method[]= {
        {java_lang_class, java_lang_class_method},
        {com_taobao_wireless_security_adapter_JNICLibrary, java_lang_class_method},
        {com_taobao_wireless_security_adapter_datacollection_DeviceInfoCapturer, java_lang_class_method},
        {com_taobao_wireless_security_adapter_datareport_DataReportJniBridge, java_lang_class_method},
        {com_taobao_wireless_security_adapter_dynamicupdatelib_DynamicUpdateLibAdapter, java_lang_class_method},
        {com_taobao_wireless_security_adapter_common_HttpUtil, java_lang_class_method},
        {com_taobao_wireless_security_adapter_common_SPUtility2, java_lang_class_method},
};


int java_class::java_lang_class_get_name(){
    return 1;
}


void init_java_class(){

    printf("init java class method\n");

    for (int i = 0; i < sizeof(java_method)/ sizeof(java_method[i]); i++){
        g_class_map.insert(std::make_pair(java_method[i].class_name, java_method[i].fake_method));
    }

}

void* get_class(const char* name){
    std::map<std::string, void*>::iterator iter = g_class_map.find(name);
    if (iter != g_class_map.end()){
        return iter->second;
    }
    return 0;
}

unsigned int get_method(class_method* method, const char* name, const char* sig){
    unsigned int hash = crc32(name,strlen(name), 0);
    hash = crc32(sig,strlen(sig), hash);
    for (int i = 0; i < sizeof(java_lang_class_method)/ sizeof(java_lang_class_method[i]); i++){
        if (hash == java_lang_class_method[i].method_id){
            return hash;
        }
    }

    return 0;
}

void* get_method_byhash(unsigned int hash)
{
    void* func = NULL;
    for (int i = 0; i < sizeof(java_lang_class_method)/ sizeof(java_lang_class_method[i]); i++){
        if (hash == java_lang_class_method[i].method_id){
            func = java_lang_class_method[i].fake_method;
            break;
        }
    }

    return func;
}

unsigned int make_object(const char* object_class, unsigned int object_data){

    unsigned int addr = (unsigned int)sys_malloc(8);
    if (addr == 0){
        return 0;
    }
    int class_name_size = strlen(object_class);
    unsigned int classz = (unsigned int)sys_malloc(class_name_size+1);
    if (classz){
        uc_mem_write(g_uc,classz, object_class, class_name_size);
        uc_mem_write(g_uc,addr, &classz, 4);
        uc_mem_write(g_uc,addr+4, &object_data, 4);
    }
    return addr;
}

unsigned int make_string_object(const char* data){
    int size = strlen(data);
    int tp = JTYPE_STRING;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr == 0){
        return 0;
    }

    unsigned int data_addr = (unsigned int)sys_malloc(size);
    if (data_addr){
        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &size, 4);
        uc_mem_write(g_uc,data_addr, data, size);
        unsigned int obj_addr = (unsigned int)make_object("java/lang/Class", data_addr);
        if (addr){
            uc_mem_write(g_uc,addr+8, &obj_addr, 4);
        }
    }

    return addr;
}

unsigned int make_bytearray(unsigned char* data, int size){

    int tp = JTYPE_BYTEARRAY;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr == 0){
        return 0;
    }
    unsigned int data_addr = (unsigned int)sys_malloc(size);
    if (data_addr){
        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &size, 4);
        uc_mem_write(g_uc,data_addr, data, size);
        unsigned int obj_addr = (unsigned int)make_object("java/lang/ByteArray", data_addr);
        if (addr){
            uc_mem_write(g_uc,addr+8, &obj_addr, 4);
        }
    }
    return addr;
}

unsigned int make_intarray(int a, int b){

    int tp = JTYPE_ARRAY;
    int size = 2;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr == 0){
        return 0;
    }
    unsigned int data_addr = (unsigned int)sys_malloc(size*4);
    if (data_addr){
        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &size, 4);
        uc_mem_write(g_uc,addr+8, &data_addr, 4);
        uc_mem_write(g_uc,data_addr, &a, 4);
        uc_mem_write(g_uc,data_addr+4, &b, 4);
    }
    return addr;
}

unsigned int make_stringarray(const char* s1, const char* s2){
    int cnt = 2;
    int tp = JTYPE_ARRAY;
    unsigned int addr = (unsigned int)sys_malloc(12);
    if (addr){

        uc_mem_write(g_uc,addr, &tp, 4);
        uc_mem_write(g_uc,addr+4, &cnt, 4);
        unsigned int saddr = (unsigned int)sys_malloc(cnt*4);
        if (saddr){
            unsigned int s1_addr = (unsigned int)make_string_object(s1);
            if (s1_addr){
                uc_mem_write(g_uc,saddr, &s1_addr, 4);
            }
            unsigned int s2_addr = (unsigned int)make_string_object(s1);
            if (s2_addr){
                uc_mem_write(g_uc,s2_addr, &s2_addr, 4);
            }
        }
        uc_mem_write(g_uc,addr+8, &saddr, 4);

    }
    return addr;
}
