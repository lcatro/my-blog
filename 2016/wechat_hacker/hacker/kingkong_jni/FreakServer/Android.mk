
BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := FreakFixerServer
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -ldl -llog

LOCAL_SRC_FILES += FreakFixerServer.cpp
LOCAL_SRC_FILES += FreakFunctionAnalyser.cpp
LOCAL_SRC_FILES += FreakFixer.cpp

LOCAL_CFLAGS += -DENABLE_DEBUG_LOG

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SHARED_LIBRARIES += libkkfixerdriver
LOCAL_SHARED_LIBRARIES += libFreakFixer

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)
