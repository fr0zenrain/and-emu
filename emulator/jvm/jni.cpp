#include "jni.h"
#include "classobject.h"

 jobject addLocalReference(void* self, Object* obj) {
    if (obj == NULL) {
        return NULL;
    }

/*    IndirectRefTable* pRefTable = &self->jniLocalRefTable;
    void* curFrame = self->interpSave.curFrame;
    u4 cookie = SAVEAREA_FROM_FP(curFrame)->xtra.localRefCookie;
    jobject jobj = (jobject) pRefTable->add(cookie, obj);
    if (UNLIKELY(jobj == NULL)) {
        AddLocalReferenceFailure(pRefTable);
    }
    if (UNLIKELY(gDvmJni.workAroundAppJniBugs)) {
        // Hand out direct pointers to support broken old apps.
        return reinterpret_cast<jobject>(obj);
    }*/
    return 0;//jobj;
}
