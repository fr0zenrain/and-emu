#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdio.h>
#if !defined(_MSC_VER) && !defined(__MACH__)
#include <pthread.h>
#endif
typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
typedef unsigned int uint32_t ;
typedef	unsigned long long	uint64_t;

#ifdef __MACH__
typedef	char *	caddr_t;
typedef unsigned long size_t;
typedef long long off_t;
typedef int pid_t;
typedef int dev_t;
typedef unsigned long long ino_t;

#else
typedef int ssize_t;
typedef unsigned int size_t;
typedef long off_t;

#ifdef _MSC_VER
typedef unsigned int pid_t;
typedef unsigned int dev_t;
typedef unsigned short ino_t;

#endif

#endif


typedef unsigned int uid_t;
typedef unsigned int gid_t;
#define _SOCKLEN_T
typedef int socklen_t;

typedef long pthread_t;



#ifdef _MSC_VER
#define __defaultcall __stdcall
#else
#define __defaultcall 
#endif


typedef void ( *sighandler_t)(int);
typedef void* (__defaultcall *start_rtn)(void*);
typedef int cmp_t(const void *, const void *);

#if defined(_MSC_VER) || defined(__MACH__)

#ifdef _MSC_VER
#ifdef WINCE
typedef unsigned long  time_t;
#else
typedef long long  time_t;
#endif
#else
typedef long time_t;
#endif

#ifdef __MACH__
#define _PTHREAD_T
typedef _opaque_pthread_attr_t	pthread_attr_t;
#else

typedef uint32_t in_addr_t;

typedef struct {
	uint32_t flags;
	void* stack_base;
	size_t stack_size;
	size_t guard_size;
	int sched_policy;
	int sched_priority;
#ifdef __LP64__
	char __reserved[16];
#endif
} pthread_attr_t;

struct iovec {
	void*iov_base;
	size_t iov_len;
};


struct msghdr {
	void * msg_name;				//0x0
	int msg_namelen;				//0x4
	struct iovec * msg_iov;			//0x8
	size_t msg_iovlen;		//0xc
	void * msg_control;				//0x10
	size_t msg_controllen;	//0x14
	unsigned msg_flags;				//0x18
};

struct mstat {
	int         st_mode;    // file type & mode(permissions)
	ino_t          st_ino;     // i-node number(serial number)
	dev_t          st_dev;     // device number(filesystem)
	dev_t          st_rdev;    // device number for specials files
	int   st_nlink;        // number of links
	uid_t          st_uid;     // user ID of owner
	gid_t          st_gid;     // group ID of owner
	off_t          st_size;    // size in bytes, for regular files
	time_t         st_atime;   // time of last access
	time_t         st_mtime;   // time of last modification
	time_t         st_ctime;   // time of last file status change
	long           st_blksize; // best I/O block size
	long           st_blocks;  // number of      512-byte blocks allocated
};

typedef unsigned short mode_t;

#ifndef _MSC_VER
#define __SOCK_SIZE__ 16

struct in_addr {
	union {
		struct{unsigned char s_b1,s_b2,s_b3,s_b4;} S_un_b;
		struct{unsigned short s_w1,s_w2;} S_un_w;
		unsigned long S_addr;
	} S_un;
};

#define s_addr  S_un.S_addr

struct sockaddr_in {
	unsigned short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	//WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS
	unsigned char __pad[__SOCK_SIZE__ - sizeof(short int) - sizeof(unsigned short int) - sizeof(struct in_addr)];
};

#endif

#define _IOC_NRBITS 8
#define _IOC_TYPEBITS 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef _IOC_SIZEBITS
#define _IOC_SIZEBITS 14
#endif
#ifndef _IOC_DIRBITS
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define _IOC_DIRBITS 2
#endif
#define _IOC_NRMASK ((1 << _IOC_NRBITS) - 1)
#define _IOC_TYPEMASK ((1 << _IOC_TYPEBITS) - 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define _IOC_SIZEMASK ((1 << _IOC_SIZEBITS) - 1)
#define _IOC_DIRMASK ((1 << _IOC_DIRBITS) - 1)
#define _IOC_NRSHIFT 0
#define _IOC_TYPESHIFT (_IOC_NRSHIFT + _IOC_NRBITS)

