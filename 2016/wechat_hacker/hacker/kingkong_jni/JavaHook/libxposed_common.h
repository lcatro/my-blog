#ifndef LIBXPOSED_COMMON_H_
#define LIBXPOSED_COMMON_H_

#include "xposed_shared.h"

#ifndef NATIVE_METHOD
#define NATIVE_METHOD(className, functionName, signature) \
  { #functionName, signature, reinterpret_cast<void*>(className ## _ ## functionName) }
#endif
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

namespace xposed {

#define CLASS_XPOSED_BRIDGE  "com/tencent/kingkong/xposed/XposedBridge" 

/////////////////////////////////////////////////////////////////
// Provided by common part, used by runtime-specific implementation
/////////////////////////////////////////////////////////////////
extern bool xposedLoadedSuccessfully;
extern jclass classXposedBridge;

extern int readIntConfig(const char* fileName, int defaultValue);
extern int register_natives_XposedBridge(JNIEnv* env, jclass clazz);


/////////////////////////////////////////////////////////////////
// To be provided by runtime-specific implementation
/////////////////////////////////////////////////////////////////
extern "C" bool xposedInitLib(xposed::XposedShared* shared);
//extern void onVmCreated(JNIEnv* env);
extern void logExceptionStackTrace();

extern jboolean callback_XposedBridge_initNative(JNIEnv* env);

extern jint    XposedBridge_getRuntime(JNIEnv* env, jclass clazz);
extern void    XposedBridge_hookMethodNative(JNIEnv* env, jclass clazz, jobject reflectedMethodIndirect,
                                             jobject declaredClassIndirect, jint slot, jobject additionalInfoIndirect);
extern void    XposedBridge_setObjectClassNative(JNIEnv* env, jclass clazz, jobject objIndirect, jclass clzIndirect);
extern jobject XposedBridge_cloneToSubclassNative(JNIEnv* env, jclass clazz, jobject objIndirect, jclass clzIndirect);
extern void    XposedBridge_dumpObjectNative(JNIEnv* env, jclass clazz, jobject objIndirect);
extern void XposedBridge_removeClassFinalFlag(JNIEnv* env, jclass clazz, jclass clzIndirect);

}  // namespace xposed

#endif  // LIBXPOSED_COMMON_H_
