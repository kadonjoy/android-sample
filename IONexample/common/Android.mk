LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := IBinderIONService.cpp
LOCAL_SHARED_LIBRARIES += \
	libbinder \
	libutils	\
	libcutils
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libcommon
include $(BUILD_SHARED_LIBRARY)
