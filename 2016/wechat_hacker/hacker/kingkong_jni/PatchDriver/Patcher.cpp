
#include "Patch.h"
#include "Instruction.h"
#include "Common.h"
#include "AddressHelper.h"
#include "ElfAnalyser.h"

KK_EXPORT bool PatcherDoPatch(PatchModule *patchModule)
{
	unsigned int result = 0;
	for (int i = 0; i < patchModule->patchWorkerCount; i++) {
		if (!patchModule->fnPatchWorker[i](patchModule, &result)) {
			LOGD("Give up patch work for worker %d failed!", i);
			return false;
		}
	}

	// 1 Modify the hook point to
	//		BLX _cxa_finalize
	// 2 Modify _cxa_finalize to :
	//		LDR PC, DESTINATION	(2 bytes)
	//		NOP					(2 bytes)
	//		DESTINATION 		(4 bytes)
	// _cxa_finalize should have been aligned to 4
	//
	// Notice : If your destination is thumb code, should +1 to the address
	// 			LDR PC will also change the mode according to the last bit
	//

	unsigned int BLXAddr = patchModule->hookPoint;
	unsigned int LdrAddr = patchModule->jumperAddr;
	unsigned int BLXCode = InstructionMaker::makeBLX(BLXAddr, LdrAddr, false);
	if (BLXCode == 0) {
		return false;
	}

	unsigned int LdrDataAddr = LdrAddr + 4;
	unsigned int LdrData = patchModule->codeEntry32;
	unsigned int LdrCode = InstructionMaker::makeLdrReg(REG_PC, LdrAddr, LdrDataAddr);
	if (LdrCode == 0) {
		return false;
	}

	if (!AddressHelper::makeWritable(BLXAddr)
		|| !AddressHelper::makeWritable(LdrAddr)
		|| !AddressHelper::makeWritable(LdrDataAddr)
		|| !AddressHelper::makeWritable(patchModule->parameterAddr)
		|| !AddressHelper::makeWritable(patchModule->driverFuncAddr)) {

		return false;
	}

	PATCH_DWORD(LdrAddr, LdrCode);
	LOGD("Patch Jumper 0x%08x -> 0x%08x", LdrAddr, LdrCode);
	PATCH_DWORD(LdrDataAddr, LdrData);
	LOGD("Patch Jumper data 0x%08x -> 0x%08x", LdrDataAddr, LdrData);
	__clear_cache(LdrAddr, LdrAddr + 8);

	for (int i = 0; i < patchModule->parameterCount; i++) {
		unsigned int addr = patchModule->parameterAddr + sizeof(int) * i;
		unsigned int value = patchModule->parameters[i];
		LOGD("Patch Parameter Word 0x%08x -> 0x%08x", addr, value);
		PATCH_DWORD(addr, value);
	}
	__clear_cache(patchModule->parameterAddr,
				  patchModule->parameterAddr + patchModule->parameterCount * 4);

	for (int i = 0; i < patchModule->driverFuncCount; i++) {
		unsigned int addr = patchModule->driverFuncAddr + sizeof(int) * i;
		unsigned int value = patchModule->driverFuncs[i];
		LOGD("Patch driver function address Word 0x%08x -> 0x%08x", addr, value);
		PATCH_DWORD(addr, value);
	}
	__clear_cache(patchModule->driverFuncAddr,
				  patchModule->driverFuncAddr + patchModule->driverFuncCount * 4);

	PATCH_DWORD(BLXAddr, BLXCode);
	LOGD("Patch Hook Point 0x%08x -> 0x%08x", BLXAddr, BLXCode);

	// +16, workaround for instruction optimization issue (STR --> STMIA)
	__clear_cache(BLXAddr, BLXAddr + 16);

	return true;
}
