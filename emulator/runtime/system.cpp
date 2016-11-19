#include "stdio.h"
#include "string.h"

typedef struct _sys_prop{
	char* name;
	char* value;
}sys_prop;


sys_prop g_sys_prop[] =
{
	{"ro.build.version.release","4.4.4"},
	{"ro.product.cpu.abi","armeabi"},
};

int get_prop(char* name,char* value)
{
	int n = 0;
	for(int i = 0; i < sizeof(g_sys_prop)/sizeof(g_sys_prop[0]); i++)
	{
		if(strcmp(g_sys_prop[i].name,name) == 0)
		{
			strcpy(value,g_sys_prop[i].value);
			break;
		}
	}

	return strlen(value);
}