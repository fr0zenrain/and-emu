#ifndef __JVM_H__
#define __JVM_H__
#include "jni.h"

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

};

#endif