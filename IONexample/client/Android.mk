LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULES_TAGS := optional
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_SRC_FILES := BinderIONClient.cpp
LOCAL_SHARED_LIBRARIES := \
				libcommon \
				libcutils \
				libutils \
				libbinder
LOCAL_MODULE := binderclient
include $(BUILD_EXECUTABLE)
