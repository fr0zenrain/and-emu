#include "jvm.h"
#include "../runtime/runtime.h"
#include "../../include/unicorn/unicorn.h"

extern uc_engine* g_uc;
unsigned int g_JNIEnv_addr = 0;

int GetEnv()
{
	int env = JVM_INTERFACE_ADDRESS;
	int jvm = libc::get_r0();
	int ret = 0;
	unsigned int env_ptr = libc::get_r1();
	int version = libc::get_r2();
	if (version < JNI_VERSION_1_1 || version > JNI_VERSION_1_6) {
		return JNI_EVERSION;
	}

	if(env_ptr)
	{
		uc_mem_write(g_uc,env_ptr,&g_JNIEnv_addr,4);
	}

	unsigned int lr = libc::get_lr();
	if(lr &1)
		lr -= 1;

#ifdef _MSC_VER
	printf("GetEnv() ->0x%x\n",env);
#else
	printf(RED "GetEnv() ->0x%x\n" RESET,env);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return JNI_OK;
}

int GetVersion()
{
	return JNI_OK;
}

int DefineClass()
{
	return JNI_OK;
}

int FindClass()
{
	int ret = 1;
	uc_err err;
	char buffer[256] ={0};
	unsigned int env = libc::get_r0();
	int name_addr = libc::get_r1();
	
	if(name_addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,name_addr+i,&buffer[i],1);
			if(buffer[i] == 0)
				break;
		}
	}

	unsigned int lr = libc::get_lr();
	if(lr &1)
		lr -= 1;

#ifdef _MSC_VER
	printf("FindClass(\"%s\")\n",buffer);
#else
	printf(RED "FindClass(\"%s\") ->0x%\n" RESET, buffer);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return JNI_OK;
}


int FromReflectedMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FromReflectedMethod(\"%s\")\n",buffer);
#else
	printf(RED "FromReflectedMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int FromReflectedField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FromReflectedField(\"%s\")\n",buffer);
#else
	printf(RED "FromReflectedField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ToReflectedMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ToReflectedMethod(\"%s\")\n",buffer);
#else
	printf(RED "ToReflectedMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetSuperclass() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetSuperclass(\"%s\")\n",buffer);
#else
	printf(RED "GetSuperclass(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsAssignableFrom() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsAssignableFrom(\"%s\")\n",buffer);
#else
	printf(RED "IsAssignableFrom(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ToReflectedField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ToReflectedField(\"%s\")\n",buffer);
