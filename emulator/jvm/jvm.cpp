#include "jvm.h"
#include "../runtime/emulator.h"
#include "../../include/unicorn/unicorn.h"
#include "jni.h"
#include <string>
#include <map>
using namespace std;
std::map<void*,std::map<std::string,std::string>> jni_func_map;

extern uc_engine* g_uc;

int GetEnv()
{
	int env = JVM_INTERFACE_ADDRESS;
	int jvm = emulator::get_r0();
	int ret = 0;
	unsigned int env_ptr = emulator::get_r1();
	int version = emulator::get_r2();
	if (version < JNI_VERSION_1_1 || version > JNI_VERSION_1_6) {
		return JNI_EVERSION;
	}
    unsigned int jnienv = emulator::get_emulator()->get_jvm_jnienv();
	if(env_ptr)
	{
		uc_mem_write(g_uc,env_ptr,&jnienv,4);
	}

	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("GetEnv()\n");
#else
	printf(RED "GetEnv()\n" RESET);
#endif

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
	unsigned int env = emulator::get_r0();
	unsigned int name_addr = emulator::get_r1();
	
	if(name_addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,name_addr+i,&buffer[i],1);
			if(buffer[i] == 0)
				break;
		}
	}

	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("FindClass(0x%x,\"%s\")\n",env, buffer);
#else
	printf(RED "FindClass(0x%x,\"%s\")\n" RESET,env, buffer);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return JNI_OK;
}


int FromReflectedMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FromReflectedMethod(\"%s\")\n",buffer);
#else
	printf(RED "FromReflectedMethod(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int FromReflectedField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FromReflectedField(\"%s\")\n",buffer);
#else
	printf(RED "FromReflectedField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ToReflectedMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ToReflectedMethod(\"%s\")\n",buffer);
#else
	printf(RED "ToReflectedMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetSuperclass() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetSuperclass(\"%s\")\n",buffer);
#else
	printf(RED "GetSuperclass(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsAssignableFrom() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsAssignableFrom(\"%s\")\n",buffer);
#else
	printf(RED "IsAssignableFrom(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ToReflectedField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ToReflectedField(\"%s\")\n",buffer);
