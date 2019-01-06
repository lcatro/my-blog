#include <jni.h>

#include <dlfcn.h>

#include "Instruction.h"
#include "Patch.h"
#include "Common.h"
#include "AddressHelper.h"
#include "ElfAnalyser.h"
#include "FunctionAnalyser.h"
#include <sys/system_properties.h>

#define WORKER_COUNT 2

#define LIBRARY_NAME "/system/lib/libssl.so"

extern unsigned int FIXER_ENTRY_CODE32;
extern unsigned int FIXER_PARAMETERS;

AddressHelper *gAddressHelper = NULL;
unsigned int gBaseAddress = 0;
ElfAnalyser *gElfAnalyser = NULL;
FreakFunctionAnalyser *gFreakAnalyser = NULL;
unsigned int gSsl3_get_key_exchange = 0;
unsigned int gRSA_new = 0;
unsigned int g_cxa_finalize = 0;

bool gFreakSafeStatus = false;

bool initializer(PatchModule *patchModule, unsigned int *result) {

	if (!holdStrongReference(LIBRARY_NAME)) {
		return false;
	}

	gAddressHelper = new AddressHelper(LIBRARY_NAME);
	if (!gAddressHelper || gAddressHelper->getBaseAddress() == 0) return false;
	gBaseAddress = gAddressHelper->getBaseAddress();

	gSsl3_get_key_exchange = findFunction("/system/lib/libssl.so", "ssl3_get_key_exchange");
	if (gSsl3_get_key_exchange == 0) {
		NotificationToJava(NOTIFICATION_TYPE_DO_PATCH_ERROR, DO_PATCH_ERROR_NO_GET_CHANGE_FOUND, 0);
		return false;
	}

	gRSA_new = findFunction("/system/lib/libcrypto.so", "RSA_new");
	if (gRSA_new == 0) {
		NotificationToJava(NOTIFICATION_TYPE_DO_PATCH_ERROR, DO_PATCH_ERROR_NO_RSA_NEW_FOUND, 0);
		return false;
	}

	return true;
}

bool parameterHunter(PatchModule *patchModule, unsigned int *result) {

	if (patchModule->fgParametersReady) {

		if (patchModule->hookPoint < gBaseAddress) {

			patchModule->hookPoint += gBaseAddress;
			patchModule->jumperAddr += gBaseAddress;

			patchModule->parameters[PARAM_RSA_NEW_ADDRESS] = gRSA_new;
			patchModule->parameters[PARAM_EXIT_TRUE] = patchModule->hookPoint + 4 + 1;	// +1 for thumb mode
			patchModule->parameters[PARAM_EXIT_FALSE] = patchModule->hookPoint + 4 + 1; // +1 for thumb mode

		}
		return true;

	} else {

		gElfAnalyser = new ElfAnalyser(LIBRARY_NAME);
		if (!gElfAnalyser || !gElfAnalyser->analyse()) return false;

		g_cxa_finalize = gElfAnalyser->FindPltOffsetByName("__cxa_finalize");
		if (g_cxa_finalize == 0) {
			NotificationToJava(NOTIFICATION_TYPE_DO_PATCH_ERROR, DO_PATCH_ERROR_NO_FINALIZE_FOUND, 0);
			LOGD("Unable to find __cxa_finalize in plt table!");
			return false;
		}
		g_cxa_finalize += gBaseAddress;

		if ((g_cxa_finalize & BINARY(1, 1)) != 0) {
			LOGE("Why NOT the address of _cxa_finalize aligned to 4 !?");
			return false;
		}

		patchModule->jumperAddr = g_cxa_finalize;

		patchModule->parameters[PARAM_RSA_NEW_ADDRESS] = gRSA_new;

		gFreakAnalyser = new FreakFunctionAnalyser(gSsl3_get_key_exchange, 0x1500, gAddressHelper, gElfAnalyser);

		if (!gFreakAnalyser) return false;

		if (!gFreakAnalyser->runFunctionAnalyser(patchModule->parameters)) return false;

		patchModule->hookPoint = patchModule->parameters[PARAM_HOOK_POINT];

		return true;
	}

}

unsigned int parameters[PARAM_MAX];
pfnPatchWorker gFreakPatchWorker[WORKER_COUNT] = {
	initializer,
	parameterHunter
};

PatchModule gFreakPatch = {

	0,					// 	unsigned int hookPoint
	0, 					//	unsigned int jumperAddr;
	(unsigned int) &FIXER_PARAMETERS, 	//	unsigned int parameterAddr;
	(unsigned int) &FIXER_ENTRY_CODE32,	//	unsigned int codeEntry32;
	parameters,			//	unsigned int *parameters;
	PARAM_MAX, 			//	unsigned int parameterCount;

	WORKER_COUNT,		//	unsigned int patchWorkerCount;
	gFreakPatchWorker,	//	pfnPatchWorker *fnPatchWorker;

	false				//	fgParametersReady
};

JNIEXPORT jboolean Java_com_tencent_kingkong_FreakFixer_nativeSetSafeStatus
	(JNIEnv *env, jobject thiz, jboolean status)
{

	if (!gFreakSafeStatus) {
		if (status) {
		    if (PatcherDoPatch(&gFreakPatch)) {
		    	LOGD("Great, everything is OK~");
		    	if (!gFreakPatch.fgParametersReady) {
		    		gFreakPatch.hookPoint -= gBaseAddress;
		    		gFreakPatch.jumperAddr -= gBaseAddress;
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1, true, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1_ONLY_B_AND_LDR, true, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1_B_AND_LDR_AND_OTHERS_WITHOUT_OFFSET, true, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
		    	}
		    } else {
		    	LOGD("Bad luck, wanna try again ?~");
		    	if (!gFreakPatch.fgParametersReady) {
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1, false, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1_ONLY_B_AND_LDR, false, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
			    	report(env, NOTIFICATION_TYPE_FUNCTION_SHA1_B_AND_LDR_AND_OTHERS_WITHOUT_OFFSET, false, gAddressHelper, gFreakAnalyser, &gFreakPatch, gBaseAddress);
		    	}
		    }
		}
		gFreakSafeStatus = true;

		// TODO : Move to a proper place
		if (gFreakAnalyser) delete gFreakAnalyser;
		if (gElfAnalyser) delete gElfAnalyser;
		if (gAddressHelper) delete gAddressHelper;
	}

	return true;
}

JNIEXPORT void Java_com_tencent_kingkong_FreakFixer_nativeSetParameters
	(JNIEnv *env, jobject thiz, jint v0, jint v1, jint v2, jint v3, jint v4, jint v5)
{

	gFreakPatch.hookPoint = v0;
	gFreakPatch.jumperAddr = v1;
	gFreakPatch.parameters[PARAM_SSL_REGISTER] = v2;
	gFreakPatch.parameters[PARAM_SSL_OFFSET1] = v3;
	gFreakPatch.parameters[PARAM_SSL_OFFSET2] = v4;
	gFreakPatch.parameters[PARAM_SSL_OFFSET3] = v5;

	gFreakPatch.fgParametersReady = true;
}

