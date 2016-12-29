LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := BinderTestService.cpp
LOCAL_SHARED_LIBRARIES  := \
		libcutils \
		libutils \
		libbinder \
		liblog \
		libcommon
LOCAL_MODULE := libbindertestservice
include $(BUILD_SHARED_LIBRARY)
