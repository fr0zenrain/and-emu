#ifndef __WIN_H__
#define __WIN_H__
#include "type.h"

#define INVALID_HANDLE_VALUE -1
#define WINAPI __stdcall 

#define PAGE_NOACCESS          0x01     
#define PAGE_READONLY          0x02     
#define PAGE_READWRITE         0x04     
#define PAGE_WRITECOPY         0x08     
#define PAGE_EXECUTE           0x10     
#define PAGE_EXECUTE_READ      0x20     
#define PAGE_EXECUTE_READWRITE 0x40     
#define PAGE_EXECUTE_WRITECOPY 0x80     
#define PAGE_GUARD            0x100     
#define PAGE_NOCACHE          0x200     
#define PAGE_WRITECOMBINE     0x400     

#define SECTION_QUERY                0x0001
#define SECTION_MAP_WRITE            0x0002
#define SECTION_MAP_READ             0x0004
#define SECTION_MAP_EXECUTE          0x0008
#define SECTION_EXTEND_SIZE          0x0010
#define SECTION_MAP_EXECUTE_EXPLICIT 0x0020 // not included in SECTION_ALL_ACCESS

#define FILE_MAP_COPY       SECTION_QUERY
#define FILE_MAP_WRITE      SECTION_MAP_WRITE
#define FILE_MAP_READ       SECTION_MAP_READ
#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS
#define FILE_MAP_EXECUTE    SECTION_MAP_EXECUTE_EXPLICIT    // not included in FILE_MAP_ALL_ACCESS

#define MEM_COMMIT           0x1000     
#define MEM_RESERVE          0x2000      
#define MEM_RELEASE          0x8000     

typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned long SIZE_T;
typedef void* LPVOID;
typedef const char *LPCSTR;

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

extern "C" DWORD WINAPI UnmapViewOfFile(const void* lpBaseAddress);
extern "C" DWORD WINAPI GetLastError(void);
extern "C" BOOL WINAPI CloseHandle(HANDLE hObject);
extern "C" HANDLE WINAPI CreateFileMappingA( HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes,
						 DWORD flProtect,  DWORD dwMaximumSizeHigh,DWORD dwMaximumSizeLow, LPCSTR lpName);
extern "C" LPVOID WINAPI MapViewOfFile( HANDLE hFileMappingObject,DWORD dwDesiredAccess, 
				DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap);
extern "C" BOOL WINAPI VirtualProtect( LPVOID lpAddress,  SIZE_T dwSize,  DWORD flNewProtect, DWORD* lpflOldProtect );
extern "C" BOOL WINAPI VirtualFree( LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
extern "C" LPVOID WINAPI VirtualAlloc( LPVOID lpAddress,SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);



#endif