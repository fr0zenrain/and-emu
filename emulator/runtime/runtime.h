#ifndef __RUNTIME_H__
#define __RUNTIME_H__
#include "../linker.h"
#include <stdlib.h>

typedef void* (*f)(void*);

typedef struct _symbols{
	unsigned int hash;
	const char * name;
	void* func;
	unsigned int model;
	int number;
	unsigned int vaddr;
}symbols;

typedef struct _symbols_map{
	unsigned int vaddr;
    f func;
}symbols_map;

struct soinfo;

///////////////////////////////////////////////

struct s_timeval {
#ifdef _MSC_VER
	unsigned __int64  tv_sec;     /* seconds */
#else
	long long      tv_sec;     /* seconds */
#endif
	int tv_usec;    /* microseconds */
};

struct s_timezone {
	int tz_minuteswest;     /* minutes west of Greenwich */
	int tz_dsttime;         /* type of DST correction */
};


class libc
{

public:
	libc(){};
	~libc(){};

public:

    static soinfo* si;
    void init(emulator* emu);

	static void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
	static void hook_inter(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);
	static void start_emulator(unsigned int pc, soinfo * si);
	static void build_proc_self_maps();
public:
	static void* s_malloc(void*);
	static void* s_free(void*);
	static void* s_memset(void*);
	static void* s__system_property_get(void*);
	static void* s_gettimeofday(void*);
	static void* s_strdup(void*);
	static void* s_strlen(void*);
	static void* s_strncmp(void*);
    static void* s_strcmp(void*);
	static void* s_open(void*);
	static void* s_read(void*);
    static void* s_write(void*);
    static void* s_lseek(void*);
	static void* s_close(void*);
	static void* s_sscanf(void*);
	static void* s_strchr(void*);
	static void* s_strtoul(void*);
	static void* s__aeabi_memset(void*);
	static void* s__aeabi_memcpy(void*);
	static void* s_bsd_signal(void*);
	static void* s_raise(void*);
    static void* s_getpid(void*);
	static void* s_strcpy(void*);
    static void* s__errno(void*);
    static void* s_strerror(void*);
	static void* s_access(void*);
	static void* s_abort(void*);
    static void* s_strcat(void*);
    static void* s_pthread_mutex_lock(void*);
    static void* s_pthread_mutex_unlock(void*);
    static void* s_pipe(void*);
    static void* s_fork(void*);
    static void* s_snprintf(void*);
	static void* s_sysconf(void*);
	static void* s_fopen(void*);
	static void* s_fread(void*);
	static void* s_fseek(void*);
	static void* s_fclose(void*);
	static void* s__stack_chk_fail(void*);
	static void* s__stack_chk_guard(void*);
	static void* sys_mmap(int type);
	static void* sys_cacheflush(int type);
	static void* sys_dlopen(void*);
	static void* sys_mprotect(void*);

};


int get_prop(char* name,char* value);
extern "C" void* sys_malloc(size_t size);
extern "C" void sys_free(void* ptr);

#endif