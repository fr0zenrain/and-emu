
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
    static int java_lang_class_get_name();
};


void init_java_class();
void* get_class(const char* name);
unsigned int make_object();