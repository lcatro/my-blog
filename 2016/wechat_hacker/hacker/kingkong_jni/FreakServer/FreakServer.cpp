
#include "jni.h"
#include "Common.h"
#include "AddressHelper.h"

extern "C" jintArray Java_com_tencent_kingkongserver_FreakServer_nativeGetParams
	(JNIEnv *env, jobject thiz)
{

	jintArray result = NULL;

	if (!holdStrongReference("libPngFixer.so")) {
		return NULL;
	}

	FP_GeneratePatch generatePatch = (FP_GeneratePatch)findSymbol("libkkfixerdriver.so", "GeneratePatch");
	if (generatePatch == NULL) {
		return NULL;
	}

	PatchModule patch = {0};
	jint params[PNG_PARAM_MAX];

	patch.parameters = (unsigned int *)params;
	patch.parameterCount = PNG_PARAM_MAX;

	if (!generatePatch(&patch)) {
		return NULL;
	}

	for (int i = 0; i < PNG_PARAM_MAX; i++) {
		LOGD("Parameter %d --> value 0x%08x", i, params[i]);
	}

	unsigned peek = findSymbol("/system/lib/libandroid_runtime.so", "_ZN15NinePatchPeeker4peekEPKcPKvj");
	if (!peek) return NULL;

	peek = peek & 0xFFFFFFFE;

/*
 *enum {
	PNG_PARAM_HOOK_POINT = 0,	// Filled in PngFunctionAnalyser
	PNG_PARAM_EXIT_TRUE,
	PNG_PARAM_EXIT_FALSE,
	PNG_PARAM_REGISTER,

	PNG_PARAM_MAX
};
 *
 */

	params[PNG_PARAM_HOOK_POINT] -= peek;
	params[PNG_PARAM_EXIT_TRUE] -= peek;
	params[PNG_PARAM_EXIT_FALSE] -= peek;

	result = env->NewIntArray(PNG_PARAM_MAX);
	if (!result) return NULL;

	env->SetIntArrayRegion(result, 0, PNG_PARAM_MAX, params);
	return result;
}

