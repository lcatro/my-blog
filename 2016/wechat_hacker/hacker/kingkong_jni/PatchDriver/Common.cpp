
#include <jni.h>
#include <dlfcn.h>

#include "Instruction.h"
#include "Patch.h"
#include "Common.h"
#include "AddressHelper.h"
#include "FunctionAnalyser.h"

#include "CachedLibUtil.h"

#define JAVA_CLASS_COMMON					"com/tencent/kingkong/Common"
#define JAVA_METHOD_COMMON_NOTIFICATION 	"NotificationFromNative"
#define JAVA_PARAM_COMMON_NOTIFICATION		"(III)V"

static JavaVM *g_jvm = NULL;
static jobject g_JavaFreakFixerObject = NULL;
static unsigned int g_AndroidVersion = 19;

unsigned int getAndroidVersion() {
	return g_AndroidVersion;
}

// Cache Java object
// We can only call this from a Java thread
void initClassHelper(JNIEnv *env, const char *path, jobject *objptr)
{
	jclass cls = env->FindClass(path);
	if(!cls) {
		LOGE("initClassHelper: failed to get %s class reference", path);
		env->ExceptionClear();
		return;
	}
	jmethodID constr = env->GetMethodID(cls, "<init>", "()V");
	if(!constr) {
		LOGE("initClassHelper: failed to get %s constructor", path);
		env->ExceptionClear();
		return;
	}
	jobject obj = env->NewObject(cls, constr);
	if(!obj) {
		LOGE("initClassHelper: failed to create a %s object", path);
		env->ExceptionClear();
		return;
	}
	// Important, NewGlobalRef, so we can access it anywhere
	(*objptr) = env->NewGlobalRef(obj);
}

void *NotificationToJavaThread(void *args)
{
	int type = ((UINT32 *)args)[0];
	int param1 = ((UINT32 *)args)[1];
	int param2 = ((UINT32 *)args)[2];
	JNIEnv* env = NULL;
	bool isAttached = false;
	jclass NativeSafeWebViewClient = NULL;
	jmethodID NotificationFromNative = NULL;

	if (g_jvm == NULL) {
		LOGE("g_JVM == NULL");
        return 0;
    }

    if (g_JavaFreakFixerObject == NULL) {
    	LOGE("g_JavaFreakFixerObject == NULL");
    	return 0;
    }

    int status = g_jvm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (status != JNI_OK) {
    	status = g_jvm->AttachCurrentThread(&env, NULL);
       	if (status != JNI_OK) {
	     	LOGE("Attach current thread to JVM failed");
           	return 0;
       	}
       	isAttached = true;
    }

    jclass javaFreakFixerClass = env->GetObjectClass(g_JavaFreakFixerObject);
    if (!javaFreakFixerClass) {
	   	LOGE("Unable to find FreakFixer class");
	   	env->ExceptionClear();
	   	goto bail;
    }

	NotificationFromNative =  env->GetStaticMethodID
					(javaFreakFixerClass, JAVA_METHOD_COMMON_NOTIFICATION, JAVA_PARAM_COMMON_NOTIFICATION);
	if (NotificationFromNative == NULL) {
		LOGE("Unable to find NotificationFromNative");
		env->ExceptionClear();
		goto bail;
	}

	env->CallStaticVoidMethod(javaFreakFixerClass, NotificationFromNative, type, param1, param2);

	if (env->ExceptionOccurred()) {
		env->ExceptionClear();
	}

bail:
	if (isAttached) {
		g_jvm->DetachCurrentThread();
	}

	return 0;
}

void NotificationToJava(int type, int index, UINT32 value)
{
	pthread_t tid;
	void *status;
	UINT32 *args = new UINT32[3];
	if (args == NULL) {
		return;
	}
	args[0] = type;
	args[1] = index;
	args[2] = value;
	pthread_create(&tid, NULL, NotificationToJavaThread, args);
	pthread_join(tid, &status);
	delete []args;
}

bool holdStrongReference(const char *library)
{
	// dlopen the library and never release it!

	void *handle = CachedLibUtil::openLib(library);
	if (!handle) {
		LOGD("Hold strong reference for %s failed!", library);
		return false;
	}
	return true;
}

