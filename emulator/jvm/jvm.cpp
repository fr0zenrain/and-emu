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
	int ret = 0;
	char buffer[64] ={0};
	unsigned int env = libc::get_r0();
	int name_addr = libc::get_r1();
	
	if(name_addr)
	{
		uc_mem_read(g_uc,name_addr,buffer,sizeof(buffer));
	}

	unsigned int lr = libc::get_lr();
	if(lr &1)
		lr -= 1;

#ifdef _MSC_VER
	printf("FindClass(\"%s\") ->0x%x\n",buffer);
#else
	printf(RED "FindClass(\"%s\") ->0x%x\n" RESET, buffer);
#endif

	uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
	uc_reg_write(g_uc,UC_ARM_REG_R0,&ret);

	return JNI_OK;
}


int FromReflectedMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int FromReflectedField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ToReflectedMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetSuperclass() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int IsAssignableFrom() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ToReflectedField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int Throw() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ThrowNew() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ExceptionOccurred() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ExceptionDescribe() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ExceptionClear() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 

#ifdef _MSC_VER
  printf("ExceptionClear()\n");
#else
  printf(RED "ExceptionClear()\n" RESET);
#endif

  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int FatalError() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int PushLocalFrame() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int PopLocalFrame() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewGlobalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int DeleteGlobalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int DeleteLocalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int IsSameObject() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewLocalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int EnsureLocalCapacity() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int AllocObject() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewObject() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewObjectV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewObjectA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetObjectClass() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int IsInstanceOf() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetMethodID() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallObjectMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallObjectMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallObjectMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallBooleanMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallBooleanMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallBooleanMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallByteMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallByteMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallByteMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallCharMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallCharMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallCharMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallShortMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallShortMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallShortMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallIntMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallIntMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallIntMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallLongMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallLongMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallLongMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallFloatMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallFloatMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallFloatMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallDoubleMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallDoubleMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallDoubleMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallVoidMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallVoidMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallVoidMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualObjectMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualObjectMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualObjectMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualBooleanMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualBooleanMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualBooleanMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualByteMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualByteMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualByteMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualCharMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualCharMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualCharMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualShortMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualShortMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualShortMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualIntMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualIntMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualIntMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualLongMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualLongMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualLongMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualFloatMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualFloatMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualFloatMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualDoubleMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualDoubleMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualDoubleMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualVoidMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualVoidMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallNonvirtualVoidMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetFieldID() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetObjectField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetBooleanField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetByteField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetCharField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetShortField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetIntField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetLongField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetFloatField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetDoubleField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetObjectField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetBooleanField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetByteField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetCharField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetShortField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetIntField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetLongField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetFloatField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetDoubleField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticMethodID() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticObjectMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticObjectMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticObjectMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticBooleanMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticBooleanMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticBooleanMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticByteMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticByteMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticByteMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticCharMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticCharMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticCharMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticShortMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticShortMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticShortMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticIntMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticIntMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticIntMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticLongMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticLongMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticLongMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticFloatMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticFloatMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticFloatMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticDoubleMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticDoubleMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticDoubleMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticVoidMethod() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticVoidMethodV() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int CallStaticVoidMethodA() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticFieldID() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticObjectField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticBooleanField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticByteField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticCharField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticShortField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticIntField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticLongField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticFloatField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStaticDoubleField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticObjectField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticBooleanField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticByteField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticCharField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticShortField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticIntField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticLongField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticFloatField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetStaticDoubleField() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewString() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringLength() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringChars() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseStringChars() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewStringUTF() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringUTFLength() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringUTFChars() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseStringUTFChars() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetArrayLength() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewObjectArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetObjectArrayElement() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetObjectArrayElement() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewBooleanArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewByteArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewCharArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewShortArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewIntArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewLongArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewFloatArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewDoubleArray() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetBooleanArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetByteArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetCharArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetShortArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetIntArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetLongArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetFloatArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetDoubleArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseBooleanArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseByteArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseCharArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseShortArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseIntArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseLongArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseFloatArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseDoubleArrayElements() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetBooleanArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetByteArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetCharArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetShortArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetIntArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetLongArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetFloatArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetDoubleArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetBooleanArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetByteArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetCharArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetShortArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetIntArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetLongArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetFloatArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int SetDoubleArrayRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int RegisterNatives() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int UnregisterNatives() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int MonitorEnter() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int MonitorExit() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetJavaVM() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringUTFRegion() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetPrimitiveArrayCritical() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleasePrimitiveArrayCritical() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetStringCritical() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ReleaseStringCritical() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewWeakGlobalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int DeleteWeakGlobalRef() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int ExceptionCheck() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int NewDirectByteBuffer() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetDirectBufferAddress() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}
int GetDirectBufferCapacity() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
  uc_reg_write(g_uc,UC_ARM_REG_PC,&lr);
  uc_reg_write(g_uc,UC_ARM_REG_R0,&ret); 

  return JNI_OK; 
}

int GetObjectRefType() 
{
  int ret = 0;
  unsigned int env = libc::get_r0(); 
  unsigned int lr = libc::get_lr(); 
  if(lr &1) 
	  lr -= 1; 
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