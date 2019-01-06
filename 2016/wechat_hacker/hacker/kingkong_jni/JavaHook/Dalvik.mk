##########################################################
# Library for Dalvik-specific functions
##########################################################
BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

export RELEASE_VERSION := true

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    libxposed_common.cpp \
    libxposed_dalvik.cpp

LOCAL_C_INCLUDES += \
    dalvik \
    dalvik/vm \
    external/stlport/stlport \
    bionic \
    bionic/libstdc++/include \
    libcore/include

LOCAL_SHARED_LIBRARIES := \
    libdvm \
    liblog \
    libdl \
    libnativehelper

ifeq ($(PLATFORM_SDK_VERSION),15)
    LOCAL_SHARED_LIBRARIES += libutils
else
#    LOCAL_SHARED_LIBRARIES += libandroidfw
endif

LOCAL_CFLAGS += -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION)

ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 17)))
    LOCAL_CFLAGS += -DXPOSED_WITH_SELINUX=1
endif

ifeq ($(RELEASE_VERSION), true)
#LOCAL_CFLAGS += -fvisibility=hidden
else
LOCAL_CFLAGS += -DENABLE_DEBUG_LOG
#LOCAL_CFLAGS += -DENABLE_DEBUG_LOG_INSTRUCTION
endif

LOCAL_MODULE := libxposed_dalvik
LOCAL_MODULE_TAGS := optional
LOCAL_STRIP_MODULE := keep_symbols
LOCAL_LDLIBS	:= -llog

include $(BUILD_SHARED_LIBRARY)
