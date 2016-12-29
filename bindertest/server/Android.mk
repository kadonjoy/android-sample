LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_TAGS := optional
LOCAL_SRC_FILES := main_bindertestservice.cpp
LOCAL_SHARED_LIBRARIES := \
		libbindertestservice \
		libcutils \
		libutils \
		libbinder
LOCAL_MODULE := binderservermain
include $(BUILD_EXECUTABLE)
