#ifndef __DLFCN_H__
#define __DLFCN_H__

void* s_dlopen(const char* filename, int flags);
void* s_dlsym(void* handle, const char* symbol) ;

#endif