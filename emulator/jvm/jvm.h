#ifndef __JVM_H__
#define __JVM_H__
#include "jni.h"
#include "method.h"

struct JNIEnvExt {
	const struct JNINativeInterface* funcTable;     /* must be first */

	const struct JNINativeInterface* baseFuncTable;

	unsigned int  envThreadId;
	void* self;

	/* if nonzero, we are in a "critical" JNI call */
	int     critical;

	struct JNIEnvExt* prev;
	struct JNIEnvExt* next;
};


struct JavaVMExt {
	const struct JNIInvokeInterface* funcTable;     /* must be first */

	const struct JNIInvokeInterface* baseFuncTable;

	/* head of list of JNIEnvs associated with this VM */
	JNIEnvExt*      envList;
	int envListLock;
};

typedef struct _func_info{
	int (*f)();
}func_info;

class jvm
{

public:
	jvm(){}
	~jvm(){}

public:
    int initialize();

private:
    void PushLocalFrame(int size);
    void PopLocalFrame(int size);
    jclass FindClass(const char*);
    jstring NewStringUTF(char* bytes);

public:
    int doJNICall(const char* name, const char* sig);
    jobject NewObject(jclass jclazz);

};

#define JTYPE_INT     0
#define JTYPE_STRING  1
#define JTYPE_BYTE    2
#define JTYPE_OBJECT  3

#endif