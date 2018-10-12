#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdlib.h>
#include "../include/unicorn/unicorn.h"

void *s_mmap(void *start, size_t length, int prot, int flags,int fd, int offset);
int s_mprotect( void *start, size_t len, int prot);
int s_munmap(void *start, size_t length);
size_t s_strlcpy(char *dst, const char *src, size_t siz);
int s_open( const char * pathname, int flags);
void* s_dlopen(const char* filename, int flags);
void print_hex_dump_bytes(const void *buf, size_t len);

#undef offsetof
#define offsetof(s, m)   (size_t)&(((s *)0)->m)

#endif