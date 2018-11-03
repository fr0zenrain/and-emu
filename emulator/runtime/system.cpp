#include "stdio.h"
#include "string.h"

typedef struct _sys_prop{
	const char* name;
    const char* value;
}sys_prop;


sys_prop g_sys_prop[] =
{
	{"ro.build.version.release","4.4.4"},
	{"ro.product.cpu.abi","armeabi"},
	{"ro.build.version.sdk","22"},
	{"ro.product.brand","Nexus 5"},
};

sys_prop g_sys_env[] =
{
    {"OAT_VERSION","75"},
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

int get_var(char* name,char* value)
{
	int n = 0;
	for(int i = 0; i < sizeof(g_sys_env)/sizeof(g_sys_env[0]); i++)
	{
		if(strcmp(g_sys_env[i].name,name) == 0)
		{
			strcpy(value,g_sys_env[i].value);
			break;
		}
	}

	return strlen(value);
}