unsigned int findSymbol(const char *library, const char *symbolName)
{
	void *handle = CachedLibUtil::openLib(library);
	if (!handle) {
		LOGE("Unable to dlopen %s", library);
		return 0;
	} else {
		void *symbol = dlsym(handle, symbolName);
		if (!symbol) {
			LOGE("Unable to find symbol %s", symbolName);
			return 0;
		} else {
			LOGD("Find %s address 0x%08x in %s", library, (unsigned int)symbol, symbolName);
			return (unsigned int)symbol;
		}
	}
}

void __clear_cache(unsigned int begin, unsigned int end)
{
    const int syscall = 0xf0002;
    __asm __volatile (
        "mov     r0, %0\n"
        "mov     r1, %1\n"
        "mov     r7, %2\n"
        "mov     r2, #0x0\n"
        "svc     0x00000000\n"
        :
        :    "r" (begin), "r" (end), "r" (syscall)
        :    "r0", "r1", "r7"
        );
}

// Called by *FixerASM.s
void reportEvilCatched()
{
	LOGD("Evil Catched!");
	NotificationToJava(NOTIFICATION_TYPE_EVIL_CATCHED, 0, 0);
}

// Called by *FixerASM.s
void reportCxaFinalizeCalled()
{
	LOGD("__cxa_finalize Called!");
	NotificationToJava(NOTIFICATION_TYPE_CXAFINALIZE_CALLED, 0, 0);
}

// Called by *FixerASM.s
void reportFixerErrorOccurred()
{
	LOGD("Encounter error :(");
	NotificationToJava(NOTIFICATION_TYPE_FIXER_ERROR, 0, 0);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;

    LOGD("KKFixer JNI_OnLoad() invoked, Version %s", KINGKONG_FIXER_VERSION);

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    	LOGE("GetEnv() failed");

        return -1;
    }

    g_jvm = vm;

    initClassHelper(env, JAVA_CLASS_COMMON, &g_JavaFreakFixerObject);

    return JNI_VERSION_1_4;
}

JNIEXPORT jint Java_com_tencent_kingkong_PatchManager_nativeCalcJumperPoint
	(JNIEnv *env, jobject thiz, jstring jstrLibraryName)
{
	const char *intrestFunctionList[] = {"__cxa_finalize"};
	IntrestFunction *intrestFunction = NULL;
	ElfAnalyser *elfAnalyser = NULL;
	jint _cxa_finalize = -1;
	const char *libraryName = NULL;
	AddressHelper *addressHelper = NULL;

	libraryName = env->GetStringUTFChars(jstrLibraryName, NULL);
	if (libraryName == NULL) goto bail;

	intrestFunction = new IntrestFunction(intrestFunctionList, arraysize(intrestFunctionList));
	if (!intrestFunction) goto bail;

	elfAnalyser = new ElfAnalyser(libraryName, intrestFunction);
	if (!elfAnalyser || !elfAnalyser->analyse()) goto bail;

	_cxa_finalize = (jint)elfAnalyser->FindPltOffsetByName("__cxa_finalize");
	if (_cxa_finalize == 0) {
		LOGD("Unable to find __cxa_finalize in plt table!");
		goto bail;
	}

	if ((_cxa_finalize & BINARY(1, 1)) != 0) {
		LOGE("Why NOT the address of _cxa_finalize aligned to 4 !?");
		goto bail;
	}

	addressHelper = new AddressHelper(libraryName);
	if (!addressHelper || addressHelper->getBaseAddress() == 0) {
		goto bail;
	}

	_cxa_finalize +=  addressHelper->getBaseAddress();

	goto OK;

bail:
	_cxa_finalize = -1;
OK:
	if (intrestFunction) delete intrestFunction;
	if (elfAnalyser) delete elfAnalyser;
	if (libraryName) env->ReleaseStringUTFChars(jstrLibraryName, libraryName);
	if (addressHelper) delete addressHelper;

	return _cxa_finalize;
}

