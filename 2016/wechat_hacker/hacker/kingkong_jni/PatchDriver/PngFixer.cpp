#include <jni.h>

#include <dlfcn.h>

#include "Instruction.h"
#include "Patch.h"
#include "Common.h"
#include "AddressHelper.h"
#include "ElfAnalyser.h"

#include "PngFunctionAnalyser.h"

#define WORKER_COUNT 2
#define INTREST_FUNCTION_COUNT 7

#define LIBRARY_NAME "/system/lib/libandroid_runtime.so"
#define LIBRARY_NAME_ANDROID_FW	"/system/lib/libandroidfw.so"

static const char *gIntrestFunctionList[INTREST_FUNCTION_COUNT]
                                        = {"__cxa_finalize",
										   "strcmp",
										   "_ZN7android14Res_png_9patch14serializedSizeEv",
										   "malloc",
										   "memcpy",
										   "_ZN7android14Res_png_9patch11deserializeEPKv",
										   "_ZN7android14Res_png_9patch12fileToDeviceEv"};

static const char *gIntrestFunctionListLollipop[INTREST_FUNCTION_COUNT]
                                        = {"__cxa_finalize",
										   "strcmp",
										   "_ZNK7android14Res_png_9patch14serializedSizeEv",	// For AndroidVersion > 19
										   "malloc",
										   "memcpy",
										   "_ZN7android14Res_png_9patch11deserializeEPv",		// For AndroidVersion > 19
										   "_ZN7android14Res_png_9patch12fileToDeviceEv"};

static AddressHelper *gAddressHelper = NULL;
static AddressHelper *gAddressHelperAndroidFw = NULL;
static unsigned int gBaseAddress = 0;
static unsigned int gBaseAddressAndroidFw = 0;
static unsigned int gPeek = 0;
static unsigned int gSerializedSize = 0;
static ElfAnalyser *gElfAnalyser = NULL;
static PngFunctionAnalyser *gPngAnalyser = NULL;
static SerializedSizeAnalyser *gSerializedSizeAnalyser = NULL;
static IntrestFunction *gIntrestFunction = NULL;

static bool initializer(PatchModule *patchModule, unsigned int *result) {

	if (!holdStrongReference(LIBRARY_NAME)) {
		return false;
	}
	if (!holdStrongReference(LIBRARY_NAME_ANDROID_FW)) {
		return false;
	}

	gAddressHelper = new AddressHelper(LIBRARY_NAME);
	if (!gAddressHelper || gAddressHelper->getBaseAddress() == 0) return false;
	gBaseAddress = gAddressHelper->getBaseAddress();

	gAddressHelperAndroidFw = new AddressHelper(LIBRARY_NAME_ANDROID_FW);
	if (!gAddressHelperAndroidFw || gAddressHelperAndroidFw->getBaseAddress() == 0) return false;
	gBaseAddressAndroidFw = gAddressHelperAndroidFw->getBaseAddress();

	gPeek = findSymbol(LIBRARY_NAME, "_ZN15NinePatchPeeker4peekEPKcPKvj");
	if (gPeek == 0) {
		LOGD("Unable to find peek in %s", LIBRARY_NAME);
		return false;
	}

	if (getAndroidVersion() > 19) {
		gSerializedSize = findSymbol(LIBRARY_NAME_ANDROID_FW, "_ZNK7android14Res_png_9patch14serializedSizeEv");
	} else {
		gSerializedSize = findSymbol(LIBRARY_NAME_ANDROID_FW, "_ZN7android14Res_png_9patch14serializedSizeEv");
	}

	if (gSerializedSize == 0) {
		LOGD("Unable to find serializedSize in %s", LIBRARY_NAME_ANDROID_FW);
		return false;
	}

	gSerializedSize = gSerializedSize & 0xFFFFFFFE;
	gPeek = gPeek & 0xFFFFFFFE;

	return true;
}

static bool parameterHunter(PatchModule *patchModule, unsigned int *result) {

	if (getAndroidVersion() > 19)
		gIntrestFunction = new IntrestFunction(gIntrestFunctionListLollipop, INTREST_FUNCTION_COUNT);
	else
		gIntrestFunction = new IntrestFunction(gIntrestFunctionList, INTREST_FUNCTION_COUNT);

	if (!gIntrestFunction) return false;

	gElfAnalyser = new ElfAnalyser(LIBRARY_NAME, gIntrestFunction);
	if (!gElfAnalyser || !gElfAnalyser->analyse()) return false;

	gPngAnalyser = new PngFunctionAnalyser(gPeek, 256, gAddressHelper, gElfAnalyser);
	if (!gPngAnalyser) return false;
	if (!gPngAnalyser->runFunctionAnalyser(patchModule->parameters)) return false;

	gSerializedSizeAnalyser = new SerializedSizeAnalyser(gSerializedSize, 50, gAddressHelperAndroidFw, NULL);
	if (!gSerializedSizeAnalyser) return false;
	if (!gSerializedSizeAnalyser->runFunctionAnalyser(patchModule->parameters)) return false;

	patchModule->jumperAddr = 0;	// Don't calculate jumper address

	patchModule->hookPoint = patchModule->parameters[PNG_PARAM_HOOK_POINT];

	return true;

}

static pfnPatchWorker gPngPatchWorker[WORKER_COUNT] = {
	initializer,
	parameterHunter
};

KK_EXPORT bool GeneratePatch(PatchModule *patch)
{
	if (patch == NULL) return false;

	if (patch->parameterCount != PNG_PARAM_MAX) {
		LOGD("Generate Patch failed : wrong parameter count!");
		return false;
	}

	bool success = true;
	unsigned int result = 0;
	for (int i = 0; i < WORKER_COUNT; i++) {
		if (!gPngPatchWorker[i](patch, &result)) {
			LOGD("Give up patch work for worker %d failed!", i);
			success = false;
			break;
		}
	}

	// TODO : Move to a proper place
	// It's a best practice that set point to NULL after delete
	if (gPngAnalyser) {
		delete gPngAnalyser;
		gPngAnalyser = NULL;
	}
	if (gAddressHelper) {
		delete gAddressHelper;
		gAddressHelper = NULL;
	}
	if (gAddressHelperAndroidFw) {
		delete gAddressHelperAndroidFw;
		gAddressHelperAndroidFw = NULL;
	}
	if (gElfAnalyser) {
		delete gElfAnalyser;
		gElfAnalyser = NULL;
	}
	if (gSerializedSizeAnalyser) {
		delete gSerializedSizeAnalyser;
		gSerializedSizeAnalyser = NULL;
	}
	if (gIntrestFunction) {
		delete gIntrestFunction;
		gIntrestFunction = NULL;
	}

	return success;
}
