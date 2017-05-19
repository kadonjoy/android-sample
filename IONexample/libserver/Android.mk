LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_SRC_FILES := BinderIONService.cpp
LOCAL_SHARED_LIBRARIES  := \
		libcutils \
		libutils \
		libbinder \
		liblog \
		libcommon
LOCAL_MODULE := libbindertestservice
include $(BUILD_SHARED_LIBRARY)
