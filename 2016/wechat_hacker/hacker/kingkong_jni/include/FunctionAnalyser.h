
#ifndef _HOT_PATCH_FUNCTION_ANALYSER_H_
#define _HOT_PATCH_FUNCTION_ANALYSER_H_
#pragma once

#include <vector>
#include "ElfAnalyser.h"
#include "Instruction.h"
#include "CPUStatus.h"
#include "AddressHelper.h"
#include "Common.h"

class KK_EXPORT FunctionAnalyser {
public:

	FunctionAnalyser(unsigned int startAddress, unsigned int maxLength, AddressHelper *addressHelper, ElfAnalyser *elfAnalyser);
	virtual ~FunctionAnalyser();

	void runInstructionAnalyser();
	void clearInstructionVector();

	Instruction *findCallFunction(const char *funcName, unsigned int index);

	unsigned int collectInstructions(int type, char *buffer, unsigned int bufferLen);

protected:
	unsigned int startAddress;
	unsigned int maxLength;

	AddressHelper *addressHelper;
	ElfAnalyser *elfAnalyser;

	std::vector<Instruction *> instructions;
	CPUStatus *cpu;
};

#endif
