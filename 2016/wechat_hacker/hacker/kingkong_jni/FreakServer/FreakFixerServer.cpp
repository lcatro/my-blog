
#include "jni.h"
#include "Common.h"
#include "AddressHelper.h"
#include "Patch.h"

extern bool GeneratePatch(PatchModule *patch);

extern "C" jintArray Java_com_tencent_kingkongserver_FreakFixerServer_nativeGetParams
	(JNIEnv *env, jobject thiz)
{
	jintArray result = NULL;

	if (!holdStrongReference("libFreakFixer.so")) {
		return NULL;
	}

	FP_GeneratePatch generatePatch = GeneratePatch;
	if (generatePatch == NULL) {
		return NULL;
	}

	PatchModule patch = {0};
	jint params[FREAK_PARAM_MAX];

	patch.parameters = (unsigned int *)params;
	patch.parameterCount = FREAK_PARAM_MAX;

	if (!generatePatch(&patch)) {
		return NULL;
	}

	for (int i = 0; i < PNG_PARAM_MAX; i++) {
		LOGD("Parameter %d --> value 0x%08x", i, params[i]);
	}

	unsigned ssl3_get_key_exchange = findSymbol("/system/lib/libssl.so", "ssl3_get_key_exchange");
	if (!ssl3_get_key_exchange) return NULL;

	ssl3_get_key_exchange = ssl3_get_key_exchange & 0xFFFFFFFE;

/*
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
 *
 */

	params[FREAK_PARAM_HOOK_POINT] -= ssl3_get_key_exchange;
	params[FREAK_PARAM_RSA_NEW_ADDRESS] = 0;
	params[FREAK_PARAM_EXIT_TRUE] -= ssl3_get_key_exchange;
	params[FREAK_PARAM_EXIT_FALSE] -= ssl3_get_key_exchange;

	result = env->NewIntArray(FREAK_PARAM_MAX);
	if (!result) return NULL;

	env->SetIntArrayRegion(result, 0, FREAK_PARAM_MAX, params);
	return result;
}

