LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULES_TAGS := optional
LOCAL_SRC_FILES := BinderTestClient.cpp
LOCAL_SHARED_LIBRARIES := \
				libcommon \
				libcutils \
				libutils \
				libbinder
LOCAL_MODULE := binderclient
include $(BUILD_EXECUTABLE)