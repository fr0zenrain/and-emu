LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE   := capstone
LOCAL_C_INCLUDES += $(SOURCE_TOP)/capstone/include

LOCAL_CFLAGS += -DDEBUG -Wall -O2 -DCAPSTONE_USE_SYS_DYN_MEM -DCAPSTONE_HAS_ARM
LOCAL_SRC_FILES :=  arch/ARM/ARMDisassembler.c	\
					arch/ARM/ARMInstPrinter.c	\
					arch/ARM/ARMMapping.c	\
					arch/ARM/ARMModule.c	\
					cs.c	\
					MCInst.c	\
					MCInstrDesc.c	\
					MCRegisterInfo.c	\
					SStream.c	\
					utils.c	

include $(BUILD_STATIC_LIBRARY)
