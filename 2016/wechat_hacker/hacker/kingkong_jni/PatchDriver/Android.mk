BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

############## Build Patch Manager Driver ############## 
include $(CLEAR_VARS)

LOCAL_MODULE := kkfixerdriver
LOCAL_MODULE_TAGS := optional

LOCAL_LDLIBS += -llog -ldl

LOCAL_SRC_FILES := ElfAnalyser.cpp
LOCAL_SRC_FILES += Instruction.cpp
LOCAL_SRC_FILES += AddressHelper.cpp
LOCAL_SRC_FILES += Patcher.cpp
LOCAL_SRC_FILES += FunctionAnalyser.cpp
LOCAL_SRC_FILES += Common.cpp
LOCAL_SRC_FILES += CachedLibUtil.cpp

ifeq ($(ENABLE_PNG_FIXER), true)
LOCAL_SRC_FILES += PngFixer.cpp
LOCAL_SRC_FILES += PngFunctionAnalyser.cpp
endif

ifeq ($(RELEASE_VERSION), true)
LOCAL_CFLAGS += -fvisibility=hidden
else
LOCAL_CFLAGS += -DENABLE_DEBUG_LOG
#LOCAL_CFLAGS += -DENABLE_DEBUG_LOG_INSTRUCTION
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS += -ffunction-sections -fdata-sections 
LOCAL_LDFLAGS += -Wl,--gc-sections

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)

