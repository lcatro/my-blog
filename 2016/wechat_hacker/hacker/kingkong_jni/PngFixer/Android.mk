
BASE_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := PngFixer
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -ldl

LOCAL_SRC_FILES += PngFixerASM.s

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_ARM_MODE := thumb

include $(BUILD_SHARED_LIBRARY)