JNIEXPORT jint Java_com_tencent_kingkong_PatchManager_nativeCalcParameter
	(JNIEnv *env, jobject thiz, jint type, jstring jstrValue1, jstring jstrValue2, jint value3)
{
	const char *libraryName = NULL;
	const char *functionName = NULL;
	AddressHelper *addressHelper = NULL;
	unsigned int value = -1;
	unsigned int func = 0;
	unsigned int baseAddress = 0;

	if ((type <= PARAM_TYPE_MIN) && (type >= PARAM_TYPE_MAX)) {
		return -1;
	}

	if (type == PARAM_TYPE_ARBITRARY_VALUE) return value3;

	libraryName = env->GetStringUTFChars(jstrValue1, NULL);
	functionName = env->GetStringUTFChars(jstrValue2, NULL);
	if ((libraryName == NULL) || (functionName == NULL)) {
		return -1;
	}

	LOGD("Calc parameter : %d, %s, %s, %d", type, libraryName, functionName, value3);

	if (!holdStrongReference(libraryName)) {
		goto bail;
	}

	addressHelper = new AddressHelper(libraryName);
	if (!addressHelper) goto bail;
	baseAddress = addressHelper->getBaseAddress();
	if (baseAddress == 0) goto bail;

	if (type == PARAM_TYPE_OFFSET_TO_FUNCTION) {
		func = findSymbol(libraryName, functionName);
		if (func == 0) goto bail;
		func = func & 0xFFFFFFFE;
		value = func + (unsigned int) value3;

		if (value < func) {
			LOGD("Unexpected value 0x%08x < 0x%08x, base = 0x%08x", value, func, baseAddress);
			goto bail;
		}
		if (!(addressHelper->checkAddress(value, true, false, false))) {
			LOGD("Unable to access the address 0x%08x, base = 0x%08x", value, baseAddress);
			goto bail;
		}
	} else if (type == PARAM_TYPE_OFFSET_TO_FILE) {
		value = baseAddress + (unsigned int) value3;

		if (value < baseAddress) {
			LOGD("Unexpected value 0x%08x < base 0x%08x", value, baseAddress);
			goto bail;
		}
		if (!(addressHelper->checkAddress(value, true, false, false))) {
			LOGD("Unable to access the address 0x%08x, base = 0x%08x", value, baseAddress);
			goto bail;
		}
	} else if (type == PARAM_TYPE_SYMBOL_ADDRESS) {
		value = findSymbol(libraryName, functionName);
		if (value == 0) {
			goto bail;
		}
	} else {
		goto bail;
	}

	goto OK;

bail:
	value = -1;
OK:
	if (addressHelper != NULL) {
		delete addressHelper;
	}
	if (libraryName != NULL) {
		env->ReleaseStringUTFChars(jstrValue1, libraryName);
	}
	if (functionName != NULL) {
		env->ReleaseStringUTFChars(jstrValue2, functionName);
	}
	return value;
}

JNIEXPORT jbyteArray Java_com_tencent_kingkong_PatchManager_nativeGetFingerprint
	(JNIEnv *env, jobject thiz, jstring jstrLibraryName, jstring jstrFunctionName, jint type)
{
	UINT32 fingerprintLen = 0;
	char *buffer = NULL;
	const char *libraryName = NULL;
	const char *functionName = NULL;
	FunctionAnalyser *analyser = NULL;
	AddressHelper *addressHelper = NULL;
	jbyteArray bInstruction = 0;
	UINT32 func = 0;
	UINT32 baseAddress = 0;

	libraryName = env->GetStringUTFChars(jstrLibraryName, NULL);
	functionName = env->GetStringUTFChars(jstrFunctionName, NULL);

	if ((libraryName == NULL) || (functionName == NULL)) goto bail;

	LOGD("Calculate fingerprint of %s, %s", libraryName, functionName);

	if (!holdStrongReference(libraryName)) goto bail;

	addressHelper = new AddressHelper(libraryName);
	if (!addressHelper) goto bail;
	baseAddress = addressHelper->getBaseAddress();
	if (baseAddress == 0) goto bail;

	func = findSymbol(libraryName, functionName);
	if (func == 0) goto bail;

	func = func & 0xFFFFFFFE;

	analyser = new FunctionAnalyser(func, 10 * 1024, addressHelper, NULL);
	if (!analyser)	goto bail;

	buffer = (char *)malloc(FINGERPRINT_BUFFER_SIZE);
	if (!buffer) goto bail;

	fingerprintLen = analyser->collectInstructions(type, buffer, FINGERPRINT_BUFFER_SIZE);

	LOGD("Fingerprint length %d", fingerprintLen);

done:
bail:

	bInstruction = env->NewByteArray(fingerprintLen);
	if (bInstruction != 0) {
		env->SetByteArrayRegion(bInstruction, 0, fingerprintLen, (jbyte *)buffer);
	} else {
		env->ExceptionClear();
	}

	if (buffer) free(buffer);
	if (analyser) delete analyser;
	if (addressHelper) delete addressHelper;
	if (libraryName != NULL) {
		env->ReleaseStringUTFChars(jstrLibraryName, libraryName);
	}
	if (functionName != NULL) {
		env->ReleaseStringUTFChars(jstrFunctionName, functionName);
	}
	return bInstruction;
}

