#ifndef _HOTPATCH_COMMON_INLUCDE_H_
#define _HOTPATCH_COMMON_INLUCDE_H_
#pragma once

//
// KingKong Fixer
//

#include <android/log.h>
#include "jni.h"

#define TAG "KingKongFixer"
#define KINGKONG_FIXER_VERSION "2.0"

class AddressHelper;
class FunctionAnalyser;
struct PatchModuleStruct;

#ifdef ENABLE_DEBUG_LOG
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , TAG, __VA_ARGS__)
#else
#define LOGV(...) do{}while(0);
#define LOGD(...) do{}while(0);
#define LOGI(...) do{}while(0);
#define LOGW(...) do{}while(0);
#define LOGE(...) do{}while(0);
#endif

#ifdef ENABLE_DEBUG_LOG_INSTRUCTION
#define LOGD_INSTRUCTION(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#else
#define LOGD_INSTRUCTION(...) do{}while(0);
#endif

#define LOG_FATAL(...) __android_log_print(ANDROID_LOG_ERROR  , TAG, __VA_ARGS__)

#define arraysize(array) (sizeof(array) / sizeof(array[0]))

typedef unsigned int UINT32;

#define UINT32MAX 0xFFFFFFFF

#define MAX_INSTRUCTION_COUNT 102400	// I think it's big enough for a function, 1024 * 100 = 100k
#define FINGERPRINT_BUFFER_SIZE 40960

#define KK_EXPORT __attribute__((visibility("default")))

enum {
	FREAK_PARAM_HOOK_POINT = 0,
	FREAK_PARAM_RSA_NEW_ADDRESS,
	FREAK_PARAM_EXIT_TRUE,
	FREAK_PARAM_EXIT_FALSE,
	FREAK_PARAM_SSL_REGISTER,
	FREAK_PARAM_SSL_OFFSET1,
	FREAK_PARAM_SSL_OFFSET2,
	FREAK_PARAM_SSL_OFFSET3,
	FREAK_PARAM_MAX
};

enum {
	PNG_PARAM_HOOK_POINT = 0,	// Filled in PngFunctionAnalyser
	PNG_PARAM_EXIT_TRUE,
	PNG_PARAM_EXIT_FALSE,
	PNG_PARAM_REGISTER,
	PNG_PARAM_MAX
};

enum {
	NOTIFICATION_TYPE_FUNCTION_SHA1 = 2080,
	NOTIFICATION_TYPE_DO_PATCH_OK,
	NOTIFICATION_TYPE_EVIL_CATCHED,
	NOTIFICATION_TYPE_NATIVE_ERROR,
	NOTIFICATION_TYPE_DO_PATCH_FAILED,
	NOTIFICATION_TYPE_INITIAL_FAILED,
	NOTIFICATION_TYPE_REPORT_FINGERPRINT,
	NOTIFICATION_TYPE_CXAFINALIZE_CALLED,	// _cxa_finalize is called by somebody else
	NOTIFICATION_TYPE_FIXER_ERROR,			// Error occurred in *FixerASM.s
};

enum {
	FINGERPRINT_TYPE_A = 0,
	FINGERPRINT_TYPE_B,
	FINGERPRINT_TYPE_C,
};

enum {
	DO_PATCH_ERROR_MAKE_BLX = 1,
	DO_PATCH_ERROR_MAKE_LDR,
	DO_PATCH_ERROR_MAKE_WRITABLE,
	DO_PATCH_ERROR_WORK,
	DO_PATCH_ERROR_NO_FINALIZE_FOUND,
	DO_PATCH_ERROR_NO_GET_CHANGE_FOUND,
	DO_PATCH_ERROR_NO_RSA_NEW_FOUND,

};

// Set SubPatch.java
enum {
	PARAM_TYPE_MIN = 0,
	PARAM_TYPE_OFFSET_TO_FUNCTION = 1,
	PARAM_TYPE_OFFSET_TO_FILE = 2,
	PARAM_TYPE_ARBITRARY_VALUE = 3,
	PARAM_TYPE_SYMBOL_ADDRESS = 4,
	PARAM_TYPE_MAX = 5,
};

extern "C"
{
	jint Java_com_tencent_kingkong_PatchManager_nativeCalcParameter
		(JNIEnv *env, jobject thiz, jint type, jstring jstrValue1, jstring jstrValue2, jint value3);
	jint JNI_OnLoad(JavaVM* vm, void *reserved);
	void reportEvilCatched();
	jbyteArray Java_com_tencent_kingkong_PatchManager_nativeGetFingerprint
		(JNIEnv *env, jobject thiz, jstring jstrLibraryName, jstring jstrFunctionName, jint type);
	jboolean Java_com_tencent_kingkong_PatchManager_nativeDoPatch
		(JNIEnv *env, jobject thiz, jstring jstrSoFileName, jstring jstrLibraryName, jstring jstrFunctionName,
				jint hookPoint,jint jumperPoint, jintArray jintParams, jint paramCount);
	jboolean Java_com_tencent_kingkong_PatchManager_nativeVerifyPatchParams
		(JNIEnv *env, jobject thiz, jstring jstrSoFileName, jint hookPoint,
				jint jumperPoint, jintArray jintParams, jint paramCount);
	jint Java_com_tencent_kingkong_PatchManager_nativeCalcJumperPoint
		(JNIEnv *env, jobject thiz, jstring jstrLibraryName);
};

KK_EXPORT unsigned int findSymbol(const char *library, const char *func);
KK_EXPORT bool holdStrongReference(const char *libraryName);
KK_EXPORT void NotificationToJava(int type, int index, UINT32 value);
KK_EXPORT void report(JNIEnv *env, int type, bool succeed, AddressHelper *addressHelper, FunctionAnalyser *analyser,
						struct PatchModuleStruct *patch, unsigned int baseAddress);
KK_EXPORT unsigned int getAndroidVersion();
KK_EXPORT void __clear_cache(unsigned int begin, unsigned int end);

typedef bool (*FP_GeneratePatch)(struct PatchModuleStruct *patch);

#endif
