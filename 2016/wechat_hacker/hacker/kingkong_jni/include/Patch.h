#ifndef _HOT_PATCH_PATCH_H__
#define _HOT_PATCH_PATCH_H__
#pragma once

#include "Common.h"

struct PatchModuleStruct;

typedef bool (*pfnPatchWorker) (struct PatchModuleStruct *patchModule, unsigned int *result);

typedef struct PatchModuleStruct {

	// Address to be patched
	unsigned int hookPoint;
	unsigned int jumperAddr;
	unsigned int parameterAddr;

	// Value
	unsigned int codeEntry32;		// Entry in ARM mode
	unsigned int *parameters;
	unsigned int parameterCount;

	unsigned int patchWorkerCount;
	pfnPatchWorker *fnPatchWorker;

	unsigned int driverFuncAddr;
	unsigned int *driverFuncs;
	unsigned int driverFuncCount;

	bool fgParametersReady;

} PatchModule;

KK_EXPORT bool PatcherDoPatch(PatchModule *patchModule);

#endif
