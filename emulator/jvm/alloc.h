#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "../runtime/runtime.h"

#include "classobject.h"
#include  "jni.h"

extern uc_engine* g_uc;

inline u1 *dvmCardFromAddr(const void *addr)
{
    u1 *biasedBase = 0;//gDvm.biasedCardTableBase;
    u1 *cardAddr = 0;//biasedBase + ((unsigned int)addr >> GC_CARD_SHIFT);
    //assert(dvmIsValidCard(cardAddr));
    return cardAddr;
}

inline void dvmMarkCard(const void *addr)
{
    u1 *cardAddr = dvmCardFromAddr(addr);
    //*cardAddr = GC_CARD_DIRTY;
}

inline void dvmWriteBarrierField(const Object *obj, void *addr)
{
    dvmMarkCard(obj);
}

inline void dvmSetFieldObject(Object* obj, int offset, Object* val) {
    JValue* lhs = (JValue*)BYTE_OFFSET(obj, offset);
    //lhs->l = val;
    unsigned int addr = (uintptr_t)lhs + offsetof(JValue,l);
    uc_mem_write(g_uc,addr,&val ,4);
    if (val != NULL) {
        //dvmWriteBarrierField(obj, &lhs->l);
    }
}

Object* dvmAllocObject(ClassObject* clazz, int flags);

jobject addLocalReference(void* self, Object* obj);

#endif