#else
	printf(RED "ToReflectedField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int Throw() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("Throw(\"%s\")\n",buffer);
#else
	printf(RED "Throw(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ThrowNew() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ThrowNew(\"%s\")\n",buffer);
#else
	printf(RED "ThrowNew(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionOccurred() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionOccurred(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionOccurred(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionDescribe() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ExceptionDescribe(\"%s\")\n",buffer);
#else
	printf(RED "ExceptionDescribe(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionClear() 
{
	int ret = 0;
	unsigned int env = emulator::get_r0();
	unsigned int lr = emulator::get_lr();

#ifdef _MSC_VER
	printf("ExceptionClear()\n");
#else
	printf(RED "ExceptionClear()\n" RESET);
#endif
	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);
	return JNI_OK;
}
int FatalError() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("FatalError(\"%s\")\n",buffer);
#else
	printf(RED "FatalError(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int PushLocalFrame() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("PushLocalFrame(\"%s\")\n",buffer);
#else
	printf(RED "PushLocalFrame(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int PopLocalFrame() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("PopLocalFrame(\"%s\")\n",buffer);
#else
	printf(RED "PopLocalFrame(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewGlobalRef() 
{
	int ret = 1;
	unsigned int env = emulator::get_r0();
    unsigned int clazz = emulator::get_r1();

#ifdef _MSC_VER
	printf("NewGlobalRef(0x%x,0x%x)\n",env,clazz);
#else
	printf(RED "NewGlobalRef(0x%x,0x%x)\n" RESET, env,clazz);
#endif 

    emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("DeleteGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "DeleteGlobalRef(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteLocalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0();
    unsigned int clazz = emulator::get_r1();


#ifdef _MSC_VER
	printf("DeleteLocalRef(0x%x,0x%x)\n", env,clazz);
#else
	printf(RED "DeleteLocalRef(0x%x,0x%x)\n" RESET, env,clazz);
#endif 

	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsSameObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsSameObject(\"%s\")\n",buffer);
#else
	printf(RED "IsSameObject(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewLocalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewLocalRef(\"%s\")\n",buffer);
#else
	printf(RED "NewLocalRef(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int EnsureLocalCapacity() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("EnsureLocalCapacity(\"%s\")\n",buffer);
#else
	printf(RED "EnsureLocalCapacity(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int AllocObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("AllocObject(\"%s\")\n",buffer);
#else
	printf(RED "AllocObject(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObject() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObject(\"%s\")\n",buffer);
#else
	printf(RED "NewObject(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectV(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectA(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectClass() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectClass(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectClass(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int IsInstanceOf() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("IsInstanceOf(\"%s\")\n",buffer);
#else
	printf(RED "IsInstanceOf(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetMethodID() 
{
	int ret = 0;
	char buffer[256]={0}; 
    char name[256] ={0};
    char sig[256] ={0};
    uc_err err;
    unsigned int env = emulator::get_r0(); 
    unsigned int classz = emulator::get_r1(); 
    unsigned int name_addr = emulator::get_r2(); 
    unsigned int sig_addr = emulator::get_r3(); 
    unsigned int lr = emulator::get_lr(); 

    if(name_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,name_addr+i,&name[i],1);
            if(name[i] == 0)
                break;
        }
    }

    if(sig_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,sig_addr+i,&sig[i],1);
            if(sig[i] == 0)
                break;
        }
    }

	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("GetMethodID(0x%x,0x%x,\"%s\",\"%s\")\n",env,classz,name,sig);
#else
	printf(RED "GetMethodID(0x%x,0x%x,\"%s\",\"%s\")\n" RESET, env,classz,name,sig); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("CallObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallObjectMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallBooleanMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallByteMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallCharMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallShortMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallIntMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallLongMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallFloatMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallDoubleMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallVoidMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualObjectMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualBooleanMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualByteMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualCharMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualShortMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualIntMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualLongMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualFloatMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualDoubleMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallNonvirtualVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallNonvirtualVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallNonvirtualVoidMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFieldID() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFieldID(\"%s\")\n",buffer);
#else
	printf(RED "GetFieldID(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectField(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetByteField(\"%s\")\n",buffer);
#else
	printf(RED "GetByteField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharField(\"%s\")\n",buffer);
#else
	printf(RED "GetCharField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortField(\"%s\")\n",buffer);
#else
	printf(RED "GetShortField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntField(\"%s\")\n",buffer);
#else
	printf(RED "GetIntField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongField(\"%s\")\n",buffer);
#else
	printf(RED "GetLongField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatField(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetObjectField(\"%s\")\n",buffer);
#else
	printf(RED "SetObjectField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "SetBooleanField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetByteField(\"%s\")\n",buffer);
#else
	printf(RED "SetByteField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetCharField(\"%s\")\n",buffer);
#else
	printf(RED "SetCharField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetShortField(\"%s\")\n",buffer);
#else
	printf(RED "SetShortField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetIntField(\"%s\")\n",buffer);
#else
	printf(RED "SetIntField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetLongField(\"%s\")\n",buffer);
#else
	printf(RED "SetLongField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetFloatField(\"%s\")\n",buffer);
#else
	printf(RED "SetFloatField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "SetDoubleField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticMethodID() 
{
	int ret = 0;
	uc_err err;
	char name[256] ={0};
	char sig[256] ={0};
	unsigned int env = emulator::get_r0(); 
	unsigned int classz = emulator::get_r1(); 
	unsigned int name_addr = emulator::get_r2(); 
	unsigned int sig_addr = emulator::get_r3(); 

	if(name_addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,name_addr+i,&name[i],1);
			if(name[i] == 0)
				break;
		}
	}

	if(sig_addr)
	{
		for(int i = 0; i < 256; i++)
		{
			err = uc_mem_read(g_uc,sig_addr+i,&sig[i],1);
			if(sig[i] == 0)
				break;
		}
	}

#ifdef _MSC_VER
	printf("GetStaticMethodID(0x%x,0x%x,\"%s\",\"%s\")\n",env, classz, name,sig);
#else
	printf(RED "GetStaticMethodID(0x%x,0x%x,\"%s\",\"%s\")\n" RESET, env, classz, name,sig);
#endif 

	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticObjectMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticObjectMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticObjectMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticBooleanMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticBooleanMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticBooleanMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticByteMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticByteMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticByteMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticCharMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticCharMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticCharMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticShortMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticShortMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticShortMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticIntMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticIntMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticIntMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticLongMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticLongMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticLongMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticFloatMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticFloatMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticFloatMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticDoubleMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticDoubleMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticDoubleMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethod() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethod(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethod(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethodV() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethodV(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethodV(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int CallStaticVoidMethodA() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("CallStaticVoidMethodA(\"%s\")\n",buffer);
#else
	printf(RED "CallStaticVoidMethodA(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticFieldID() 
{
	int ret = 0;
	char buffer[256]={0}; 
    char sig_buffer[256]={0}; 
    uc_err err;
	unsigned int env = emulator::get_r0(); 
    unsigned int classz = emulator::get_r1(); 
    unsigned int name_addr = emulator::get_r2(); 
    unsigned int sig_addr = emulator::get_r3();
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

    if(name_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,name_addr+i,&buffer[i],1);
            if(buffer[i] == 0)
                break;
        }
    }

    if(sig_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,sig_addr+i,&sig_buffer[i],1);
            if(sig_buffer[i] == 0)
                break;
        }
    }

#ifdef _MSC_VER
	printf("GetStaticFieldID(0x%x, 0x%x, \"%s\",\"%s\")\n",env,classz,buffer,sig_buffer);
#else
	printf(RED "GetStaticFieldID(0x%x, 0x%x, \"%s\",\"%s\")\n" RESET, env,classz,buffer,sig_buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
    unsigned int classz = emulator::get_r1(); 
    unsigned int field_addr = emulator::get_r2(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticObjectField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticObjectField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticBooleanField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticByteField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticByteField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticCharField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticCharField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticShortField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticShortField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticIntField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticIntField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticLongField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticLongField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticFloatField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticFloatField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStaticDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStaticDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "GetStaticDoubleField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticObjectField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticObjectField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticObjectField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticBooleanField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticBooleanField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticBooleanField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticByteField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticByteField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticByteField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticCharField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticCharField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticCharField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticShortField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticShortField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticShortField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticIntField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticIntField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticIntField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticLongField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticLongField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticLongField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticFloatField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticFloatField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticFloatField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetStaticDoubleField() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetStaticDoubleField(\"%s\")\n",buffer);
#else
	printf(RED "SetStaticDoubleField(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewString() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewString(\"%s\")\n",buffer);
#else
	printf(RED "NewString(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringLength(\"%s\")\n",buffer);
#else
	printf(RED "GetStringLength(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringChars(\"%s\")\n",buffer);
#else
	printf(RED "GetStringChars(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseStringChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringChars(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringChars(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewStringUTF() 
{
	int ret = 0;
	char buffer[256]={0};
    uc_err err;
	unsigned int env = emulator::get_r0();
    unsigned int str_addr = emulator::get_r1();
    unsigned int lr = emulator::get_lr();

    if(str_addr)
    {
        for(int i = 0; i < 256; i++)
        {
            err = uc_mem_read(g_uc,str_addr+i,&buffer[i],1);
            if(buffer[i] == 0)
                break;
        }
    }

    emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("NewStringUTF(\"%s\")\n",buffer);
#else
	printf(RED "NewStringUTF(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0();
    unsigned int str = emulator::get_r1();
	unsigned int lr = emulator::get_lr();
    if (str){
        uc_mem_read(g_uc, str, buffer, 256);
	    ret = strlen(buffer);
    }

#ifdef _MSC_VER
	printf("GetStringUTFLength(\"%s\")\n",buffer);
#else
	printf(RED "GetStringUTFLength(\"%s\")\n" RESET, buffer); 
#endif
    emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFChars() 
{
    int ret = 0;
	char buffer[256]={0};
	unsigned int env = emulator::get_r0();
	unsigned int jstr_addr = emulator::get_r1();
	unsigned int cstr_addr = emulator::get_r2();
    unsigned int lr = emulator::get_lr();
	if (jstr_addr){
		uc_mem_read(g_uc, jstr_addr, buffer, 256);
	}

	if(cstr_addr){
		uc_mem_write(g_uc, cstr_addr, buffer, strlen(buffer));
	}

#ifdef _MSC_VER
    printf("GetStringUTFChars(0x%x,%s,0x%x)\n",env, buffer,cstr_addr,ret);
#else
    printf(RED "GetStringUTFChars(0x%x,%s,0x%x)\n" RESET, env, buffer,cstr_addr,ret);
#endif
    emulator::update_cpu_model();

    uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
    uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

    return JNI_OK;
}
int ReleaseStringUTFChars() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringUTFChars(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringUTFChars(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetArrayLength() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("GetArrayLength(\"%s\")\n",buffer);
#else
	printf(RED "GetArrayLength(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewObjectArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewObjectArray(\"%s\")\n",buffer);
#else
	printf(RED "NewObjectArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectArrayElement() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectArrayElement(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectArrayElement(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetObjectArrayElement() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetObjectArrayElement(\"%s\")\n",buffer);
#else
	printf(RED "SetObjectArrayElement(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewBooleanArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewBooleanArray(\"%s\")\n",buffer);
#else
	printf(RED "NewBooleanArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewByteArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewByteArray(\"%s\")\n",buffer);
#else
	printf(RED "NewByteArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewCharArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewCharArray(\"%s\")\n",buffer);
#else
	printf(RED "NewCharArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewShortArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewShortArray(\"%s\")\n",buffer);
#else
	printf(RED "NewShortArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewIntArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewIntArray(\"%s\")\n",buffer);
#else
	printf(RED "NewIntArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewLongArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewLongArray(\"%s\")\n",buffer);
#else
	printf(RED "NewLongArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewFloatArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewFloatArray(\"%s\")\n",buffer);
#else
	printf(RED "NewFloatArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewDoubleArray() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewDoubleArray(\"%s\")\n",buffer);
#else
	printf(RED "NewDoubleArray(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("GetByteArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetByteArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetCharArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetShortArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetIntArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetLongArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseBooleanArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseBooleanArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseBooleanArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseByteArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("ReleaseByteArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseByteArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseCharArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseCharArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseCharArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseShortArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseShortArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseShortArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseIntArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseIntArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseIntArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseLongArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseLongArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseLongArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseFloatArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseFloatArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseFloatArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseDoubleArrayElements() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseDoubleArrayElements(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseDoubleArrayElements(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetBooleanArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetBooleanArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetBooleanArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetByteArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetByteArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetByteArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetCharArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetCharArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetCharArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetShortArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetShortArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetShortArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetIntArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetIntArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetIntArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetLongArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetLongArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetLongArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetFloatArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetFloatArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetFloatArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDoubleArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDoubleArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetDoubleArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetBooleanArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetBooleanArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetBooleanArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetByteArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetByteArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetByteArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetCharArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetCharArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetCharArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetShortArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetShortArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetShortArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetIntArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetIntArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetIntArrayRegion(\"%s\")\n" RESET, buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetLongArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetLongArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetLongArrayRegion(\"%s\")\n" RESET,buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetFloatArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetFloatArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetFloatArrayRegion(\"%s\")\n" RESET,buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int SetDoubleArrayRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("SetDoubleArrayRegion(\"%s\")\n",buffer);
#else
	printf(RED "SetDoubleArrayRegion(\"%s\")\n" RESET,buffer); 
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int RegisterNatives() 
{
	int ret = 0;
	char name[256]={0};
    char sig[256]={0};
    int count = emulator::get_r3();
	unsigned int env = emulator::get_r0(); 
	unsigned int method_addr = emulator::get_r2();
	unsigned int lr = emulator::get_lr();

	JNINativeMethod* methodptr = (JNINativeMethod* )malloc(count*sizeof(JNINativeMethod));
    JNINativeMethod* method = methodptr;

    uc_err err = uc_mem_read(g_uc,method_addr,method,count*sizeof(JNINativeMethod));

    for(int i = 0 ; i < count ;i++) {

        if(method->name)
        {
            for(int i = 0; i < 256; i++)
            {
                err = uc_mem_read(g_uc,(uint64_t)method->name+i,&name[i],1);
                if(name[i] == 0)
                    break;
            }
        }

        if(method->signature)
        {
            for(int i = 0; i < 256; i++)
            {
                err = uc_mem_read(g_uc,(uint64_t)method->signature+i,&sig[i],1);
                if(sig[i] == 0)
                    break;
            }
        }
        std::map<std::string,std::string> info;
        info.insert(std::make_pair(name,sig));
	    jni_func_map.insert(std::make_pair(method->fnPtr,info));
#ifdef _MSC_VER
        printf("RegisterNatives(\"%s\",\"%s\",%p)\n",name,sig,method->fnPtr);
#else
        printf(RED "RegisterNatives(\"%s\",\"%s\",%p)\n" RESET, name,sig,method->fnPtr);
#endif
        method++;
    }

    free(methodptr);

	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int UnregisterNatives() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("UnregisterNatives(\"%s\")\n",buffer);
#else
	printf(RED "UnregisterNatives(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int MonitorEnter() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("MonitorEnter(\"%s\")\n",buffer);
#else
	printf(RED "MonitorEnter(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int MonitorExit() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("MonitorExit(\"%s\")\n",buffer);
#else
	printf(RED "MonitorExit(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetJavaVM() 
{
	int ret = 0;
    uc_err err;

	unsigned int env = emulator::get_r0();
    unsigned int vm = emulator::get_r1();

    unsigned int jvm = emulator::get_emulator()->get_jvm();
    err = uc_mem_write(g_uc,vm,&jvm,4);
#ifdef _MSC_VER
	printf("GetJavaVM(0x%x, 0x%x)\n",env, vm);
#else
	printf(RED "GetJavaVM(0x%x, 0x%x)\n" RESET, env, vm);
#endif

	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetStringRegion(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringUTFRegion() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringUTFRegion(\"%s\")\n",buffer);
#else
	printf(RED "GetStringUTFRegion(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetPrimitiveArrayCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetPrimitiveArrayCritical(\"%s\")\n",buffer);
#else
	printf(RED "GetPrimitiveArrayCritical(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleasePrimitiveArrayCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleasePrimitiveArrayCritical(\"%s\")\n",buffer);
#else
	printf(RED "ReleasePrimitiveArrayCritical(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetStringCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetStringCritical(\"%s\")\n",buffer);
#else
	printf(RED "GetStringCritical(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ReleaseStringCritical() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("ReleaseStringCritical(\"%s\")\n",buffer);
#else
	printf(RED "ReleaseStringCritical(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewWeakGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewWeakGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "NewWeakGlobalRef(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int DeleteWeakGlobalRef() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("DeleteWeakGlobalRef(\"%s\")\n",buffer);
#else
	printf(RED "DeleteWeakGlobalRef(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int ExceptionCheck() 
{
	int ret = 0;
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 

#ifdef _MSC_VER
	printf("ExceptionCheck()\n");
#else
	printf(RED "ExceptionCheck()\n" RESET);
#endif
	emulator::update_cpu_model();

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int NewDirectByteBuffer() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("NewDirectByteBuffer(\"%s\")\n",buffer);
#else
	printf(RED "NewDirectByteBuffer(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDirectBufferAddress() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDirectBufferAddress(\"%s\")\n",buffer);
#else
	printf(RED "GetDirectBufferAddress(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetDirectBufferCapacity() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetDirectBufferCapacity(\"%s\")\n",buffer);
#else
	printf(RED "GetDirectBufferCapacity(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}
int GetObjectRefType() 
{
	int ret = 0;
	char buffer[256]={0}; 
	unsigned int env = emulator::get_r0(); 
	unsigned int lr = emulator::get_lr(); 
	if(lr &1) 
		lr -= 1; 

#ifdef _MSC_VER
	printf("GetObjectRefType(\"%s\")\n",buffer);
#else
	printf(RED "GetObjectRefType(\"%s\")\n" RESET, buffer);
#endif 

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

	return JNI_OK; 
}

int AttachCurrentThread()
{
	int ret = 0;
	char buffer[256]={0};
	unsigned int env = emulator::get_r0();
	unsigned int lr = emulator::get_lr();
	emulator::update_cpu_model();

#ifdef _MSC_VER
	printf("AttachCurrentThread(\"%s\")\n",buffer);
#else
	printf(RED "AttachCurrentThread(\"%s\")\n" RESET, buffer);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return JNI_OK;
}

int DetachCurrentThread()
{
    int ret = 0;
    char buffer[256]={0};
    unsigned int env = emulator::get_r0();
    unsigned int lr = emulator::get_lr();
    emulator::update_cpu_model();

#ifdef _MSC_VER
    printf("DetachCurrentThread(\"%s\")\n",buffer);
#else
    printf(RED "DetachCurrentThread(\"%s\")\n" RESET, buffer);
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
    AttachCurrentThread,
    DetachCurrentThread,
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

int jvm::doJNICall(const char* name, const char* sig)
{
    std::map<std::string,std::string> name_map;
    std::map<void*,std::map<std::string,std::string>>::iterator iter = jni_func_map.begin();

    while (iter != jni_func_map.end())
    {
        void* fptr = iter->first;
        std::map<std::string,std::string>::iterator iter1 = iter->second.begin();
        if(iter1 != iter->second.end())
        {
            string func = iter1->first;
            if (func == name && iter1->second == sig)
            {
               /* emulator::get_emulator()->restore_cpu_status();
                unsigned int jnienv = emulator::get_emulator()->get_jvm_jnienv();
                jclass clazz;
                int value = 0;
                jobject obj = NewObject(0);
                uc_err err = uc_reg_write(g_uc,UC_ARM_REG_R0, &jnienv);
                err = uc_reg_write(g_uc,UC_ARM_REG_R1,&obj);
                err = uc_reg_write(g_uc,UC_ARM_REG_R2,&value);
                uintptr_t lr = EMULATOR_PAUSE_ADDRESS;
                lr |= 1;
                err=uc_reg_write(g_uc, UC_ARM_REG_LR, &lr);
                int cpsr= 0x20;
                err = uc_reg_write(g_uc,UC_ARM_REG_CPSR,&cpsr);

                emulator::get_emulator()->show_disasm = 1;
                unsigned int pc = (unsigned int)fptr;
                pc = pc&1?pc-1:pc;
                emulator::get_emulator()->start_emulator(pc,0);*/
            }
            ++iter;
        }

        ++iter;
    }

    return 1;
}