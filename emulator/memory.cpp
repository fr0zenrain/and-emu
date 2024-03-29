#include "stdlib.h"
#include "linker.h"
#ifndef WINCE
#include "errno.h"
#endif

#ifdef _MSC_VER
#include "win.h"
#ifndef WINCE
#include "io.h"
#else
int errno =0;
#endif
#else
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#endif

#ifdef _MSC_VER

static int __map_mman_error(const uint32_t err, const int deferr)
{
	if (err == 0)
		return 0;
	//TODO: implement
	return err;
}

static DWORD __map_mmap_prot_page(const int prot)
{
	DWORD protect = 0;

	if (prot == PROT_NONE)
		return protect;

	if ((prot & PROT_EXEC) != 0)
	{
		protect = ((prot & PROT_WRITE) != 0) ? 
PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
	}
	else
	{
		protect = ((prot & PROT_WRITE) != 0) ?
PAGE_READWRITE : PAGE_READONLY;
	}

	return protect;
}

static uint32_t __map_mmap_prot_file(const int prot)
{
	uint32_t desiredAccess = 0;

	if (prot == PROT_NONE)
		return desiredAccess;

	if ((prot & PROT_READ) != 0)
		desiredAccess |= FILE_MAP_READ;
	if ((prot & PROT_WRITE) != 0)
		desiredAccess |= FILE_MAP_WRITE;
	if ((prot & PROT_EXEC) != 0)
		desiredAccess |= FILE_MAP_EXECUTE;

	return desiredAccess;
}

#endif

void *s_mmap(void *start, size_t length, int prot, int flags,int fd, int offset)
{
#ifdef _MSC_VER
	HANDLE fm = 0, h =0;

	void * map = MAP_FAILED;

	if (length == 0 
		/* Unsupported flag combinations */
		//|| (flags & MAP_FIXED) != 0
		/* Usupported protection combinations */
		|| prot == PROT_EXEC)
	{
		return MAP_FAILED;
	}

	const DWORD dwFileOffsetLow = (sizeof(off_t) <= sizeof(DWORD)) ? 
		(DWORD)offset : (DWORD)(offset & 0xFFFFFFFFL);
	const DWORD dwFileOffsetHigh = (sizeof(off_t) <= sizeof(DWORD)) ?
		(DWORD)0 : (DWORD)((offset >> 32) & 0xFFFFFFFFL);
	const DWORD protect = __map_mmap_prot_page(prot);
	const DWORD desiredAccess = __map_mmap_prot_file(prot);

	const off_t maxSize = offset + (off_t)length;

	const DWORD dwMaxSizeLow = (sizeof(off_t) <= sizeof(DWORD)) ? 
		(DWORD)maxSize : (DWORD)(maxSize & 0xFFFFFFFFL);
	const DWORD dwMaxSizeHigh = (sizeof(off_t) <= sizeof(DWORD)) ?
		(DWORD)0 : (DWORD)((maxSize >> 32) & 0xFFFFFFFFL);
	
	if((int)fd == -1 || (flags & MAP_FIXED))
	{
		if(fd == -1 && prot == 0)
			map = VirtualAlloc(0,length,MEM_COMMIT,PAGE_READWRITE);
		else
		{
			if(start == 0)
			{
				map = VirtualAlloc(0,length,MEM_COMMIT,PAGE_READWRITE);
			}
		}

		errno = __map_mman_error(GetLastError(), -1);
		if( map || start)
		{
			if(map == MAP_FAILED && start)
			{
				if(fd != -1)
				{
					lseek(fd,offset,SEEK_SET);
					read(fd,start,length);
				}
				return start;
			}
			else
			{
				if(fd != -1)
					read(fd,map,length);
			}
		}
		return map;
	}
#ifndef WINCE
	h = ((flags & MAP_ANONYMOUS) == 0) ? (HANDLE)_get_osfhandle(fd) : (HANDLE)INVALID_HANDLE_VALUE;
#endif

	if ((flags & MAP_ANONYMOUS) == 0 && (int)h == -1)
	{
		return MAP_FAILED;
	}
#ifndef WINCE
	fm = CreateFileMappingA(h, NULL, protect, dwMaxSizeHigh, dwMaxSizeLow, NULL);
#endif
	if (fm == NULL)
	{
		errno = __map_mman_error(GetLastError(), -1);
		return MAP_FAILED;
	}

	map = MapViewOfFile(fm, desiredAccess, dwFileOffsetHigh, dwFileOffsetLow, length);

	CloseHandle(fm);

	if (map == NULL)
	{
		errno = __map_mman_error(GetLastError(), -1);
		return MAP_FAILED;
	}

	return map;
#else
	return mmap(start,length,prot,flags,fd,offset);
#endif
}

int s_mprotect( void *start, size_t len, int prot)
{
#ifdef _MSC_VER
	DWORD newProtect = __map_mmap_prot_page(prot);
	DWORD oldProtect = 0;

	if (VirtualProtect(start, len, newProtect, &oldProtect))
		return 0;

	__map_mman_error(GetLastError(), -1);

	return -1;
#else
	return mprotect(start,len,prot);
#endif
}

int s_msync(void *addr, size_t length, int flags)
{
#ifdef _MSC_VER
	return 0;
#else
	return msync(addr,length,flags);
#endif
}


int s_munmap(void *start, size_t length)
{
#ifdef _MSC_VER
	if (UnmapViewOfFile(start))
		return 0;
	__map_mman_error(GetLastError(), -1);
	return -1;
#else
	return munmap(start,length);
#endif
}