#define _IOC_SIZESHIFT (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_DIRSHIFT (_IOC_SIZESHIFT + _IOC_SIZEBITS)
#ifndef _IOC_NONE
#define _IOC_NONE 0U
#endif

#ifndef _IOC_WRITE
#define _IOC_WRITE 1U
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef _IOC_READ
#define _IOC_READ 2U
#endif

#define _IOC(dir,type,nr,size) (((dir) << _IOC_DIRSHIFT) | ((type) << _IOC_TYPESHIFT) | ((nr) << _IOC_NRSHIFT) | ((size) << _IOC_SIZESHIFT))
#define _IOC_TYPECHECK(t) (sizeof(t))

#undef _IO
#define _IO(type,nr) _IOC(_IOC_NONE, (type), (nr), 0)
#undef _IOR
#define _IOR(type,nr,size) _IOC(_IOC_READ, (type), (nr), (_IOC_TYPECHECK(size)))
#undef _IOW
#define _IOW(type,nr,size) _IOC(_IOC_WRITE, (type), (nr), (_IOC_TYPECHECK(size)))

/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define _IOWR(type,nr,size) _IOC(_IOC_READ | _IOC_WRITE, (type), (nr), (_IOC_TYPECHECK(size)))




#define SIG_DFL 0
#define SIG_IGN 1
#define SIG_ERR -1

#define SIGHUP 1
#define SIGINT 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIGPOLL SIGIO
#define SIGPWR 30
#define SIGSYS 31
#define SIGUNUSED 31


#define PTRACE_TRACEME 0
#define PTRACE_PEEKTEXT 1
#define PTRACE_PEEKDATA 2
#define PTRACE_PEEKUSR 3
#define PTRACE_POKETEXT 4

#define WNOHANG 0x00000001
#define WUNTRACED 0x00000002

#define AF_INET     2
#define PF_INET		2
#define SOCK_STREAM 1
#define SOCK_DGRAM	2

#define IPPROTO_ICMP 1 	
#define IPPROTO_TCP  6

#define SOL_IP 0
#define SOL_TCP 6
#define SOL_UDP 17

#define IP_TOS 1
#define IP_TTL 2

#define SOL_SOCKET	1
#define SO_REUSEADDR	2

#define FIONREAD	0x541B
#define AF_UNSPEC	0

#endif

//int pthread_create(pthread_t * tidp,const pthread_attr_t *restrict_attr,start_rtn rtn,void * arg);
#endif

#define MAP_RENAME 0x020

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#ifndef PAGE_MASK
#define PAGE_MASK (~(PAGE_SIZE - 1))
#endif

#ifndef PAGE_ALIGN
#define PAGE_ALIGN(addr) (((addr)+PAGE_SIZE-1)&PAGE_MASK)
#endif

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

#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'

#ifndef LONG_MAX
#define LONG_MAX	((long)(~0UL>>1))
#endif
#ifndef LONG_MIN
#define LONG_MIN	(-LONG_MAX - 1)
#endif

#define ULONG_MAX     0xffffffffUL  /* maximum unsigned long value */

#ifndef ULLONG_MAX
#define ULLONG_MAX	(~0ULL)
#endif

#ifdef _MSC_VER
/*
struct timespec {
	long	tv_sec;			
	long	tv_nsec;		
};*/

#ifndef RLIMIT_NOFILE
#define RLIMIT_NOFILE		7	/* max number of open files */
#endif

struct rlimit {
	unsigned long	rlim_cur;
	unsigned long	rlim_max;
};

#undef offsetof
#define offsetof(a,b) ((int)(&(((a*)(0))->b)))

#endif


#endif
