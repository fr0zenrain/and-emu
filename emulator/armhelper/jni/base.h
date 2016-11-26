#ifndef __TYPE_H__
#define __TYPE_H__
#include "stdarg.h"

typedef int size_t;

#define _isdigit(c) ((c) >= '0' && (c) <= '9')
#define _islower(c) ((c)>='a' && (c)<= 'z')
#define _isspace(c)  ((unsigned int)(c - 9) < 5u || ' ' == c)
#define _isascii(c) (((unsigned char)(c))<=0x7f)
#define _toascii(c) (((unsigned char)(c))&0x7f)
#define _isprint(c) ((c) > 0x1f && c< 0x7f)
#define _isalpha(c) ((c >='a' && c <='z') || (c >='A' && c <='Z'))
#define _isupper(c) ((c>='A') && (c<='Z'))
#define _max(a,b)            (((a) > (b)) ? (a) : (b))
#define _min(a,b)            (((a) < (b)) ? (a) : (b))
#define NULL 0


#ifdef __cplusplus
extern "C" {
#endif

void* s_memcpy(void* dst,void* src,size_t n);
void * s_memset(void *d, int v, size_t c);
size_t s_strlen(const char *s);
char* s_strrchr(const char* p, int ch);
int s_strcmp(const char *s1, const char *s2);
char * s_itoa(int n, char * buf);
int s_atoi(char *str);
int s_strncmp(const char * dst, const char * src, size_t n);
unsigned int s_strnlen(const char *s, int count);
char* s_strchr(const char* s, int ch);
char * s_strncpy(char *dst, const char *src, size_t n);
void * s_memchr(const void *s, int c, size_t n);
int sprintf(char *str, const char *fmt, ...);

#ifdef __cplusplus
}
#endif


#endif