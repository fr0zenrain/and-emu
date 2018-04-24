LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := armhelper.c    \
                  malloc.c  \
                  math.S

LOCAL_MODULE    := armhelper
#LOCAL_ARM_MODE := arm
include $(BUILD_SHARED_LIBRARY)
