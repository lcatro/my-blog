
BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := FreakFixer
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES += FreakFixerASM.s

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SHARED_LIBRARIES += libkkfixerdriver

ifeq ($(RELEASE_VERSION), true)
LOCAL_CFLAGS += -fvisibility=hidden
else
LOCAL_CFLAGS += -DENABLE_DEBUG_LOG
#LOCAL_CFLAGS += -DENABLE_DEBUG_LOG_INSTRUCTION
endif

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)