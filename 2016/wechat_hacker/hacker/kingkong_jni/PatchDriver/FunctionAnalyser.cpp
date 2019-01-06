#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "FunctionAnalyser.h"
#include "Common.h"
#include "AddressHelper.h"

FunctionAnalyser::FunctionAnalyser(UINT32 startAddress, UINT32 maxLength, AddressHelper *addressHelper, ElfAnalyser *elfAnalyser)
{
	this->startAddress = startAddress & 0xFFFFFFFE;
	this->maxLength = maxLength;
	this->addressHelper = addressHelper;
	this->elfAnalyser = elfAnalyser;

	cpu = new CPUStatus(startAddress);

	runInstructionAnalyser();
}

FunctionAnalyser::~FunctionAnalyser()
{
	if (cpu) {
		delete cpu;
		cpu = NULL;
	}
	clearInstructionVector();
}

void FunctionAnalyser::clearInstructionVector()
{
	for (UINT32 i = 0; i < instructions.size(); i++) {
		Instruction *pInstruction = instructions[i];
		delete pInstruction;
	}
	instructions.clear();
}

Instruction *FunctionAnalyser::findCallFunction(const char *funcName, unsigned int index)
{
	unsigned int count = 0;

	for (std::vector<Instruction *>::iterator
			it = instructions.begin(); it != instructions.end(); ++it) {

		Instruction *pInstruction = *it;
		if (pInstruction->type == BRANCH_WITH_LINK_LABEL) {
			if (pInstruction->destFuncName.compare(funcName) == 0) {
				count++;
				LOGD("Found %d function call to %s at 0x%08x %s", count, funcName, pInstruction->pc, pInstruction->ASMStr.c_str());
				if (count == index) return pInstruction;
			}
		}
	}

	LOGD("Unable to found %d call to %s", index, funcName);

	return NULL;
}

void FunctionAnalyser::runInstructionAnalyser()
{

	// Currently we can determinate three types of functions
	// 1. 	PUSH LR
	//	POP PC
	// 2.      PUSH LR
	//	...
	//	PUSH LR 	<< Regard this PUSH LR as start of a new function
	// 3.	... 	<< There is no push LR
	//	B LR 		<< Branch directly to the LR
	// 4.	PUSH LR
	//	...
	//	POP PC 		<< Pop PC in the middle of the function
	// 	...			<< There should be no PUSH LR, or I'll think the function is finished
	//	POP PC  	<< Currently our tolerance is 1 non-end Pop PC

	LOGD("Running Instruction Analyser at 0x%08x", cpu->PC);
	UINT32 i = 0;
	bool pushLR = false;
	UINT32 popPCCount = 0;
	UINT32 maxPopPCCount = 1;	//	TODO : Change to a parameter!!!!
	while (i < maxLength) {
		if (!addressHelper->checkAddress(cpu->PC, true, false, true)) {
			LOGD("Quit, because of Instruction Address unreachable!");
			break;
		}
		Instruction *pInstruction = InstructionAnalyser::analyse(cpu, true);
		if (pInstruction != NULL) {
			i +=  pInstruction->length;;
			cpu->PC += pInstruction->length;
			instructions.push_back(pInstruction);
			if (pInstruction->type == STACK_PUSH
				&& pInstruction->isPushLR) {
				if (pushLR) {
					LOGD("Found Push LR at 0x%08x again, quit at offset = %d",
						pInstruction->pc, i);
					break;
				} else {
					pushLR = true;
					LOGD("Found Push LR at 0x%08x, offset = %d",
						pInstruction->pc, i);
				}
			}
			if (pInstruction->type == STACK_POP
				&& pInstruction->isPopPC
				&& pushLR) {
				popPCCount++;
				LOGD("Found Pop PC at 0x%08x, offset = %d, PopPC count is %d",
					pInstruction->pc, i, popPCCount);
				if (popPCCount >= maxPopPCCount) {
					LOGD("Encounter max Pop PC count, quit");
					break;
				}
			}
			if (pInstruction->type == BRANCH_WITHOUT_LINK_REGISTER
				&& pInstruction->Rm == REGISTER_LR) {
				if (!pushLR) {
					LOGD("Found BX LR, quit at 0x%08x, offset = %d",
						pInstruction->pc, i);
					break;
				} else {
					LOGD("Ignore BX LR at 0x%08x, offset = %d, because LR pushed",
						pInstruction->pc, i);
				}
			}
			if (pInstruction->type == LDR_LITERAL) {
				if (!addressHelper->checkAddress(pInstruction->imm32, true, false, false)) {
					LOGD("Quit, because of LDR data Address unreachable!");
					break;
				} else {
					UINT32 t = pInstruction->Rt;
					cpu->registers[t] = MEMU4(pInstruction->imm32);
					cpu->registerStatus[t] = REGISTER_IMMEDIATE;
					LOGD("Calculate LDRLiteral Value = 0x%08x", cpu->registers[t]);
				}
			}
			if (pInstruction->type == BRANCH_WITH_LINK_LABEL && elfAnalyser) {
				std::string name = elfAnalyser->FindPltNameByOffset(pInstruction->label - addressHelper->getBaseAddress());
				if (!name.empty()) {
					LOGD("BL %s at 0x%08x", name.c_str(), pInstruction->pc);
					pInstruction->destFuncName = name;
				}
			}
		} else {
			i += 2;
			cpu->PC += 2;
		}
	}
	LOGD("Instruction Analyser Exit");
}

