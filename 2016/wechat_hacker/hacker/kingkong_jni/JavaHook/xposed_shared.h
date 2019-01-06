/**
 * These declarations are needed for both app_process and the libraries.
 */

#ifndef XPOSED_SHARED_H_
#define XPOSED_SHARED_H_

#include <sys/stat.h>
#include <android/log.h>
#include "jni.h"

#define TAG "XPOSED"

#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR  , TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO  , TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

namespace xposed {

struct XposedShared {
    // Global variables
//    bool zygote;
    bool startSystemServer;
    const char* startClassName;
    uint32_t xposedVersionInt;
//    bool isSELinuxEnabled;
//    bool isSELinuxEnforcing;

    // Provided by runtime-specific library, used by executable
    // void (*onVmCreated)(JNIEnv* env);

//#if XPOSED_WITH_SELINUX
//    // Provided by the executable, used by runtime-specific library
//    int (*zygoteservice_accessFile)(const char* path, int mode);
//    int (*zygoteservice_statFile)(const char* path, struct stat* st);
//    char* (*zygoteservice_readFile)(const char* path, int* bytesRead);
//#endif
};

extern XposedShared* xposed;

} // namespace xposed

#endif // XPOSED_SHARED_H_
