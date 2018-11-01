#ifndef _JAVA_H__
#define _JAVA_H__

struct class_method{
    unsigned int method_id;
    const char* class_method_name;
    void* fake_method;
};

struct java_class_type{
    const char* class_name;
    class_method* fake_method;
};


class java_class{

public:
    static unsigned int java_lang_class_get_name(unsigned int env, unsigned int obj, unsigned int mid, unsigned int arg);
    static unsigned int java_lang_class_get_Property(unsigned int env, unsigned int obj, unsigned int mid, unsigned int arg);
    static unsigned int java_lang_class_get_Bytes(unsigned int env, unsigned int obj, unsigned int mid, unsigned int arg);
};

class virtual_app{

public:
    virtual_app();
    ~virtual_app();

public:
    int init(void* obj);
    static unsigned int get_sys_ctx();
    static unsigned int get_sdk_int();
    static unsigned int get_pkg_name();
    static unsigned int get_pkg_mgr(void* p);
    static unsigned int get_pkg_info(void* p);
    static unsigned int get_app_context(void* p);
private:
    static unsigned int ctx;
    static unsigned int mgr;
};

void init_java_class();
void* get_class(const char* name);
void* get_class_byptr(void* obj);
unsigned int make_object(const char* object_class, unsigned int object_data);
unsigned int make_string_object(const char* data);
unsigned int get_field(class_method* method, const char* name, const char* sig);
unsigned int get_method(class_method* method, const char* name, const char* sig);
void* get_method_byhash(unsigned int hash);
void* get_field_byhash(unsigned int hash);
unsigned int make_bytearray(unsigned char* data, int size);
unsigned int make_intarray(int a, int b);
unsigned int make_stringarray(const char* s1, const char* s2);

#endif