#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdlib.h>

void *s_mmap(void *start, size_t length, int prot, int flags,int fd, int offset);
int s_mprotect( void *start, size_t len, int prot);
int s_munmap(void *start, size_t length);
size_t s_strlcpy(char *dst, const char *src, size_t siz);
int s_open( const char * pathname, int flags);
void* s_dlopen(const char* filename, int flags);


#endif