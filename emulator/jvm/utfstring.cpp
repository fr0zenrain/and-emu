#include "utfstring.h"
#include "alloc.h"
#include <string>
using namespace std;

# define STRING_FIELDOFF_VALUE      8
# define STRING_FIELDOFF_HASHCODE   12
# define STRING_FIELDOFF_OFFSET     16
# define STRING_FIELDOFF_COUNT      20

size_t dvmUtf8Len(const char* utf8Str)
{
    size_t len = 0;
    int ic;

    while ((ic = *utf8Str++) != '\0') {
        len++;
        if ((ic & 0x80) != 0) {
            /* two- or three-byte encoding */
            utf8Str++;
            if ((ic & 0x20) != 0) {
                /* three-byte encoding */
                utf8Str++;
            }
        }
    }

    return len;
}

static inline u4 computeUtf16Hash(const u2* utf16Str, size_t len)
{
    u4 hash = 0;

    while (len--)
        hash = hash * 31 + *utf16Str++;

    return hash;
}

inline u2 dexGetUtf16FromUtf8(const char** pUtf8Ptr)
{
    unsigned int one, two, three;

    one = *(*pUtf8Ptr)++;
    if ((one & 0x80) != 0) {
        /* two- or three-byte encoding */
        two = *(*pUtf8Ptr)++;
        if ((one & 0x20) != 0) {
            /* three-byte encoding */
            three = *(*pUtf8Ptr)++;
            return ((one & 0x0f) << 12) |
                   ((two & 0x3f) << 6) |
                   (three & 0x3f);
        } else {
            /* two-byte encoding */
            return ((one & 0x1f) << 6) |
                   (two & 0x3f);
        }
    } else {
        /* one-byte encoding */
        return one;
    }
}

void dvmConvertUtf8ToUtf16(u2* utf16Str, const char* utf8Str)
{
    while (*utf8Str != '\0')
        *utf16Str++ = dexGetUtf16FromUtf8(&utf8Str);
}

std::string dvmHumanReadableDescriptor(const char* descriptor) {
    // Count the number of '['s to get the dimensionality.
    const char* c = descriptor;
    size_t dim = 0;
    while (*c == '[') {
        dim++;
        c++;
    }

    // Reference or primitive?
    if (*c == 'L') {
        // "[[La/b/C;" -> "a.b.C[][]".
        c++; // Skip the 'L'.
    } else {
        // "[[B" -> "byte[][]".
        // To make life easier, we make primitives look like unqualified
        // reference types.
        switch (*c) {
            case 'B': c = "byte;"; break;
            case 'C': c = "char;"; break;
            case 'D': c = "double;"; break;
            case 'F': c = "float;"; break;
            case 'I': c = "int;"; break;
            case 'J': c = "long;"; break;
            case 'S': c = "short;"; break;
            case 'Z': c = "boolean;"; break;
            default: return descriptor;
        }
    }

    // At this point, 'c' is a string of the form "fully/qualified/Type;"
    // or "primitive;". Rewrite the type with '.' instead of '/':
    std::string result;
    const char* p = c;
    while (*p != ';') {
        char ch = *p++;
        if (ch == '/') {
            ch = '.';
        }
        result.push_back(ch);
    }
    // ...and replace the semicolon with 'dim' "[]" pairs:
    while (dim--) {
        result += "[]";
    }
    return result;
}

#define CLZ(x) __builtin_clz(x)
static ArrayObject* allocArray(ClassObject* arrayClass, size_t length,
                               size_t elemWidth, int allocFlags)
{
    size_t elementShift = sizeof(size_t) * 1 - 1 - CLZ(elemWidth);
    size_t elementSize = length << elementShift;
    size_t headerSize = OFFSETOF_MEMBER(ArrayObject, contents);
    size_t totalSize = elementSize + headerSize;
    if (elementSize >> elementShift != length || totalSize < elementSize) {
        std::string descriptor(dvmHumanReadableDescriptor(arrayClass->descriptor));
        return NULL;
    }
    ArrayObject* newArray = (ArrayObject*)sys_malloc(totalSize);
    if (newArray != NULL) {
        DVM_OBJECT_INIT(newArray, arrayClass);
        newArray->length = length;
        //dvmTrackAllocation(arrayClass, totalSize);
    }
    return newArray;
}


ArrayObject* dvmAllocPrimitiveArray(char type, size_t length, int allocFlags)
{
    ArrayObject* newArray;
    ClassObject* arrayClass;
    int width;

    switch (type) {
        case 'I':
            arrayClass = 0;//gDvm.classArrayInt;
            width = 4;
            break;
        case 'C':
            arrayClass = 0;//gDvm.classArrayChar;
            width = 2;
            break;
        case 'B':
            arrayClass = 0;//gDvm.classArrayByte;
            width = 1;
            break;
        case 'Z':
            arrayClass = 0;//gDvm.classArrayBoolean;
            width = 1; /* special-case this? */
            break;
        case 'F':
            arrayClass = 0;//gDvm.classArrayFloat;
            width = 4;
            break;
        case 'D':
            arrayClass = 0;//gDvm.classArrayDouble;
            width = 8;
            break;
        case 'S':
            arrayClass = 0;//gDvm.classArrayShort;
            width = 2;
            break;
        case 'J':
            arrayClass = 0;//gDvm.classArrayLong;
            width = 8;
            break;
        default:
            return NULL; // Keeps the compiler happy.
    }

    newArray = allocArray(arrayClass, length, width, allocFlags);

    /* the caller must dvmReleaseTrackedAlloc if allocFlags==ALLOC_DEFAULT */
    return newArray;
}

static StringObject* makeStringObject(u4 charsLength, ArrayObject** pChars)
{
    Object* result = dvmAllocObject(0,ALLOC_DEFAULT);//gDvm.classJavaLangString, ALLOC_DEFAULT);
    if (result == NULL) {
        return NULL;
    }

    ArrayObject* chars = dvmAllocPrimitiveArray('C', charsLength, ALLOC_DEFAULT);
    if (chars == NULL) {
        //dvmReleaseTrackedAlloc(result, NULL);
        return NULL;
    }

    dvmSetFieldInt(result, STRING_FIELDOFF_COUNT, charsLength);
    dvmSetFieldObject(result, STRING_FIELDOFF_VALUE, (Object*) chars);
    //dvmReleaseTrackedAlloc((Object*) chars, NULL);
    /* Leave offset and hashCode set to zero. */

    *pChars = chars;
    return (StringObject*) result;
}

StringObject* dvmCreateStringFromCstrAndLength(const char* utf8Str,
                                               size_t utf16Length)
{
    ArrayObject* chars;
    StringObject* newObj = makeStringObject(utf16Length, &chars);
    if (newObj == NULL) {
        return NULL;
    }

    dvmConvertUtf8ToUtf16((u2*)(void*)chars->contents, utf8Str);

    u4 hashCode = computeUtf16Hash((u2*)(void*)chars->contents, utf16Length);
    dvmSetFieldInt((Object*) newObj, STRING_FIELDOFF_HASHCODE, hashCode);

    return newObj;
}