JNIEXPORT jboolean Java_com_tencent_kingkong_PatchManager_nativeVerifyPatchParams
	(JNIEnv *env, jobject thiz, jstring jstrSoFileName, jint hookPoint,
			jint jumperPoint, jintArray jintParams, jint paramCount)
{
#ifdef ENABLE_PNG_FIXER
	bool success = false;
	const char *soFileName = NULL;
	PatchModule patch = {0};
	FP_GeneratePatch fixerGeneratePatch = NULL;
	unsigned int generateParams[paramCount];
	jint *params = NULL;

	patch.parameters = generateParams;
	patch.parameterCount = paramCount;

	soFileName = env->GetStringUTFChars(jstrSoFileName, NULL);
	params = env->GetIntArrayElements(jintParams, NULL);
	if (soFileName == NULL || params == NULL) {
		goto bail;
	}

	fixerGeneratePatch = GeneratePatch;

	if (!fixerGeneratePatch(&patch)) {
		LOGD("Generate patch failed");
		goto bail;
	}

	if (patch.hookPoint != hookPoint) {
		LOGD("Generate patch got wrong hookPoint (0x%08x, 0x%08x)",
				patch.hookPoint, hookPoint);
		goto bail;
	}

	for (unsigned int i = 0; i < paramCount; i++) {
		if (params[i] != generateParams[i]) {
			LOGD("Generate patch got wrong parameter at index %d (0x%08x, 0x%08x)",
					i, params[i], generateParams[i]);
			goto bail;
		}
	}

	success = true;

bail:
	if (params != NULL) {
		env->ReleaseIntArrayElements(jintParams, params, 0);
	}
	if (soFileName != NULL) {
		env->ReleaseStringUTFChars(jstrSoFileName, soFileName);
	}
	return success;

#else
	return true;
#endif
}

