#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdlib.h"

typedef unsigned char            u1;
typedef unsigned short            u2;
typedef unsigned int            u4;
typedef char              s1;
typedef short             s2;
typedef int             s4;
#ifdef _MSC_VER
typedef unsigned __int64            u8;
typedef __int64             s8;
#else
typedef unsigned long long            u8;
typedef long long             s8;
#endif

struct Object;

union JValue {
    u1      z;
    s1      b;
    u2      c;
    s2      s;
    s4      i;
    s8      j;
    float   f;
    double  d;
    Object* l;
};

#define OFFSETOF_MEMBER(t, f)         \
  (reinterpret_cast<char*>(           \
     &reinterpret_cast<t*>(16)->f) -  \
   reinterpret_cast<char*>(16))


#ifndef NULL
#define NULL    0
#endif

enum {
    ALLOC_DEFAULT = 0x00,
    ALLOC_DONT_TRACK = 0x01,  /* don't add to internal tracking list */
    ALLOC_NON_MOVING = 0x02,
};


#endif