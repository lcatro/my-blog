#include <jni.h>

#include <dlfcn.h>

#include "Instruction.h"
#include "Patch.h"
#include "Common.h"
#include "AddressHelper.h"
#include "ElfAnalyser.h"
#include "FreakFunctionAnalyser.h"
#include <sys/system_properties.h>

#define WORKER_COUNT 2

#define LIBRARY_NAME "/system/lib/libssl.so"

static AddressHelper *gAddressHelper = NULL;
static unsigned int gBaseAddress = 0;
static ElfAnalyser *gElfAnalyser = NULL;
static FreakFunctionAnalyser *gFreakAnalyser = NULL;
static unsigned int gSsl3_get_key_exchange = 0;
static unsigned int gRSA_new = 0;
static unsigned int g_cxa_finalize = 0;
static IntrestFunction *gIntrestFunction = NULL;

static const char *gIntrestFunctionList[]
                                        = {"__cxa_finalize",
										   "RSA_new",
										   "EC_KEY_get0_group",
										   "EC_GROUP_get_degree",
										   "memcpy"};
static const unsigned int INTREST_FUNCTION_COUNT = arraysize(gIntrestFunctionList);

static bool initializer(PatchModule *patchModule, unsigned int *result) {

	if (!holdStrongReference(LIBRARY_NAME)) {
		return false;
	}

	gAddressHelper = new AddressHelper(LIBRARY_NAME);
	if (!gAddressHelper || gAddressHelper->getBaseAddress() == 0) return false;
	gBaseAddress = gAddressHelper->getBaseAddress();

	gSsl3_get_key_exchange = findSymbol("/system/lib/libssl.so", "ssl3_get_key_exchange");
	if (gSsl3_get_key_exchange == 0) {
		return false;
	}

	gRSA_new = findSymbol("/system/lib/libcrypto.so", "RSA_new");
	if (gRSA_new == 0) {
		return false;
	}

	return true;
}

static bool parameterHunter(PatchModule *patchModule, unsigned int *result) {

	gIntrestFunction = new IntrestFunction(gIntrestFunctionList, INTREST_FUNCTION_COUNT);
	if (!gIntrestFunction) return false;

    gElfAnalyser = new ElfAnalyser(LIBRARY_NAME, gIntrestFunction);
    if (!gElfAnalyser || !gElfAnalyser->analyse()) return false;

    g_cxa_finalize = gElfAnalyser->FindPltOffsetByName("__cxa_finalize");
    if (g_cxa_finalize == 0) {
        LOGD("Unable to find __cxa_finalize in plt table!");
        return false;
    }
    g_cxa_finalize += gBaseAddress;

    if ((g_cxa_finalize & BINARY(1, 1)) != 0) {
        LOGE("Why NOT the address of _cxa_finalize aligned to 4 !?");
        return false;
    }

    patchModule->jumperAddr = g_cxa_finalize;

    patchModule->parameters[FREAK_PARAM_RSA_NEW_ADDRESS] = gRSA_new;

	gSsl3_get_key_exchange = gSsl3_get_key_exchange & 0xFFFFFFFE;
    gFreakAnalyser = new FreakFunctionAnalyser(gSsl3_get_key_exchange, 0x1500, gAddressHelper, gElfAnalyser);

    if (!gFreakAnalyser) return false;

    if (!gFreakAnalyser->runFunctionAnalyser(patchModule->parameters)) return false;

    patchModule->hookPoint = patchModule->parameters[FREAK_PARAM_HOOK_POINT];

    return true;

}

static pfnPatchWorker gFreakPatchWorker[WORKER_COUNT] = {
	initializer,
	parameterHunter
};

KK_EXPORT bool GeneratePatch(PatchModule *patch)
{
	if (patch == NULL) return false;

	if (patch->parameterCount != FREAK_PARAM_MAX) {
		LOGD("Generate Patch failed : wrong parameter count!");
		return false;
	}

	bool success = true;
	unsigned int result = 0;
	for (int i = 0; i < WORKER_COUNT; i++) {
		if (!gFreakPatchWorker[i](patch, &result)) {
			LOGD("Give up patch work for worker %d failed!", i);
			success = false;
			break;
		}
	}

    // TODO : Move to a proper place
    if (gFreakAnalyser) {
        delete gFreakAnalyser;
		gFreakAnalyser = NULL;
	}
	if (gElfAnalyser) {
        delete gElfAnalyser;
        gElfAnalyser = NULL;
	}
    if (gAddressHelper) {
        delete gAddressHelper;
        gAddressHelper = NULL;
    }
    if (gIntrestFunction) {
    	delete gIntrestFunction;
    	gIntrestFunction = NULL;
    }
	return success;
}
