#ifndef __UTFSTRING_H__
#define __UTFSTRING_H__

#include "object.h"

size_t dvmUtf8Len(const char* utf8Str);
StringObject* dvmCreateStringFromCstrAndLength(const char* utf8Str,
                                               size_t utf16Length);

inline void dvmSetFieldInt(Object* obj, int offset, s4 val) {
    ((JValue*)BYTE_OFFSET(obj, offset))->i = val;
}

#endif