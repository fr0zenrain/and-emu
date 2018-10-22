#include "java.h"
#include <map>
#include <string>
#include "../runtime/runtime.h"

extern uc_engine* g_uc;
std::map<std::string, void*> g_class_map;

const char* java_lang_class = "java/lang/Class";

class_method java_lang_class_method[]= {
        {0x2551d904, "getName()Ljava/lang/String;",(void*)java_class::java_lang_class_get_name},
};

java_class_type java_method[]= {
        {java_lang_class, java_lang_class_method},
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

unsigned int make_object(const char* object_class, unsigned int object_data){

    unsigned int addr = (unsigned int)sys_malloc(8);
    if (addr == NULL){
        return 0;
    }
    int class_name_size = strlen(object_class);
    unsigned int classz = (unsigned int)sys_malloc(class_name_size+1);
    if (classz){
        uc_mem_write(g_uc,addr, &classz, 4);
        uc_mem_write(g_uc,addr+4, &object_data, 4);
    }
    return addr;
}
