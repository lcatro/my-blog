
BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := testfile
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -ldl -llog

LOCAL_SRC_FILES += stub.cpp

LOCAL_CFLAGS += -DENABLE_DEBUG_LOG

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := ut
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -ldl -llog

LOCAL_SRC_FILES += ut.cpp

LOCAL_CFLAGS += -DENABLE_DEBUG_LOG

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SHARED_LIBRARIES += libkkfixerdriver

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := PngFixerServer
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -ldl -llog

LOCAL_SRC_FILES += PngFixerServer.cpp

LOCAL_CFLAGS += -DENABLE_DEBUG_LOG

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SHARED_LIBRARIES += libkkfixerdriver
LOCAL_SHARED_LIBRARIES += libPngFixer

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)
