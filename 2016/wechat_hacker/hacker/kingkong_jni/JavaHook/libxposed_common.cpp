/**
 * This file includes functions shared by different runtimes.
 */

#define LOG_TAG "Xposed"

#include "libxposed_common.h"
//#include <ScopedUtfChars.h>


//extern "C" jboolean initNative(JNIEnv* env, jclass clazz) ;

namespace xposed {

////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////

bool xposedLoadedSuccessfully = false;
xposed::XposedShared* xposed = NULL;
jclass classXposedBridge = NULL;

////////////////////////////////////////////////////////////
// JNI methods
////////////////////////////////////////////////////////////

jboolean XposedBridge_initNative(JNIEnv* env, jclass clazz) {
    if (!xposedLoadedSuccessfully) {
        ALOGE("Not initializing Xposed because of previous errors");
        return false;
    }

    if (!callback_XposedBridge_initNative(env))
        return false;

    return true;
}

////////////////////////////////////////////////////////////
// JNI methods registrations
////////////////////////////////////////////////////////////

int register_natives_XposedBridge(JNIEnv* env, jclass clazz) {
    const JNINativeMethod methods[] = {
        NATIVE_METHOD(XposedBridge, initNative, "()Z"),
        NATIVE_METHOD(XposedBridge, hookMethodNative, "(Ljava/lang/reflect/Member;Ljava/lang/Class;ILjava/lang/Object;)V"),
        NATIVE_METHOD(XposedBridge, removeClassFinalFlag, "(Ljava/lang/Class;)V")
    };

    return env->RegisterNatives(clazz, methods, NELEM(methods));
}

}  // namespace xposed