JNIEXPORT jboolean Java_com_tencent_kingkong_PatchManager_nativeDoPatch
	(JNIEnv *env, jobject thiz, jstring jstrSoFileName, jstring jstrLibraryName, jstring jstrFunctionName, jint hookPoint,
			jint jumperPoint, jintArray jintParams, jint paramCount)
{
	const char *libraryName = NULL;
	const char *functionName = NULL;
	const char *soFileName = NULL;
	jint *params = NULL;
	unsigned int fixerParameterStart = 0;
	unsigned int fixerParameterEnd = 0;
	unsigned int fixerEntry = 0;
	AddressHelper *addressHelper = NULL;
	PatchModule patch = {0};
	bool success = false;

	unsigned int driverFuncs[] = {0, 0, 0, 0, 0};
	unsigned int driverFuncCount = arraysize(driverFuncs);

	unsigned int fixerDriverFuncStart = 0;
	unsigned int fixerDriverFuncEnd = 0;
	unsigned int fixerDriverFuncReportEvilCatched = 0;
	unsigned int fixerDriverFuncReportCxaFinalizeCalled = 0;
	unsigned int fixerDriverFuncreportFixerErrorOccurred = 0;
	unsigned int __cxa_finalize = 0;
	unsigned int fixerDriverFunc_cxa_finalize = 0;
	unsigned int fixerDriverFunc_original_LR = 0;

	soFileName = env->GetStringUTFChars(jstrSoFileName, NULL);
	libraryName = env->GetStringUTFChars(jstrLibraryName, NULL);
	functionName = env->GetStringUTFChars(jstrFunctionName, NULL);
	params = env->GetIntArrayElements(jintParams, NULL);

	if ((libraryName == NULL) || (functionName == NULL)
			|| (soFileName == NULL) || (params == NULL)) goto bail;

	LOGD("Do Patch : %s, %s, %s, 0x%08x, 0x%08x, %d",
			soFileName, libraryName, functionName, hookPoint, jumperPoint, paramCount);
	for (int i = 0; i < paramCount; i++) {
		LOGD("Parameter %d : 0x%08x", i, params[i]);
	}

	if (!holdStrongReference(soFileName) || (!holdStrongReference(libraryName))) {
		goto bail;
	}

	addressHelper = new AddressHelper(libraryName);
	if (!addressHelper || (addressHelper->getBaseAddress() == 0)) {
		goto bail;
	}

	if (!(addressHelper->checkAddress(hookPoint, true, false, false)
			&& addressHelper->checkAddress(jumperPoint, true, false, false))) {
		LOGD("Check Address for hookPoint or jumperPoint failed");
		goto bail;
	}

	fixerParameterStart = findSymbol(soFileName, "FIXER_PARAMETERS_START");
	fixerParameterEnd = findSymbol(soFileName, "FIXER_PARAMETERS_END");
	fixerEntry = findSymbol(soFileName, "FIXER_ENTRY_CODE32");
	fixerDriverFuncStart = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_START");
	fixerDriverFuncEnd = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_END");
	fixerDriverFuncReportEvilCatched = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_reportEvilCatched");
	fixerDriverFuncReportCxaFinalizeCalled = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_reportCxaFinalizeCalled");
	fixerDriverFuncreportFixerErrorOccurred = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_reportFixerErrorOccurred");
	__cxa_finalize = findSymbol("/system/lib/libc.so", "__cxa_finalize");
	fixerDriverFunc_cxa_finalize = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_cxa_finalize");
	fixerDriverFunc_original_LR = findSymbol(soFileName, "PATCH_DRIVER_FUNCTION_original_LR");
	if ((fixerParameterStart == 0) || (fixerParameterEnd == 0) || (fixerEntry == 0)
			|| fixerDriverFuncStart == 0 || fixerDriverFuncEnd == 0
			|| fixerDriverFuncReportEvilCatched == 0
			|| fixerDriverFuncReportCxaFinalizeCalled == 0
			|| fixerDriverFuncreportFixerErrorOccurred == 0
			|| __cxa_finalize == 0
			|| fixerDriverFunc_cxa_finalize == 0
			|| fixerDriverFunc_original_LR == 0) {
		goto bail;
	}

	if ((fixerParameterEnd - fixerParameterStart) != (paramCount * 4)
			|| (fixerDriverFuncEnd - fixerDriverFuncStart) != (driverFuncCount * 4)
			|| !(fixerDriverFuncReportEvilCatched == fixerDriverFuncStart
					&& fixerDriverFuncReportCxaFinalizeCalled > fixerDriverFuncReportEvilCatched
					&& fixerDriverFuncreportFixerErrorOccurred > fixerDriverFuncReportCxaFinalizeCalled
					&& fixerDriverFunc_cxa_finalize > fixerDriverFuncreportFixerErrorOccurred
					&& fixerDriverFunc_original_LR > fixerDriverFunc_cxa_finalize
					&& fixerDriverFuncEnd > fixerDriverFunc_original_LR)) {
		LOGD("Verify parameter count failed!");
		goto bail;
	}

	patch.hookPoint = hookPoint;
	patch.jumperAddr = jumperPoint;
	patch.parameterAddr = fixerParameterStart;
	patch.codeEntry32 = fixerEntry;
	patch.parameters = (unsigned int *)params;
	patch.parameterCount = paramCount;

	patch.patchWorkerCount = 0;
	patch.fnPatchWorker = NULL;

	driverFuncs[0] = (unsigned int)reportEvilCatched;
	driverFuncs[1] = (unsigned int)reportCxaFinalizeCalled;
	driverFuncs[2] = (unsigned int)reportFixerErrorOccurred;
	driverFuncs[3] = __cxa_finalize;
	driverFuncs[4] = hookPoint + 4 + 1;		// The next instruction of hookPoint, with Thumb mode
	patch.driverFuncAddr = fixerDriverFuncStart;
	patch.driverFuncs = driverFuncs;
	patch.driverFuncCount = driverFuncCount;

	patch.fgParametersReady = true;

	if (PatcherDoPatch(&patch)) {
		success = true;
	}

bail:
	if (soFileName != NULL) {
		env->ReleaseStringUTFChars(jstrSoFileName, soFileName);
	}
	if (libraryName != NULL) {
		env->ReleaseStringUTFChars(jstrLibraryName, libraryName);
	}
	if (functionName != NULL) {
		env->ReleaseStringUTFChars(jstrFunctionName, functionName);
	}
	if (params != NULL) {
		env->ReleaseIntArrayElements(jintParams, params, 0);
	}
	if (addressHelper != NULL) {
		delete addressHelper;
	}

	return success;
}