#else
	printf(RED "ToReflectedField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int Throw() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("Throw(\"%s\")\n",buffer);
#else
	printf(RED "Throw(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ThrowNew() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ThrowNew(\"%s\")\n",buffer);
#else
	printf(RED "ThrowNew(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionOccurred() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionOccurred(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionOccurred(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionDescribe() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionDescribe(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionDescribe(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionClear() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionClear(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionClear(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int FatalError() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FatalError(\"%s\")\n",buffer);
#else
	printf(RED "FatalError(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int PushLocalFrame() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("PushLocalFrame(\"%s\")\n",buffer);
#else
	printf(RED "PushLocalFrame(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int PopLocalFrame() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("PopLocalFrame(\"%s\")\n",buffer);
#else
	printf(RED "PopLocalFrame(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "NewGlobalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("DeleteGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "DeleteGlobalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteLocalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("DeleteLocalRef(\"%s\")\n",buffer);
#else
	printf(RED "DeleteLocalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsSameObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsSameObject(\"%s\")\n",buffer);
#else
	printf(RED "IsSameObject(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewLocalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewLocalRef(\"%s\")\n",buffer);
#else
	printf(RED "NewLocalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int EnsureLocalCapacity() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("EnsureLocalCapacity(\"%s\")\n",buffer);
#else
	printf(RED "EnsureLocalCapacity(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int AllocObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("AllocObject(\"%s\")\n",buffer);
#else
	printf(RED "AllocObject(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObject(\"%s\")\n",buffer);
#else
	printf(RED "NewObject(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectV(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectA(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectClass() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectClass(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectClass(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsInstanceOf() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsInstanceOf(\"%s\")\n",buffer);
#else
	printf(RED "IsInstanceOf(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetMethodID() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetMethodID(\"%s\")\n",buffer);
#else
	printf(RED "GetMethodID(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFieldID() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFieldID(\"%s\")\n",buffer);
#else
	printf(RED "GetFieldID(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectField(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetByteField(\"%s\")\n",buffer);
#else
	printf(RED "GetByteField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharField(\"%s\")\n",buffer);
#else
	printf(RED "GetCharField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortField(\"%s\")\n",buffer);
#else
	printf(RED "GetShortField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntField(\"%s\")\n",buffer);
#else
	printf(RED "GetIntField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongField(\"%s\")\n",buffer);
#else
	printf(RED "GetLongField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatField(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetObjectField(\"%s\")\n",buffer);
#else
	printf(RED "SetObjectField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "SetBooleanField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetByteField(\"%s\")\n",buffer);
#else
	printf(RED "SetByteField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetCharField(\"%s\")\n",buffer);
#else
	printf(RED "SetCharField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetShortField(\"%s\")\n",buffer);
#else
	printf(RED "SetShortField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetIntField(\"%s\")\n",buffer);
#else
	printf(RED "SetIntField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetLongField(\"%s\")\n",buffer);
#else
	printf(RED "SetLongField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetFloatField(\"%s\")\n",buffer);
#else
	printf(RED "SetFloatField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "SetDoubleField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticMethodID() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticMethodID(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticMethodID(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethod(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethodV(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethodA(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticFieldID() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticFieldID(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticFieldID(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticObjectField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticObjectField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticBooleanField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticByteField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticByteField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticCharField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticCharField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticShortField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticShortField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticIntField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticIntField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticLongField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticLongField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticFloatField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticFloatField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticDoubleField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticObjectField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticObjectField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticBooleanField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticByteField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticByteField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticCharField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticCharField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticShortField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticShortField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticIntField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticIntField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticLongField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticLongField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticFloatField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticFloatField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticDoubleField(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewString() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewString(\"%s\")\n",buffer);
#else
	printf(RED "NewString(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringLength(\"%s\")\n",buffer);
#else
	printf(RED "GetStringLength(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringChars(\"%s\")\n",buffer);
#else
	printf(RED "GetStringChars(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseStringChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringChars(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringChars(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewStringUTF() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewStringUTF(\"%s\")\n",buffer);
#else
	printf(RED "NewStringUTF(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringUTFLength(\"%s\")\n",buffer);
#else
	printf(RED "GetStringUTFLength(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringUTFChars(\"%s\")\n",buffer);
#else
	printf(RED "GetStringUTFChars(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseStringUTFChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringUTFChars(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringUTFChars(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetArrayLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetArrayLength(\"%s\")\n",buffer);
#else
	printf(RED "GetArrayLength(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectArray(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectArrayElement() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectArrayElement(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectArrayElement(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetObjectArrayElement() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetObjectArrayElement(\"%s\")\n",buffer);
#else
	printf(RED "SetObjectArrayElement(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewBooleanArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewBooleanArray(\"%s\")\n",buffer);
#else
	printf(RED "NewBooleanArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewByteArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewByteArray(\"%s\")\n",buffer);
#else
	printf(RED "NewByteArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewCharArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewCharArray(\"%s\")\n",buffer);
#else
	printf(RED "NewCharArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewShortArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewShortArray(\"%s\")\n",buffer);
#else
	printf(RED "NewShortArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewIntArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewIntArray(\"%s\")\n",buffer);
#else
	printf(RED "NewIntArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewLongArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewLongArray(\"%s\")\n",buffer);
#else
	printf(RED "NewLongArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewFloatArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewFloatArray(\"%s\")\n",buffer);
#else
	printf(RED "NewFloatArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewDoubleArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewDoubleArray(\"%s\")\n",buffer);
#else
	printf(RED "NewDoubleArray(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetByteArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetByteArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetCharArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetShortArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetIntArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetLongArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseBooleanArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseBooleanArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseBooleanArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseByteArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseByteArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseByteArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseCharArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseCharArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseCharArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseShortArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseShortArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseShortArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseIntArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseIntArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseIntArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseLongArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseLongArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseLongArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseFloatArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseFloatArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseFloatArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseDoubleArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseDoubleArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseDoubleArrayElements(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetByteArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetByteArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetCharArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetShortArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetIntArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetLongArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetBooleanArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetBooleanArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetBooleanArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetByteArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetByteArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetByteArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetCharArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetCharArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetCharArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetShortArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetShortArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetShortArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetIntArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetIntArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetIntArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetLongArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetLongArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetLongArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetFloatArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetFloatArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetFloatArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetDoubleArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetDoubleArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetDoubleArrayRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int RegisterNatives() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("RegisterNatives(\"%s\")\n",buffer);
#else
	printf(RED "RegisterNatives(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int UnregisterNatives() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("UnregisterNatives(\"%s\")\n",buffer);
#else
	printf(RED "UnregisterNatives(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int MonitorEnter() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("MonitorEnter(\"%s\")\n",buffer);
#else
	printf(RED "MonitorEnter(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int MonitorExit() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("MonitorExit(\"%s\")\n",buffer);
#else
	printf(RED "MonitorExit(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetJavaVM() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetJavaVM(\"%s\")\n",buffer);
#else
	printf(RED "GetJavaVM(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetStringRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringUTFRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetStringUTFRegion(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetPrimitiveArrayCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetPrimitiveArrayCritical(\"%s\")\n",buffer);
#else
	printf(RED "GetPrimitiveArrayCritical(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleasePrimitiveArrayCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleasePrimitiveArrayCritical(\"%s\")\n",buffer);
#else
	printf(RED "ReleasePrimitiveArrayCritical(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringCritical(\"%s\")\n",buffer);
#else
	printf(RED "GetStringCritical(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseStringCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringCritical(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringCritical(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewWeakGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewWeakGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "NewWeakGlobalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteWeakGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("DeleteWeakGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "DeleteWeakGlobalRef(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionCheck() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionCheck(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionCheck(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewDirectByteBuffer() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewDirectByteBuffer(\"%s\")\n",buffer);
#else
	printf(RED "NewDirectByteBuffer(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDirectBufferAddress() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDirectBufferAddress(\"%s\")\n",buffer);
#else
	printf(RED "GetDirectBufferAddress(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDirectBufferCapacity() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDirectBufferCapacity(\"%s\")\n",buffer);
#else
	printf(RED "GetDirectBufferCapacity(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectRefType() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = libc::get_r0(); 
	unsigned int lr = libc::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectRefType(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectRefType(\"%s\") ->0x%\n" RESET, buffer,ret); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}



func_info g_invoke_func[] ={
	0,
	0,
	0,
	0,
	0,
	0,
	GetEnv,
	0,
	0
};

func_info g_native_func[] ={
	0,
	0,
	0,
	0,

	GetVersion,

	DefineClass,
	FindClass,//6

	FromReflectedMethod,
	FromReflectedField,
	ToReflectedMethod,

	GetSuperclass,
	IsAssignableFrom,

	ToReflectedField,

	Throw,
	ThrowNew,
	ExceptionOccurred,
	ExceptionDescribe,
	ExceptionClear,//0x11
	FatalError,

	PushLocalFrame,
	PopLocalFrame,

	NewGlobalRef,
	DeleteGlobalRef,
	DeleteLocalRef,
	IsSameObject,
	NewLocalRef,
	EnsureLocalCapacity,

	AllocObject,
	NewObject,
	NewObjectV,
	NewObjectA,

	GetObjectClass,
	IsInstanceOf,

	GetMethodID,

	CallObjectMethod,
	CallObjectMethodV,
	CallObjectMethodA,
	CallBooleanMethod,
	CallBooleanMethodV,
	CallBooleanMethodA,
	CallByteMethod,
	CallByteMethodV,
	CallByteMethodA,
	CallCharMethod,
	CallCharMethodV,
	CallCharMethodA,
	CallShortMethod,
	CallShortMethodV,
	CallShortMethodA,
	CallIntMethod,
	CallIntMethodV,
	CallIntMethodA,
	CallLongMethod,
	CallLongMethodV,
	CallLongMethodA,
	CallFloatMethod,
	CallFloatMethodV,
	CallFloatMethodA,
	CallDoubleMethod,
	CallDoubleMethodV,
	CallDoubleMethodA,
	CallVoidMethod,
	CallVoidMethodV,
	CallVoidMethodA,

	CallNonvirtualObjectMethod,
	CallNonvirtualObjectMethodV,
	CallNonvirtualObjectMethodA,
	CallNonvirtualBooleanMethod,
	CallNonvirtualBooleanMethodV,
	CallNonvirtualBooleanMethodA,
	CallNonvirtualByteMethod,
	CallNonvirtualByteMethodV,
	CallNonvirtualByteMethodA,
	CallNonvirtualCharMethod,
	CallNonvirtualCharMethodV,
	CallNonvirtualCharMethodA,
	CallNonvirtualShortMethod,
	CallNonvirtualShortMethodV,
	CallNonvirtualShortMethodA,
	CallNonvirtualIntMethod,
	CallNonvirtualIntMethodV,
	CallNonvirtualIntMethodA,
	CallNonvirtualLongMethod,
	CallNonvirtualLongMethodV,
	CallNonvirtualLongMethodA,
	CallNonvirtualFloatMethod,
	CallNonvirtualFloatMethodV,
	CallNonvirtualFloatMethodA,
	CallNonvirtualDoubleMethod,
	CallNonvirtualDoubleMethodV,
	CallNonvirtualDoubleMethodA,
	CallNonvirtualVoidMethod,
	CallNonvirtualVoidMethodV,
	CallNonvirtualVoidMethodA,

	GetFieldID,

	GetObjectField,
	GetBooleanField,
	GetByteField,
	GetCharField,
	GetShortField,
	GetIntField,
	GetLongField,
	GetFloatField,
	GetDoubleField,
	SetObjectField,
	SetBooleanField,
	SetByteField,
	SetCharField,
	SetShortField,
	SetIntField,
	SetLongField,
	SetFloatField,
	SetDoubleField,

	GetStaticMethodID,

	CallStaticObjectMethod,
	CallStaticObjectMethodV,
	CallStaticObjectMethodA,
	CallStaticBooleanMethod,
	CallStaticBooleanMethodV,
	CallStaticBooleanMethodA,
	CallStaticByteMethod,
	CallStaticByteMethodV,
	CallStaticByteMethodA,
	CallStaticCharMethod,
	CallStaticCharMethodV,
	CallStaticCharMethodA,
	CallStaticShortMethod,
	CallStaticShortMethodV,
	CallStaticShortMethodA,
	CallStaticIntMethod,
	CallStaticIntMethodV,
	CallStaticIntMethodA,
	CallStaticLongMethod,
	CallStaticLongMethodV,
	CallStaticLongMethodA,
	CallStaticFloatMethod,
	CallStaticFloatMethodV,
	CallStaticFloatMethodA,
	CallStaticDoubleMethod,
	CallStaticDoubleMethodV,
	CallStaticDoubleMethodA,
	CallStaticVoidMethod,
	CallStaticVoidMethodV,
	CallStaticVoidMethodA,

	GetStaticFieldID,

	GetStaticObjectField,
	GetStaticBooleanField,
	GetStaticByteField,
	GetStaticCharField,
	GetStaticShortField,
	GetStaticIntField,
	GetStaticLongField,
	GetStaticFloatField,
	GetStaticDoubleField,

	SetStaticObjectField,
	SetStaticBooleanField,
	SetStaticByteField,
	SetStaticCharField,
	SetStaticShortField,
	SetStaticIntField,
	SetStaticLongField,
	SetStaticFloatField,
	SetStaticDoubleField,

	NewString,

	GetStringLength,
	GetStringChars,
	ReleaseStringChars,

	NewStringUTF,
	GetStringUTFLength,
	GetStringUTFChars,
	ReleaseStringUTFChars,

	GetArrayLength,
	NewObjectArray,
	GetObjectArrayElement,
	SetObjectArrayElement,

	NewBooleanArray,
	NewByteArray,
	NewCharArray,
	NewShortArray,
	NewIntArray,
	NewLongArray,
	NewFloatArray,
	NewDoubleArray,

	GetBooleanArrayElements,
	GetByteArrayElements,
	GetCharArrayElements,
	GetShortArrayElements,
	GetIntArrayElements,
	GetLongArrayElements,
	GetFloatArrayElements,
	GetDoubleArrayElements,

	ReleaseBooleanArrayElements,
	ReleaseByteArrayElements,
	ReleaseCharArrayElements,
	ReleaseShortArrayElements,
	ReleaseIntArrayElements,
	ReleaseLongArrayElements,
	ReleaseFloatArrayElements,
	ReleaseDoubleArrayElements,

	GetBooleanArrayRegion,
	GetByteArrayRegion,
	GetCharArrayRegion,
	GetShortArrayRegion,
	GetIntArrayRegion,
	GetLongArrayRegion,
	GetFloatArrayRegion,
	GetDoubleArrayRegion,
	SetBooleanArrayRegion,
	SetByteArrayRegion,
	SetCharArrayRegion,
	SetShortArrayRegion,
	SetIntArrayRegion,
	SetLongArrayRegion,
	SetFloatArrayRegion,
	SetDoubleArrayRegion,

	RegisterNatives,
	UnregisterNatives,

	MonitorEnter,
	MonitorExit,

	GetJavaVM,

	GetStringRegion,
	GetStringUTFRegion,

	GetPrimitiveArrayCritical,
	ReleasePrimitiveArrayCritical,

	GetStringCritical,
	ReleaseStringCritical,

	NewWeakGlobalRef,
	DeleteWeakGlobalRef,

	ExceptionCheck,

	NewDirectByteBuffer,
	GetDirectBufferAddress,
	GetDirectBufferCapacity,

	GetObjectRefType
};