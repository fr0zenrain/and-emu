#ifndef __DIRENT_WIN_H
#define __DIRENT_WIN_H

#ifdef _MSC_VER
typedef struct _dirdesc {
	int     dd_fd;      /** file descriptor associated with directory */
	long    dd_loc;     /** offset in current buffer */
	long    dd_size;    /** amount of data returned by getdirentries */
	char    *dd_buf;    /** data buffer */
	int     dd_len;     /** size of data buffer */
	long    dd_seek;    /** magic cookie returned by getdirentries */
} DIRX;

# define __dirfd(dp)    ((dp)->dd_fd)
#include <sys/types.h>

#pragma pack(1)
struct direntx
{
	unsigned __int64 d_ino;              /* inode number*/
	__int64 d_off;             /* offset to this dirent*/
	unsigned short d_reclen; /* length of this d_name*/
	unsigned char d_type;    /* the type of d_name*/
	char d_name[256];          /* file name (null-terminated)*/
};
#pragma pack()

#else

typedef struct __dirstream
{
    int dd_fd; /* `struct hurd_fd' pointer for descriptor.   */
    char *__data; /* Directory block.   */
    int __entry_data; /* Entry number `__data' corresponds to.   */
    char *__ptr; /* Current pointer into the block.   */
    int __entry_ptr; /* Entry number `__ptr' corresponds to.   */
    int __allocation; /* Space allocated for the block.   */
    int __size; /* Total valid data in the block.   */
    int lock; /* Mutex lock for this structure.   */
}DIRX;


struct direntx {
	unsigned long long  d_ino;
    long long      d_off;
	unsigned short   d_reclen;
	unsigned char    d_type;
	char             d_name[256];
};

#endif

DIRX *opendir_ (const char *name);
struct direntx *readdir_ (DIRX *);
int closedir_ (DIRX *);

#endif