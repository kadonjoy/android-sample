LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= callStackMain
LOCAL_SRC_FILES := main.cpp

LOCAL_SHARED_LIBRARIES := libutilscallstack liblog libutils

include $(BUILD_EXECUTABLE)
