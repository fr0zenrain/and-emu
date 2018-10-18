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

struct soinfo;

///////////////////////////////////////////////

struct s_timeval {
#ifdef _MSC_VER
	long  tv_sec;     /* seconds */
#else
	long  tv_sec;     /* seconds */
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
	static void* s_pthread_mutex_init(void*);
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
	static void* s_time(void*);
	static void* s_sbrk(void*);
	static void* s__stack_chk_fail(void*);
	static void* s__stack_chk_guard(void*);
	static void* s_opendir(void*);
	static void* s_readdir(void*);
    static void* s_closedir(void*);
    static void* s_getppid(void*);
    static void* s_kill(void*);
    static void* s_realloc(void*);
    static void* s_atoi(void*);
    static void* s_itoa(void*);
    static void* s_stat(void*);
    static void* s_munmap(void*);
    static void* s_pthread_create(void*);
	static void* s_pthread_key_create(void*);
	static void* s_pthread_key_delete(void*);
	static void* s_pthread_getspecific(void*);
	static void* s_pthread_setspecific(void*);
    static void* s_inotify_init(void*);
    static void* s_inotify_add_watch(void*);
	static void* s_inotify_rm_watch(void*);
    static void* s_fgets(void*);
    static void* s_select(void*);
    static void* s_strlcpy(void*);
	static void* s_signal(void*);
	static void* s_rename(void*);
	static void* s_remove(void*);
	static void* s_strncpy(void*);
	static void* s_exit(void*);
	static void* s_getuid(void*);
	static void* s_unlink(void*);
	static void* s_madvise(void*);
	static void* s_puts(void*);
	static void* s_pread(void*);
	static void* s_memmove(void*);
	static void* s_setenv(void*);
	static void* s_getopt(void*);
	static void* sys_mmap(int type);
	static void* sys_cacheflush(int type);
	static void* sys_dlopen(void*);
	static void* sys_dlclose(void*);
	static void* sys_dlsym(void*);
    static void* sys_dladdr(void*);
	static void* sys_mprotect(void*);
	static void* sys_realloc(void*);
	static void* s__cxa_exit(void*);
	static void* s__cxa_finalize(void*);
	static void* s_printf(void*);
	static void* s_srand(void*);
	static void* s_srand48(void*);
    static void* s_lrand48(void*);
	static void* s_strstr(void*);
    static void* s_strtol(void*);
	static void* s_calloc(void*);
	static void* s_getenv(void*);
    static void* s_pthread_cond_broadcast(void*);
    static void* s_pthread_cond_wait(void*);
    static void* s_fwrite(void*);
    static void* s_strrchr(void*);
    static void* s_memcmp(void*);
	static void* s__aeabi_atexit(void*);
};


int get_prop(char* name,char* value);
extern "C" void* sys_malloc(size_t size);
extern "C" void sys_free(void* ptr);

#endif