unsigned int FunctionAnalyser::collectInstructions(int type, char *buffer, unsigned int bufferLen)
{
	// Collect position-independent instructions for function SHA1 calculation

	unsigned int len = 0;
	unsigned int baseAddress = addressHelper->getBaseAddress();
	unsigned int offset = 0;

	switch (type) {
	case FINGERPRINT_TYPE_A:
		for (std::vector<Instruction *>::iterator
				it = instructions.begin(); it != instructions.end(); ++it) {
			Instruction *pInstruction = *it;
			if (pInstruction->type == STACK_PUSH
					|| pInstruction->type == STACK_POP
					|| pInstruction->type == BRANCH_WITHOUT_LINK
					|| pInstruction->type == BRANCH_WITHOUT_LINK_REGISTER
					|| pInstruction->type == MOV_REGISTER
					|| pInstruction->type == MOV_IMMEDIATE
					|| pInstruction->type == CBNZ
					|| pInstruction->type == ADD_REGISTER
					|| pInstruction->type == SUB_IMMEDIATE
					|| pInstruction->type == LDR_REGISTER
					|| pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE) {
				if (len + 4 + pInstruction->length <= bufferLen) {
					offset = pInstruction->pc - startAddress;
					memcpy(buffer + len, &offset, 4);
					memcpy(buffer + len + 4, (void *)(pInstruction->pc), pInstruction->length);
					len = len + pInstruction->length + 4;
				} else {
					LOGD("Buffer overflow when calculating SHA1!");
					return 0;
				}
			}
		}
		break;
	case FINGERPRINT_TYPE_B:
		for (std::vector<Instruction *>::iterator
				it = instructions.begin(); it != instructions.end(); ++it) {
			Instruction *pInstruction = *it;
			if (pInstruction->type == BRANCH_WITHOUT_LINK
					|| pInstruction->type == BRANCH_WITHOUT_LINK_REGISTER
					|| pInstruction->type == LDR_REGISTER
					|| pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE) {
				if (len + 4 + pInstruction->length <= bufferLen) {
					offset = pInstruction->pc - startAddress;
					memcpy(buffer + len, &offset, 4);
					memcpy(buffer + len + 4, (void *)(pInstruction->pc), pInstruction->length);
					len = len + pInstruction->length + 4;
				} else {
					LOGD("Buffer overflow when calculating SHA1!");
					return 0;
				}
			}
		}
		break;
	case FINGERPRINT_TYPE_C:
		for (std::vector<Instruction *>::iterator
				it = instructions.begin(); it != instructions.end(); ++it) {
			Instruction *pInstruction = *it;
			if (pInstruction->type == BRANCH_WITHOUT_LINK
					|| pInstruction->type == BRANCH_WITHOUT_LINK_REGISTER
					|| pInstruction->type == LDR_REGISTER
					|| pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE) {
				if (len + 4 + pInstruction->length <= bufferLen) {
					offset = pInstruction->pc - startAddress;
					memcpy(buffer + len, &offset, 4);
					memcpy(buffer + len + 4, (void *)(pInstruction->pc), pInstruction->length);
					len = len + pInstruction->length + 4;
				} else {
					LOGD("Buffer overflow when calculating SHA1!");
					return 0;
				}
			} else if (pInstruction->type == STACK_PUSH
					|| pInstruction->type == STACK_POP
					|| pInstruction->type == MOV_REGISTER
					|| pInstruction->type == MOV_IMMEDIATE
					|| pInstruction->type == CBNZ
					|| pInstruction->type == ADD_REGISTER
					|| pInstruction->type == SUB_IMMEDIATE) {
				if (len + 4 + pInstruction->length <= bufferLen) {
					memcpy(buffer + len + 4, (void *)(pInstruction->pc), pInstruction->length);
					len = len + pInstruction->length;
				} else {
					LOGD("Buffer overflow when calculating SHA1!");
					return 0;
				}
			}
		}
		break;
	}
	return len;
}
