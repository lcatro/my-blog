/**
 * This file includes functions specific to the Dalvik runtime.
 */

#define LOG_TAG "Xposed"

#include "libxposed_dalvik.h"
#include "xposed_offsets.h"

#include <dlfcn.h>


static void* load_jar(JNIEnv* jni_env,const char* jar_path,const char* find_class);
static bool dvmRegisterJNIMethod(Method* method, void* fnPtr);
static jobject get_method(JNIEnv* jni_env,jclass find_class,const char* find_method_name);


namespace xposed {

////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////

bool initMemberOffsets(JNIEnv* env);
void prepareSubclassReplacement(jclass clazz);
void hookedMethodCallback(const u4* args, JValue* pResult, const Method* method, ::Thread* self);
void XposedBridge_invokeOriginalMethodNative(const u4* args, JValue* pResult, const Method* method, ::Thread* self);

////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////

static Method* xposedHandleHookedMethod = NULL;
static ClassObject* objectArrayClass = NULL;
static size_t arrayContentsOffset = 0;
static void* PTR_gDvmJit = NULL;


JNIEnv* global_jnienv=NULL;

////////////////////////////////////////////////////////////
// Library initialization
////////////////////////////////////////////////////////////

/** Called by Xposed's app_process replacement. */
bool xposedInitLib(xposed::XposedShared* shared) {
    xposed = shared;

//    xposed->onVmCreated = &onVmCreated;
    return true;
}

/** Called very early during VM startup. */
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    ALOGD("Call JNI_OnLoad() ...");

    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("Could not get env for '%s'", CLASS_XPOSED_BRIDGE);
        return JNI_ERR;
    }
    if (!initMemberOffsets(env)) {
  	return JNI_ERR;
    }

/*
    classXposedBridge = env->FindClass(CLASS_XPOSED_BRIDGE);
    classXposedBridge = reinterpret_cast<jclass>(env->NewGlobalRef(classXposedBridge));
    if (classXposedBridge == NULL) {
         ALOGE("Error while loading Xposed class '%s':", CLASS_XPOSED_BRIDGE);
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return JNI_ERR;
    }
    ALOGI("Found Xposed class '%s', now initializing", CLASS_XPOSED_BRIDGE);
    if (register_natives_XposedBridge(env, classXposedBridge) != JNI_OK) {
        ALOGE("Could not register natives for '%s'", CLASS_XPOSED_BRIDGE);
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return JNI_ERR;
    }
    xposedLoadedSuccessfully = true;
*/

    classXposedBridge=(jclass)load_jar(env,"/data/data/com.tencent.mm/app_recover_lib/kingkong.jar",CLASS_XPOSED_BRIDGE);
    if (NULL==classXposedBridge) {
        ALOGD("Load Xposed framework jar ERROR ..");
        return JNI_ERR;
    }
    ALOGD("Load Xposed framework jar Success ..");

    callback_XposedBridge_initNative(env);

    ALOGD("try to registe hookMethodNative ..");
    Method* hookMethodNative=dvmFindDirectMethodByDescriptor((ClassObject*)dvmDecodeIndirectRef(dvmThreadSelf(), classXposedBridge),"hookMethodNative","(Ljava/lang/reflect/Member;Ljava/lang/Class;ILjava/lang/Object;)V");
    dvmRegisterJNIMethod(hookMethodNative,(void*)XposedBridge_hookMethodNative);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        ALOGD("WARNING! Exception ..");
        return JNI_ERR;
    }

    ALOGD("try to get try_hook ..");
    jmethodID try_hook=(jmethodID)dvmFindDirectMethodByDescriptor((ClassObject*)dvmDecodeIndirectRef(dvmThreadSelf(), classXposedBridge),"try_hook","(Ljava/lang/Class;Ljava/lang/Class;)V");
    if (NULL==try_hook) {
        ALOGD("Get try_hook ERROR ..");
        return JNI_ERR;
    }
    ALOGD("Get com/tencent/kingkong/database/SQLiteDatabase Class ..");
    jclass SQLiteDatabase=env->FindClass("com/tencent/kingkong/database/SQLiteDatabase");
    if (NULL==SQLiteDatabase) {
        ALOGD("Get Class ERROR ..");
        return JNI_ERR;
    }
    ALOGD("Get com/tenpay/android/wechat/TenpaySecureEncrypt Class ..");
    jclass Encrypt=env->FindClass("com/tenpay/android/wechat/TenpaySecureEncrypt");
    if (NULL==Encrypt) {
        ALOGD("Get Class ERROR ..");
        return JNI_ERR;
    }
    ALOGD("try to call try_hook ..");
    env->CallStaticObjectMethod(classXposedBridge,try_hook,SQLiteDatabase,Encrypt);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        ALOGD("WARNING! Exception ..");
        return JNI_ERR;
    }

    return 0x10006;
}

