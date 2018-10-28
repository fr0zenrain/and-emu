#include "java.h"
#include <map>
#include <string>
#include "jvm.h"
#include "../runtime/runtime.h"
#include "../crc32.h"

extern uc_engine* g_uc;
std::map<std::string, void*> g_class_map;

const char* java_lang_class = "java/lang/Class";
const char* java_lang_System = "java/lang/System";
const char* java_lang_String = "java/lang/String";
const char* android_os_Build_VERSION = "android/os/Build$VERSION";
const char* android_app_ActivityThread= "android/app/ActivityThread";
const char* com_stub_StubApp = "com/stub/StubApp";
const char* android_content_Context = "android/content/Contex";
const char* com_qihoo_util_QHDialog = "com/qihoo/util/QHDialog";
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
        {0x8c960769, "<init>([BLjava/lang/String;)V",(void*)java_class::java_lang_class_get_name},
        {0x2551d904, "getName()Ljava/lang/String;",(void*)java_class::java_lang_class_get_name},
        {0x0a0cfcb0, "getPackageName()Ljava/lang/String;",(void*)android_os::get_pkg_name},
        {0x373612a0, "getPackageManager()Landroid/content/pm/PackageManager;",(void*)java_class::java_lang_class_get_name},
        {0x957deaca, "checkPermission(Ljava/lang/String;Ljava/lang/String;)I",(void*)java_class::java_lang_class_get_name},
};

class_method android_os_Build_VERSION_method[] = {
        {0x9cdc0e8a, "SDK_INTI",(void*)android_os::get_sdk_int},
        {0x82ebc47e, "CONNECTIVITY_SERVICELjava/lang/String;",(void*)android_os::get_sdk_int},
};

class_method android_app_ActivityThread_method[]= {

};

class_method com_stub_StubApp_method[]= {
        {0x284522f2, "getAppContext()Landroid/content/Context;",(void*)stub_app::get_app_context},
        {0xb7706e0b, "showDialog(Landroid/content/Context;Ljava/lang/String;)V",(void*)stub_app::get_app_context},
};

java_class_type java_method[]= {
        {java_lang_class, java_lang_class_method},
        {java_lang_String, java_lang_class_method},
        {java_lang_System, java_lang_class_method},
        {com_taobao_wireless_security_adapter_JNICLibrary, java_lang_class_method},
        {com_taobao_wireless_security_adapter_datacollection_DeviceInfoCapturer, java_lang_class_method},
        {com_taobao_wireless_security_adapter_datareport_DataReportJniBridge, java_lang_class_method},
        {com_taobao_wireless_security_adapter_dynamicupdatelib_DynamicUpdateLibAdapter, java_lang_class_method},
        {com_taobao_wireless_security_adapter_common_HttpUtil, java_lang_class_method},
        {com_taobao_wireless_security_adapter_common_SPUtility2, java_lang_class_method},
        {android_app_ActivityThread, android_app_ActivityThread_method},
        {com_stub_StubApp, java_lang_class_method},
        {android_os_Build_VERSION, android_os_Build_VERSION_method},
        {com_qihoo_util_QHDialog, java_lang_class_method},
        {android_content_Context,java_lang_class_method}
};


unsigned int java_class::java_lang_class_get_name(){
    return 1;
}

unsigned int stub_app::get_app_context(){
    return 1;
}

unsigned int android_os::get_sdk_int(){
    return 19;
}

unsigned int android_os::get_pkg_name(){
    return emulator::get_pkg_name();
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
    return (void*)1;
}

unsigned int get_field(class_method* method, const char* name, const char* sig){
    unsigned int hash = crc32(name,strlen(name), 0);
    hash = crc32(sig,strlen(sig), hash);
    for (int i = 0; i < sizeof(android_os_Build_VERSION_method)/ sizeof(android_os_Build_VERSION_method[i]); i++){
        if (hash == android_os_Build_VERSION_method[i].method_id){
            return hash;
        }
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
    for (int i = 0; i < sizeof(com_stub_StubApp_method)/ sizeof(com_stub_StubApp_method[i]); i++){
        if (hash == com_stub_StubApp_method[i].method_id){
            return hash;
        }
    }
    return 0;
}

void* get_method_byhash(unsigned int hash)
{
    void* func = (void*)1;
    for (int i = 0; i < sizeof(java_lang_class_method)/ sizeof(java_lang_class_method[i]); i++){
        if (hash == java_lang_class_method[i].method_id){
            func = java_lang_class_method[i].fake_method;
            break;
        }
    }
    for (int i = 0; i < sizeof(com_stub_StubApp_method)/ sizeof(com_stub_StubApp_method[i]); i++){
        if (hash == com_stub_StubApp_method[i].method_id){
            func = com_stub_StubApp_method[i].fake_method;
            break;
        }
    }

    return func;
}

void* get_field_byhash(unsigned int hash)
{
    void* func = (void*)1;
    for (int i = 0; i < sizeof(android_os_Build_VERSION_method)/ sizeof(android_os_Build_VERSION_method[i]); i++){
        if (hash == android_os_Build_VERSION_method[i].method_id){
            func = android_os_Build_VERSION_method[i].fake_method;
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