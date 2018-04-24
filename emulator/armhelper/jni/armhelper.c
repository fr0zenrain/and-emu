#include "base.h"

void* s_memcpy(void* dst,void* src,size_t n)
{
    if ((((unsigned int) dst) | ((unsigned int) dst) | n) & (sizeof(unsigned int) - 1)) {

        unsigned char *pS = (unsigned char *) src;
        unsigned char *pD = (unsigned char *) dst;
        unsigned char *pE = (unsigned char *) (((unsigned int) src) + n);

        while (pS != pE)
            *(pD++) = *(pS++);
    }
    else {
        unsigned int *pS = (unsigned int *) src;
        unsigned int *pD = (unsigned int *) dst;
        unsigned int *pE = (unsigned int *) (unsigned char *) (((unsigned int) src) + n);

        while (pS != pE)
            *(pD++) = *(pS++);
    }
    return dst;
}

void * s_memset(void *d, int v, size_t c)
{
    if ((((unsigned int) d) | c) & (sizeof(unsigned int) - 1)) {

        unsigned char *pD = (unsigned char *) d;
        unsigned char *pE = (unsigned char *) (((unsigned int) d) + c);

        while (pD != pE)
            *(pD++) = (unsigned char) v;

    }
    else {

        unsigned int *pD = (unsigned int *) d;
        unsigned int *pE = (unsigned int *) (unsigned char *) (((unsigned int) d) + c);
        unsigned int uv;

        uv = ((unsigned int) (v & 0xff)) | (((unsigned int) (v & 0xff)) << 8);

        /* Our processors are at least 32 bits
         */
        uv |= uv << 16;

#if (_UINTSIZE == 64)
        /* They might be 64 bits
         */
        uv |= uv << 32;
#endif

        while (pD != pE)
            *(pD++) = uv;
    }
    return d;
}


size_t s_strlen(const char *s)
{
    const char *pEnd;

    for (pEnd = s; *pEnd != '\0'; pEnd++)
        continue;

    return pEnd - s;
}

int s_strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

char* s_strrchr(const char* p, int ch)
{
    char *save;
    char c;

    for (save = (char *) 0; (c = *p); p++) {
        if (c == ch)
            save = (char *) p;
    }

    return save;
}

char * s_itoa(int n, char * buf)
{
    int i = 1;
    char * pch = buf;
    if(!pch) return 0;
    while(n / i) i *= 10;

    if(n < 0)
    {
        n = -n;
        *pch++ = '-';
    }
    if (0 == n) i = 10;

    while(i /= 10)
    {
        *pch++ = n / i + '0';
        n %= i;
    }
    *pch = '\0';
    return buf;
}

int s_atoi(char *str)
{
    int sign;
    int n;
    char *p = str;

    while (_isspace(*p) ) p++;

    sign = ('-' == *p) ? -1 : 1;
    if ('+' == *p || '-' == *p) p++;

    for (n = 0; _isdigit(*p); p++)
        n = 10 * n + (*p - '0');

    return sign*n;
}

int s_strncmp(const char * dst, const char * src, size_t n)
{
    if (n == 0)
        return (0);
    do {
        if (*dst != *src++)
            return (*(unsigned char *)dst - *(unsigned char *)--src);
        if (*dst++ == 0)
            break;
    } while (--n != 0);
    return (0);
}

unsigned int s_strnlen(const char *s, int count)
{
    const char *sc;
    for (sc = s; *sc != '\0' && count--; ++sc);
    return sc - s;
}

char* s_strchr(const char* s, int ch)
{
    for (;;) {
        if (*s == (char)ch)
            return (char *)s;
        if (*s == '\0')
            return 0;
        s++;
    }
}

char * s_strncpy(char *dst, const char *src, size_t n)
{
    if (n != 0) {
        char *d = dst;
        const char *s = src;

        do {
            if ((*d++ = *s++) == 0) {
                /* NUL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }
    return (dst);
}

void * s_memchr(const void *s, int c, size_t n)
{
    if (n != 0) {
        const unsigned char *p = (const unsigned char *)s;

        do {
            if (*p++ == (unsigned char)c)
                return ((void *)(p - 1));
        } while (--n != 0);
    }
    return 0;
}
