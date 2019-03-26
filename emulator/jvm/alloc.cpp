#include "alloc.h"


Object* dvmAllocObject(ClassObject* clazz, int flags)
{
    Object* newObj;

    /* allocate on GC heap; memory is zeroed out */
    newObj = (Object*)sys_malloc(clazz->objectSize);
    if (newObj != NULL) {
        dvmSetFieldObject(newObj, OFFSETOF_MEMBER(Object, clazz), clazz);
        //dvmTrackAllocation(clazz, clazz->objectSize);   /* notify DDMS */
    }

    return newObj;
}
