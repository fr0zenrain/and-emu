#ifdef _MSC_VER
//#include "windows.h"
#include "dirent_win.h"
#include "stdlib.h"
#include "stdio.h"
#else
#include <dirent.h>
#include "dirent_win.h"
#endif
#include <stdlib.h>
#include "string.h"
#include "runtime.h"

extern uc_engine* g_uc;

#ifdef _MSC_VER
static HANDLE hFind;  

DIRX *opendir_(const char *name)
{  
	DIRX *dir;  
	WIN32_FIND_DATA FindData;  
	char namebuf[512];  

	sprintf(namebuf, "%s\\*.*",name);  

	hFind = FindFirstFile(namebuf, &FindData );   
	if(hFind == INVALID_HANDLE_VALUE)   
	{  
		//printf("FindFirstFile failed (%d)\n", GetLastError());  
		return 0;  
	}   

	dir = (DIRX *)malloc(sizeof(DIRX));
	if(!dir)  
	{
		return 0;  
	} 
	memset(dir,0,sizeof(DIRX));
	return dir;  
}  

struct direntx *readdir_(DIRX *d)
{  
	int i; 
	unsigned int addr = 0;
	direntx* dirent = (direntx*)sys_malloc(sizeof(direntx));  
	BOOL bf;  
	unsigned int value = 0;
	WIN32_FIND_DATA FileData;  
	if(!d)  
	{  
		return 0;  
	}  

	bf = FindNextFile(hFind,&FileData);  
	//fail or end  
	if(!bf)  
	{  
		return 0;  
	}

	for(i = 0; i < 256; i++)  
	{  
		addr = (unsigned int)dirent + offsetof(direntx,d_name);
		uc_mem_write(g_uc,addr+i,&FileData.cFileName[i],1);

		if(FileData.cFileName[i] == '\0') 
			break;  
	}  

	value = FileData.nFileSizeLow;  
	addr = (unsigned int)dirent + offsetof(direntx,d_reclen);
	uc_mem_write(g_uc,addr,&value,2);

	//check there is file or directory  
	if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
	{  
		value = 2;  
		addr = (unsigned int)dirent + offsetof(direntx,d_type);
		uc_mem_write(g_uc,addr,&value,1);
	}  
	else  
	{  
		value = 1;  
		addr = (unsigned int)dirent + offsetof(direntx,d_type);
		uc_mem_write(g_uc,addr,&value,1);
	}  


	return dirent;  
}  

int closedir_(DIRX *d)
{  
	if(!d) 
		return -1;  
	FindClose(hFind);
	hFind = 0;
	free(d);  
	return 0;  
}

#else

DIRX * opendir_(const char *name)
{
    DIRX* dir = (DIRX*)malloc(sizeof(DIR));
    if(!dir){
        return 0;
    }

    memset(dir, 0, sizeof(DIRX));

    DIR* d = opendir(name);
    dir->dd_fd = (int)d;
    return dir;
}

direntx *readdir_(DIRX *d)
{
    DIR* dir = (DIR*)d->dd_fd;
    dirent* x = readdir((DIR*)dir);
    if(x == 0)
    {
        return 0;
    }

    int value = 0;

    direntx* ent = (direntx*)sys_malloc(sizeof(direntx));
    for(int i = 0; i < sizeof(direntx); i++)
    {
        uc_mem_write(g_uc,(unsigned int)ent+i,&value, 1);
    }

    unsigned int addr = (unsigned int)ent + offsetof(direntx, d_name);
    for(int i = 0; i < 256; i++)
    {
        uc_mem_write(g_uc,addr+i,&x->d_name[i], 1);
    }

    addr = (unsigned int)ent + offsetof(direntx, d_type);
    uc_mem_write(g_uc,addr,&x->d_type, 1);

    addr = (unsigned int)ent + offsetof(direntx, d_ino);
    uc_mem_write(g_uc,addr,&x->d_ino, 8);

    return ent;
}

int closedir_(DIRX *d)
{
    DIR* dir = (DIR*)d->dd_fd;
    closedir((DIR*)d->dd_fd);
    return 0;
}

#endif