bool initMemberOffsets(JNIEnv* env) {
    PTR_gDvmJit = dlsym(RTLD_DEFAULT, "gDvmJit");

    if (PTR_gDvmJit == NULL) {
        offsetMode = MEMBER_OFFSET_MODE_NO_JIT;
    } else {
        offsetMode = MEMBER_OFFSET_MODE_WITH_JIT;
    }
    ALOGD("Using structure member offsets for mode %s", xposedOffsetModesDesc[offsetMode]);

    MEMBER_OFFSET_COPY(DvmJitGlobals, codeCacheFull);

//    int overrideCodeCacheFull = readIntConfig(XPOSED_OVERRIDE_JIT_RESET_OFFSET, -1);
//    if (overrideCodeCacheFull > 0 && overrideCodeCacheFull < 0x400) {
//        ALOGI("Offset for DvmJitGlobals.codeCacheFull is overridden, new value is 0x%x", overrideCodeCacheFull);
//        MEMBER_OFFSET_VAR(DvmJitGlobals, codeCacheFull) = overrideCodeCacheFull;
//    }

    // detect offset of ArrayObject->contents
    jintArray dummyArray = env->NewIntArray(1);
    if (dummyArray == NULL) {
        ALOGE("Could allocate int array for testing");
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return false;
    }

    jint* dummyArrayElements = env->GetIntArrayElements(dummyArray, NULL);
    arrayContentsOffset = (size_t)dummyArrayElements - (size_t)dvmDecodeIndirectRef(dvmThreadSelf(), dummyArray);
    env->ReleaseIntArrayElements(dummyArray,dummyArrayElements, 0);
    env->DeleteLocalRef(dummyArray);

    if (arrayContentsOffset < 12 || arrayContentsOffset > 128) {
        ALOGE("Detected strange offset %d of ArrayObject->contents", arrayContentsOffset);
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////
// Utility methods
////////////////////////////////////////////////////////////

/** Portable clone of dvmSetObjectArrayElement() */
inline void setObjectArrayElement(const ArrayObject* obj, int index, Object* val) {
    uintptr_t arrayContents = (uintptr_t)obj + arrayContentsOffset;
    ((Object **)arrayContents)[index] = val;
    dvmWriteBarrierArray(obj, index, index + 1);
}

/** Lay the foundations for XposedBridge.setObjectClassNative() */
void prepareSubclassReplacement(jclass clazz) {
    // clazz is supposed to replace its superclass, so make sure enough memory is allocated
    ClassObject* sub = (ClassObject*) dvmDecodeIndirectRef(dvmThreadSelf(), clazz);
    ClassObject* super = sub->super;
    super->objectSize = sub->objectSize;
}

/** Wrapper used by the common part of the library. */
void logExceptionStackTrace() {
    dvmLogExceptionStackTrace();
}

/** Check whether a method is already hooked. */
inline bool isMethodHooked(const Method* method) {
    return (method->nativeFunc == &hookedMethodCallback);
}

////////////////////////////////////////////////////////////
// JNI methods
////////////////////////////////////////////////////////////

jboolean callback_XposedBridge_initNative(JNIEnv* env) {
    xposedHandleHookedMethod = (Method*) env->GetStaticMethodID(classXposedBridge, "handleHookedMethod",
        "(Ljava/lang/reflect/Member;ILjava/lang/Object;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
    if (xposedHandleHookedMethod == NULL) {
        ALOGE("ERROR: could not find method %s.handleHookedMethod(Member, int, Object, Object, Object[])", CLASS_XPOSED_BRIDGE);
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return false;
    }

    Method* xposedInvokeOriginalMethodNative = (Method*) env->GetStaticMethodID(classXposedBridge, "invokeOriginalMethodNative",
        "(Ljava/lang/reflect/Member;I[Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
    if (xposedInvokeOriginalMethodNative == NULL) {
        ALOGE("ERROR: could not find method %s.invokeOriginalMethodNative(Member, int, Class[], Class, Object, Object[])", CLASS_XPOSED_BRIDGE);
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return false;
    }
    dvmSetNativeFunc(xposedInvokeOriginalMethodNative, XposedBridge_invokeOriginalMethodNative, NULL);

    objectArrayClass = dvmFindArrayClass("[Ljava/lang/Object;", NULL);
    if (objectArrayClass == NULL) {
        ALOGE("Error while loading Object[] class");
        dvmLogExceptionStackTrace();
        env->ExceptionClear();
        return false;
    }

    return true;
}

/** This is called when a hooked method is executed. */
void hookedMethodCallback(const u4* args, JValue* pResult, const Method* method, ::Thread* self) {
    if (!isMethodHooked(method)) {
        dvmThrowNoSuchMethodError("Could not find Xposed original method - how did you even get here?");
        return;
    }

    XposedHookInfo* hookInfo = (XposedHookInfo*) method->insns;
    Method* original = (Method*) hookInfo;
    Object* originalReflected = hookInfo->reflectedMethod;
    Object* additionalInfo = hookInfo->additionalInfo;

    // convert/box arguments
    const char* desc = &method->shorty[1]; // [0] is the return type.
    Object* thisObject = NULL;
    size_t srcIndex = 0;
    size_t dstIndex = 0;

    // for non-static methods determine the "this" pointer
    if (!dvmIsStaticMethod(original)) {
        thisObject = (Object*) args[0];
        srcIndex++;
    }

    ArrayObject* argsArray = dvmAllocArrayByClass(objectArrayClass, strlen(method->shorty) - 1, ALLOC_DEFAULT);
    if (argsArray == NULL) {
        return;
    }

    while (*desc != '\0') {
        char descChar = *(desc++);
        JValue value;
        Object* obj;

        switch (descChar) {
        case 'Z':
        case 'C':
        case 'F':
        case 'B':
        case 'S':
        case 'I':
            value.i = args[srcIndex++];
            obj = (Object*) dvmBoxPrimitive(value, dvmFindPrimitiveClass(descChar));
            dvmReleaseTrackedAlloc(obj, self);
            break;
        case 'D':
        case 'J':
            value.j = dvmGetArgLong(args, srcIndex);
            srcIndex += 2;
            obj = (Object*) dvmBoxPrimitive(value, dvmFindPrimitiveClass(descChar));
            dvmReleaseTrackedAlloc(obj, self);
            break;
        case '[':
        case 'L':
            obj  = (Object*) args[srcIndex++];
            break;
        default:
            ALOGE("Unknown method signature description character: %c", descChar);
            obj = NULL;
            srcIndex++;
        }
        setObjectArrayElement(argsArray, dstIndex++, obj);
    }

    // call the Java handler function
    JValue result;

    dvmCallMethod(self, xposedHandleHookedMethod, NULL, &result,
        originalReflected, (int) original, additionalInfo, thisObject, argsArray);

    dvmReleaseTrackedAlloc(argsArray, self);

    // exceptions are thrown to the caller
    if (dvmCheckException(self)) {
        return;
    }

    // return result with proper type
    ClassObject* returnType = dvmGetBoxedReturnType(method);
    if (returnType->primitiveType == PRIM_VOID) {
        // ignored
    } else if (result.l == NULL) {
        if (dvmIsPrimitiveClass(returnType)) {
            dvmThrowNullPointerException("null result when primitive expected");
        }
        pResult->l = NULL;
    } else {
        if (!dvmUnboxPrimitive(result.l, returnType, pResult)) {
            dvmThrowClassCastException(result.l->clazz, returnType);
        }
    }
}


void XposedBridge_hookMethodNative(JNIEnv* env, jclass clazz, jobject reflectedMethodIndirect,
            jobject declaredClassIndirect, jint slot, jobject additionalInfoIndirect) {
    // Usage errors?
    if (declaredClassIndirect == NULL || reflectedMethodIndirect == NULL) {
        dvmThrowIllegalArgumentException("method and declaredClass must not be null");
        return;
    }

    // Find the internal representation of the method
    ClassObject* declaredClass = (ClassObject*) dvmDecodeIndirectRef(dvmThreadSelf(), declaredClassIndirect);
    Method* method = dvmSlotToMethod(declaredClass, slot);
    if (method == NULL) {
        dvmThrowNoSuchMethodError("Could not get internal representation for method");
        return;
    }

    if (isMethodHooked(method)) {
        // already hooked
        return;
    }

    // Save a copy of the original method and other hook info
    XposedHookInfo* hookInfo = (XposedHookInfo*) calloc(1, sizeof(XposedHookInfo));
    memcpy(hookInfo, method, sizeof(hookInfo->originalMethodStruct));
    hookInfo->reflectedMethod = dvmDecodeIndirectRef(dvmThreadSelf(), env->NewGlobalRef(reflectedMethodIndirect));
    hookInfo->additionalInfo = dvmDecodeIndirectRef(dvmThreadSelf(), env->NewGlobalRef(additionalInfoIndirect));

    // Replace method with our own code
    SET_METHOD_FLAG(method, ACC_NATIVE);
    method->nativeFunc = &hookedMethodCallback;
    method->insns = (const u2*) hookInfo;
    method->registersSize = method->insSize;
    method->outsSize = 0;

    if (PTR_gDvmJit != NULL) {
        // reset JIT cache
        char currentValue = *((char*)PTR_gDvmJit + MEMBER_OFFSET_VAR(DvmJitGlobals,codeCacheFull));
        if (currentValue == 0 || currentValue == 1) {
            MEMBER_VAL(PTR_gDvmJit, DvmJitGlobals, codeCacheFull) = true;
        } else {
            ALOGE("Unexpected current value for codeCacheFull: %d", currentValue);
        }
    }
}


/**
 * Simplified copy of Method.invokeNative(), but calls the original (non-hooked) method
 * and has no access checks. Used to call the real implementation of hooked methods.
 */
void XposedBridge_invokeOriginalMethodNative(const u4* args, JValue* pResult,
            const Method* method, ::Thread* self) {
    Method* meth = (Method*) args[1];
    if (meth == NULL) {
        meth = dvmGetMethodFromReflectObj((Object*) args[0]);
        if (isMethodHooked(meth)) {
            meth = (Method*) meth->insns;
        }
    }
    ArrayObject* params = (ArrayObject*) args[2];
    ClassObject* returnType = (ClassObject*) args[3];
    Object* thisObject = (Object*) args[4]; // null for static methods
    ArrayObject* argList = (ArrayObject*) args[5];

    // invoke the method
    pResult->l = dvmInvokeMethod(thisObject, meth, argList, params, returnType, true);
    return;
}

void XposedBridge_setObjectClassNative(JNIEnv* env, jclass clazz, jobject objIndirect, jclass clzIndirect) {
    Object* obj = (Object*) dvmDecodeIndirectRef(dvmThreadSelf(), objIndirect);
    ClassObject* clz = (ClassObject*) dvmDecodeIndirectRef(dvmThreadSelf(), clzIndirect);
    if (clz->status < CLASS_INITIALIZED && !dvmInitClass(clz)) {
        ALOGE("Could not initialize class %s", clz->descriptor);
        return;
    }
    obj->clazz = clz;
}

void XposedBridge_dumpObjectNative(JNIEnv* env, jclass clazz, jobject objIndirect) {
    Object* obj = (Object*) dvmDecodeIndirectRef(dvmThreadSelf(), objIndirect);
    dvmDumpObject(obj);
}

jobject XposedBridge_cloneToSubclassNative(JNIEnv* env, jclass clazz, jobject objIndirect, jclass clzIndirect) {
    Object* obj = (Object*) dvmDecodeIndirectRef(dvmThreadSelf(), objIndirect);
    ClassObject* clz = (ClassObject*) dvmDecodeIndirectRef(dvmThreadSelf(), clzIndirect);

    jobject copyIndirect = env->AllocObject(clzIndirect);
    if (copyIndirect == NULL)
        return NULL;

    Object* copy = (Object*) dvmDecodeIndirectRef(dvmThreadSelf(), copyIndirect);
    size_t size = obj->clazz->objectSize;
    size_t offset = sizeof(Object);
    memcpy((char*)copy + offset, (char*)obj + offset, size - offset);

    if (IS_CLASS_FLAG_SET(clz, CLASS_ISFINALIZABLE))
        dvmSetFinalizable(copy);

    return copyIndirect;
}

jint XposedBridge_getRuntime(JNIEnv* env, jclass clazz) {
    return 1; // RUNTIME_DALVIK
}

void XposedBridge_removeClassFinalFlag(JNIEnv* env, jclass clazz, jclass clzIndirect) {
	ClassObject* clz = (ClassObject*) dvmDecodeIndirectRef(dvmThreadSelf(), clzIndirect);
	ALOGD("Current Class Flag %08x", clz->accessFlags);
	clz->accessFlags = clz->accessFlags & (~ACC_FINAL);
}





struct HookInfo {
    char *classDesc;
    char *methodName;
    char *methodSig;

    bool isStaticMethod;
    void *originalMethod;
    void *returnType;
    void *paramTypes;
    void *callback;
};
#define CHECK_VALID(V)              \
    if(V == NULL){                  \
        ALOGD("%s is null.", #V);    \
        exit(-1);                   \
    }else{                          \
        ALOGD("%s is %p.", #V, V);   \
    }
 jclass dvmFindJNIClass(JNIEnv *env, const char *classDesc) {
    jclass classObj = env->FindClass(classDesc);

    ALOGD("dvmFindJNIClass - Find Class");
    if (classObj == NULL) {
        ALOGD("dvmFindJNIClass - clazzApplicationLoaders1");
        jclass clazzApplicationLoaders = env->FindClass(
                "android/app/ApplicationLoaders");
        ALOGD("dvmFindJNIClass - clazzApplicationLoaders2");
        CHECK_VALID(clazzApplicationLoaders);
        ALOGD("dvmFindJNIClass - clazzApplicationLoaders");

        jfieldID fieldApplicationLoaders = env->GetStaticFieldID(
                clazzApplicationLoaders, "gApplicationLoaders",
                "Landroid/app/ApplicationLoaders;");
        CHECK_VALID(fieldApplicationLoaders);
        ALOGD("dvmFindJNIClass - fieldApplicationLoaders");

        jobject objApplicationLoaders = env->GetStaticObjectField(
                clazzApplicationLoaders, fieldApplicationLoaders);
        CHECK_VALID(objApplicationLoaders);
        ALOGD("dvmFindJNIClass - objApplicationLoaders");

        jfieldID fieldLoaders = env->GetFieldID(clazzApplicationLoaders,
                "mLoaders", "Ljava/util/Map;");
        CHECK_VALID(fieldLoaders);
        ALOGD("dvmFindJNIClass - fieldLoaders");

        jobject objLoaders = env->GetObjectField(objApplicationLoaders,
                fieldLoaders);
        CHECK_VALID(objLoaders);
        ALOGD("dvmFindJNIClass - objLoaders");

        jclass clazzHashMap = env->GetObjectClass(objLoaders);
        static jmethodID methodValues = env->GetMethodID(clazzHashMap, "values",
                "()Ljava/util/Collection;");
        jobject values = env->CallObjectMethod(objLoaders, methodValues);
        jclass clazzValues = env->GetObjectClass(values);
        static jmethodID methodToArray = env->GetMethodID(clazzValues,
                "toArray", "()[Ljava/lang/Object;");
        jobjectArray classLoaders = (jobjectArray) env->CallObjectMethod(values,
                methodToArray);

        int size = env->GetArrayLength(classLoaders);
        jstring param = env->NewStringUTF(classDesc);

        ALOGD("Call \n");
        for (int i = 0; i < size; i++) {
            ALOGD("Call %d \n",i);
            jobject classLoader = env->GetObjectArrayElement(classLoaders, i);
            jclass clazzCL = env->GetObjectClass(classLoader);
            static jmethodID loadClass = env->GetMethodID(clazzCL, "loadClass",
                    "(Ljava/lang/String;)Ljava/lang/Class;");
            classObj = (jclass) env->CallObjectMethod(classLoader, loadClass,
                    param);

            if (classObj != NULL) {
                break;
            }
        }
    }
    ALOGD("Exit \n");
    return (jclass) env->NewGlobalRef(classObj);
}

 ClassObject* dvmFindClass(const char *classDesc) {
    JNIEnv *env = env;//AndroidRuntime::getJNIEnv();
//    assert(env != NULL);

    char *newclassDesc = dvmDescriptorToName(classDesc);

    jclass jnicls = dvmFindJNIClass(env, newclassDesc);
    ClassObject *res =
            jnicls ?
                    static_cast<ClassObject*>(dvmDecodeIndirectRef(
                            dvmThreadSelf(), jnicls)) :
                    NULL;
    free(newclassDesc);
    return res;
}


 ArrayObject* dvmGetMethodParamTypes(const Method* method,
        const char* methodsig) {
    /* count args */
    size_t argCount = dexProtoGetParameterCount(&method->prototype);
    static ClassObject* java_lang_object_array = dvmFindSystemClass(
            "[Ljava/lang/Object;");

    /* allocate storage */
    ArrayObject* argTypes = dvmAllocArrayByClass(java_lang_object_array,
            argCount, ALLOC_DEFAULT);
    if (argTypes == NULL) {
        return NULL;
    }

    Object** argObjects = (Object**) argTypes->contents;
    const char *desc = (const char *) (strchr(methodsig, '(') + 1);

    /*
     * Fill in the array.
     */
    size_t desc_index = 0;
    size_t arg_index = 0;
    bool isArray = false;
    char descChar = desc[desc_index];

    while (descChar != ')') {

        switch (descChar) {
        case 'Z':
        case 'C':
        case 'F':
        case 'B':
        case 'S':
        case 'I':
        case 'D':
        case 'J':
            if (!isArray) {
                argObjects[arg_index++] = dvmFindPrimitiveClass(descChar);
                isArray = false;
            } else {
                char buf[3] = { 0 };
                memcpy(buf, desc + desc_index - 1, 2);
                argObjects[arg_index++] = dvmFindSystemClass(buf);
            }

            desc_index++;
            break;

        case '[':
            isArray = true;
            desc_index++;
            break;

        case 'L':
            int s_pos = desc_index, e_pos = desc_index;
            while (desc[++e_pos] != ';')
                ;
            s_pos = isArray ? s_pos - 1 : s_pos;
            isArray = false;

            size_t len = e_pos - s_pos + 1;
            char buf[128] = { 0 };
            memcpy((void *) buf, (const void *) (desc + s_pos), len);
            argObjects[arg_index++] = dvmFindClass(buf);
            desc_index = e_pos + 1;
            break;
        }

        descChar = desc[desc_index];
    }

    return argTypes;
}

int calcMethodArgsSize(const char* shorty) {
    int count = 0;

    /* Skip the return type. */
    shorty++;

    for (;;) {
        switch (*(shorty++)) {
        case '\0': {
            return count;
        }
        case 'D':
        case 'J': {
            count += 2;
            break;
        }
        default: {
            count++;
            break;
        }
        }
    }

    return count;
}

 u4 dvmPlatformInvokeHints(const char* shorty) {
    const char* sig = shorty;
    int padFlags, jniHints;
    char sigByte;
    int stackOffset, padMask;

    stackOffset = padFlags = 0;
    padMask = 0x00000001;

    /* Skip past the return type */
    sig++;

    while (true) {
        sigByte = *(sig++);

        if (sigByte == '\0')
            break;

        if (sigByte == 'D' || sigByte == 'J') {
            if ((stackOffset & 1) != 0) {
                padFlags |= padMask;
                stackOffset++;
                padMask <<= 1;
            }
            stackOffset += 2;
            padMask <<= 2;
        } else {
            stackOffset++;
            padMask <<= 1;
        }
    }

    jniHints = 0;

    if (stackOffset > DALVIK_JNI_COUNT_SHIFT) {
        /* too big for "fast" version */
        jniHints = DALVIK_JNI_NO_ARG_INFO;
    } else {
        assert((padFlags & (0xffffffff << DALVIK_JNI_COUNT_SHIFT)) == 0);
        stackOffset -= 2;           // r2/r3 holds first two items
        if (stackOffset < 0)
            stackOffset = 0;
        jniHints |= ((stackOffset + 1) / 2) << DALVIK_JNI_COUNT_SHIFT;
        jniHints |= padFlags;
    }

    return jniHints;
}

 int dvmComputeJniArgInfo(const char* shorty) {
    const char* sig = shorty;
    int returnType, jniArgInfo;
    u4 hints;

    /* The first shorty character is the return type. */
    switch (*(sig++)) {
    case 'V':
        returnType = DALVIK_JNI_RETURN_VOID;
        break;
    case 'F':
        returnType = DALVIK_JNI_RETURN_FLOAT;
        break;
    case 'D':
        returnType = DALVIK_JNI_RETURN_DOUBLE;
        break;
    case 'J':
        returnType = DALVIK_JNI_RETURN_S8;
        break;
    case 'Z':
    case 'B':
        returnType = DALVIK_JNI_RETURN_S1;
        break;
    case 'C':
        returnType = DALVIK_JNI_RETURN_U2;
        break;
    case 'S':
        returnType = DALVIK_JNI_RETURN_S2;
        break;
    default:
        returnType = DALVIK_JNI_RETURN_S4;
        break;
    }

    jniArgInfo = returnType << DALVIK_JNI_RETURN_SHIFT;

    hints = dvmPlatformInvokeHints(shorty);

    if (hints & DALVIK_JNI_NO_ARG_INFO) {
        jniArgInfo |= DALVIK_JNI_NO_ARG_INFO;
    } else {
        assert((hints & DALVIK_JNI_RETURN_MASK) == 0);
        jniArgInfo |= hints;
    }

    return jniArgInfo;
}

 void setTargetMethodNative(Method* method) {
    if (method != NULL) {
        int argsSize = calcMethodArgsSize(method->shorty);
        if (!dvmIsStaticMethod(method)) //·Ç¾²Ì¬·½·¨ÐèÒªÔö¼Ó²ÎÊý this
            argsSize++;

        SET_METHOD_FLAG(method, ACC_NATIVE);
        method->registersSize = method->insSize = argsSize;
        method->outsSize = 0;
        method->jniArgInfo = dvmComputeJniArgInfo(method->shorty);
    }
}

int hook_in_dalvik_java(JNIEnv* env,const char* class_path_string,const char* method_name_string,const char* method_signal_string,bool is_static_method) {
    const char* classDesc = class_path_string;
    const char* methodName = method_name_string;
    const char* methodSig = method_signal_string;
    const bool isStaticMethod = true;

    ALOGD("Init HookInfo \n");
    HookInfo* info=(HookInfo*)malloc(sizeof(info));
    memset(info,0,sizeof(info));
    info->classDesc=(char*)classDesc;
    info->methodName=(char*)methodName;
    info->methodSig=(char*)methodSig;
    info->isStaticMethod=isStaticMethod;
    jclass classObj=env->FindClass(classDesc);

    if (NULL==classObj) {
        ALOGD("Get Class object is NULL");
        return -1;
    }

    jmethodID methodId=NULL;
    ALOGD("Get Method ID \n");
    if (!is_static_method) {
        methodId = env->GetMethodID(classObj, methodName, methodSig);

        if (methodId == NULL) {
            ALOGD("[-] %s->%s method not found", classDesc, methodName);
            return -1;
        }
    } else {
        methodId = env->GetStaticMethodID(classObj, methodName, methodSig);

        if (methodId == NULL) {
            ALOGD("[-] %s->%s method not found", classDesc, methodName);
            return -1;
        }
    }

    ALOGD("Get Static Method .. \n");
    Method* method = (Method*) methodId;
    info->isStaticMethod = dvmIsStaticMethod(method);
    if (method->nativeFunc == hookedMethodCallback) {
        ALOGD("[*] %s->%s method had been hooked", classDesc, methodName);
        free(info);
        return -1;
    }

    ALOGD("Get Method Information .. \n");
    Method* bakMethod = (Method*) malloc(sizeof(Method));
    memcpy(bakMethod, method, sizeof(Method));

    // init info
    info->originalMethod = (void *) bakMethod;
    info->returnType = (void *) dvmGetBoxedReturnType(bakMethod);
    info->paramTypes = dvmGetMethodParamTypes(bakMethod, info->methodSig);

    ALOGD("Set Method to Native .. \n");
    // set method native
    setTargetMethodNative(method);

    // save info to insns
    method->insns = (u2*) info;

    // bind the bridge func£¬only one line
    method->nativeFunc = hookedMethodCallback;
    ALOGD("[+] %s->%s was hooked\n", classDesc, methodName);

    return 0;
}
}









static jstring char_to_jstring(JNIEnv* env, const char* string) {
    return env->NewStringUTF(string); 
}

static char* jstring_to_char(JNIEnv *env, jstring jstr) {
    return (char*)env->GetStringUTFChars(jstr,NULL);
}

static jobject get_method(JNIEnv* jni_env,jclass find_class,const char* find_method_name) {
    ALOGD("Get Class ..");
    jclass class_class = jni_env->FindClass("java/lang/Class");
    if (NULL==class_class) {    
        ALOGD("Find Class ERROR! \n");
        return NULL;
    }

    ALOGD("Get Method ..");
    jclass class_method = jni_env->FindClass("java/lang/reflect/Method");
    if (NULL==class_method) {    
        ALOGD("Find Method ERROR! \n");
        return NULL;
    }

    ALOGD("Get Class Method getMethod() ..");
    jmethodID getmethod=(jmethodID)jni_env->GetMethodID(class_class,"getMethods","()[Ljava/lang/reflect/Method;");
    if (NULL==getmethod) {
        ALOGD("Get Class Method getMethod() ERROR! \n");
        return NULL;
    }

    ALOGD("Get Class Method getName() ..");
    jmethodID get_name=(jmethodID)jni_env->GetMethodID(class_method,"getName","()Ljava/lang/String;");
    if (NULL==get_name) {
        ALOGD("Get Class Method getName() ERROR! \n");
        return NULL;
    }

    if (jni_env->ExceptionCheck()) {
        jni_env->ExceptionDescribe();
        ALOGD("WARNING! Exception ..");
        return NULL;
    }

    ALOGD("Get get_method_list ..");
    jobject get_method_list=jni_env->CallObjectMethod(find_class,getmethod);
    if (NULL==get_method_list) {
        ALOGD("Get get_method_list ERROR ..");
        return NULL;
    }

    int array_length=jni_env->GetArrayLength((jarray)get_method_list);
    ALOGD("get_method_list length=%d",array_length);

    for (int array_index=0;array_index<array_length;++array_index) {  //  get method
        jobject method_index=(jobject)jni_env->GetObjectArrayElement((jobjectArray)get_method_list,array_index);
        const char* method_name=jstring_to_char(jni_env,(jstring)jni_env->CallObjectMethod(method_index,get_name));

        ALOGD("find index %d - %s",array_index,method_name);
        if (!strcmp(find_method_name,method_name)) {
            ALOGD("Return Method ..");
            return (jobject)method_index;
        }
    }
    return NULL;
}

static void* load_jar(JNIEnv* jni_env,const char* jar_path,const char* find_class) {
    ALOGD("Find ClassLoader \n");
    jclass classloaderClass = jni_env->FindClass("java/lang/ClassLoader");
    if (NULL==classloaderClass) {    
        ALOGD("Find ClassLoader ERROR! \n");
        return NULL;
    }

    ALOGD("Get SystemClassLoader \n");
    jmethodID getsysloaderMethod = jni_env->GetStaticMethodID(classloaderClass, "getSystemClassLoader","()Ljava/lang/ClassLoader;");
    if (NULL==getsysloaderMethod) {    
        ALOGD("Get getSystemClassLoader ERROR! \n");
        return NULL;
    }
    ALOGD("Call getSystemClassLoader() \n");
    jobject loader =jni_env->CallStaticObjectMethod(classloaderClass,getsysloaderMethod);  //  var classLoader=Class.getSystemClassLoader();

    ALOGD("Build Jar/ODEX JString.. \n");
    jstring dexpath = char_to_jstring(jni_env,jar_path);
    jstring dex_odex_path = char_to_jstring(jni_env,"/data/data/com.tencent.mm/app_recover_lib/");
    jstring hook_class_name= char_to_jstring(jni_env,find_class);

    ALOGD("Find DexClassLoader \n");
    jclass dexLoaderClass = jni_env->FindClass("dalvik/system/DexClassLoader");
    if (NULL==dexLoaderClass) {    
        ALOGD("Find DexClassLoader ERROR! \n");
        return NULL;
    }
    
    ALOGD("Get DexClassLoader Construtor Function \n");
    jmethodID initDexLoaderMethod =jni_env->GetMethodID(dexLoaderClass, "<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    if (NULL==initDexLoaderMethod) {    
        ALOGD("Get DexClassLoader Construtor Function ERROR! \n");
        return NULL;
    }
    ALOGD("Get LoadClass \n");
    jmethodID loadclassMethod = jni_env->GetMethodID(dexLoaderClass,"loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    if(NULL==loadclassMethod) {
        ALOGD("Get LoadClass ERROR !..");
        return NULL;
    }

    ALOGD("Init DexClassLoader \n");
    jobject dexLoader = jni_env->NewObject(dexLoaderClass,initDexLoaderMethod, dexpath, dex_odex_path, NULL, loader);  //  new DexClassLoader();
    if (NULL==dexLoader) {
        ALOGD("Init DexClassLoader ERROR! \n");
        return NULL;
    }

    ALOGD("DexClassLoader=%X \n",dexLoader);
    ALOGD("Ready to loadClass() ..");
    jclass hook_class=(jclass)jni_env->CallObjectMethod(dexLoader,loadclassMethod,hook_class_name);  //  DexClassLoader.loadClass();

    if (jni_env->ExceptionCheck()) {
        jni_env->ExceptionDescribe();
        ALOGD("WARNING! Exception ..");                                                                        
        return NULL;
    }

    if (NULL==hook_class) {
        ALOGD("loadClass hook_class ERROR! \n");
        return NULL;
    }
    ALOGD("Return Class ..");
    return (void*)hook_class;
}



extern "C" int Java_com_tencent_mm_sdk_platformtools_CrashMonitorForJni_setClientVersionMsg(int result,int a2,int a3) {
    ALOGD("set Client Version ..");
    return 0;
}

static bool dvmRegisterJNIMethod(Method* method, void* fnPtr)
{
    if (fnPtr == NULL) {
        return false;
    }

    if (!dvmIsNativeMethod(method)) {
        ALOGE("This isn't Native Method ..");
        return false;
    }

    if (method->nativeFunc != dvmResolveNativeMethod) {
        ALOGV("Note : it was already registered");
    }

    method->fastJni = false;
    dvmUseJNIBridge(method, fnPtr);

    ALOGV("JNI-registered ..");
    return true;
}

