#include "Common.h"
#include "AddressHelper.h"
#include "Instruction.h"
#include <dlfcn.h>

static void clearCache(unsigned int begin, unsigned int end)
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

bool testOtherbodyCallCxaFinalize()
{
	unsigned int jumperAddr = 0xB6C;

	void *handle = dlopen("libtestfile.so", RTLD_NOW);

	AddressHelper *addressHelper = new AddressHelper("libtestfile.so");
	if (handle == NULL || !addressHelper || addressHelper->getBaseAddress() == 0) {
		LOGD("Unable to open libtestfile.so");
		return false;
	}

	jumperAddr += (addressHelper->getBaseAddress());
	LOGD("Jumper Address 0x%08x ", jumperAddr);

	unsigned int fixerEntry = findSymbol("/data/data/com.example.freaktest/files/patches/libPngFixer.so", "FIXER_ENTRY_CODE32");
	if (fixerEntry == 0) {
		LOGD("Unable to find FIXER_ENTRY_CODE32 in /data/data/com.example.freaktest/files/patches/libPngFixer.so");
		return false;
	}

	unsigned int LdrAddr = jumperAddr;

	unsigned int LdrDataAddr = LdrAddr + 4;
	unsigned int LdrData = fixerEntry;
	unsigned int LdrCode = InstructionMaker::makeLdrReg(REG_PC, LdrAddr, LdrDataAddr);

	if (LdrCode == 0) {
		LOGD("Make LDR code failed!");
		return false;
	}

	PATCH_DWORD(LdrAddr, LdrCode);
	LOGD("Patch Jumper 0x%08x -> 0x%08x", LdrAddr, LdrCode);
	PATCH_DWORD(LdrDataAddr, LdrData);
	LOGD("Patch Jumper data 0x%08x -> 0x%08x", LdrDataAddr, LdrData);

	clearCache(LdrAddr, LdrAddr + 8);

	dlclose(handle);
}

extern "C" jboolean Java_com_tencent_kingkong_UnitTest_nativeDoUnitTest()
{
	testOtherbodyCallCxaFinalize();
}
