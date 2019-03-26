#include "jvm.h"
#include "utfstring.h"
#include "alloc.h"

void jvm::PushLocalFrame(int size)
{
}

void jvm::PopLocalFrame(int size)
{
}

const Method* dvmGetCurrentJNIMethod() {

    //void* fp = dvmThreadSelf()->interpSave.curFrame;
    //const Method* meth = SAVEAREA_FROM_FP(fp)->method;

    return 0;//meth;
}

jclass jvm::FindClass(const char* name)
{
    return 0;
}

jstring jvm::NewStringUTF(char* bytes)
{
    StringObject* newStr = dvmCreateStringFromCstrAndLength(bytes,dvmUtf8Len(bytes));
    jstring result = (jstring) addLocalReference(0, (Object*) newStr);
    return result;
}

jobject jvm::NewObject(jclass jclazz) {
    ClassObject* clazz = new ClassObject();
    Object* newObj = dvmAllocObject(clazz, ALLOC_DONT_TRACK);
   // jobject result = addLocalReference(0,newObj);
    return (jobject)newObj;
}


int jvm::initialize()
{


    return 1;
}
