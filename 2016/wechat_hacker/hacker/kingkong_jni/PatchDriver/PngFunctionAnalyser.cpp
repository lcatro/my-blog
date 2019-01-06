
#include "ElfAnalyser.h"
#include "Instruction.h"
#include "CPUStatus.h"
#include "PngFunctionAnalyser.h"

bool PngFunctionAnalyser::runFunctionAnalyser(unsigned int *result)
{
	LOGD("Running Png Peek Function Analyser...");

	if (!cpu) return false;

	runInstructionAnalyser();

	Instruction *call_strcmp = findCallFunction("strcmp", 1);
	Instruction *call_malloc = findCallFunction("malloc", 1);
	Instruction *call_memcpy = findCallFunction("memcpy", 1);
	Instruction *call_fileToDevice = findCallFunction("_ZN7android14Res_png_9patch12fileToDeviceEv", 1);
	Instruction *call_serializedSize = findCallFunction(
				getAndroidVersion() > 19 ?
					"_ZNK7android14Res_png_9patch14serializedSizeEv" :
					"_ZN7android14Res_png_9patch14serializedSizeEv",
				1);
	Instruction *call_deserialize = findCallFunction(
				getAndroidVersion() > 19 ?
					"_ZN7android14Res_png_9patch11deserializeEPv" :
					"_ZN7android14Res_png_9patch11deserializeEPKv",
				1);


	if (!call_strcmp || !call_serializedSize || !call_malloc
			|| !call_memcpy || !call_deserialize || !call_fileToDevice) {
		return false;
	}

	if (!(call_strcmp->pc < call_serializedSize->pc && call_serializedSize->pc < call_malloc->pc && call_malloc->pc < call_memcpy->pc
			&& call_memcpy->pc < call_deserialize->pc && call_deserialize->pc < call_fileToDevice->pc)) {
		LOGD("Wrong calling sequence, give up!");
		return false;
	}

	unsigned int status = 0;
	unsigned int reg = 0;
	for (std::vector<Instruction *>::iterator
			it = instructions.begin(); it != instructions.end(); ++it) {

		Instruction *pInstruction = *it;

		switch (status) {
		case 0:
			if (pInstruction->type == STACK_PUSH) {
				status++;
				LOGD("Stack push, status --> 1");
			} else {
				LOGD("No stack push found!");
				return false;
			}
		case 1:
			if (pInstruction->type == MOV_REGISTER
					&& pInstruction->Rm == REG_R3) {
				reg = pInstruction->Rd;
				if (reg < REG_R4 || reg > REG_R12) {
					LOGE("Invalid destination register");
					return false;
				} else {
					status++;
					LOGD("reg = %d, status --> 2", reg);
				}
			}
			if (pInstruction == call_strcmp) {
				LOGE("Unable to find MOV Rn, R3");
				return false;
			}
			break;
		case 2:
			if (pInstruction->type == MOV_REGISTER
					&& pInstruction->Rd == reg) {
				LOGE("Illegal MOV %d, %d ", reg, pInstruction->Rm);
				return false;
			}

			if (pInstruction == call_serializedSize) {
				status++;
				LOGD("Arrived at serializedSize, status --> 3 ");
			}
			break;
		case 3:
			if (pInstruction == call_fileToDevice) {
				status++;
				LOGD("Arrived at serializedSize, status --> 4");
			}
			break;
		case 4:
			if (pInstruction->type == MOV_IMMEDIATE
					&& pInstruction->Rd == REG_R0
					&& pInstruction->imm32 == 1) {
				status++;
				LOGD("Arrived at MOV R0, #1, status --> 5");
			} else if (pInstruction->type == ADD_SP_IMMEDIATE
					|| pInstruction->type == STACK_POP) {
				LOGE("Give up for unexpected instruction");
				return false;
			}
			break;
		case 5:
			if (pInstruction->type == STACK_POP) {

				/*
				 enum {
					PNG_PARAM_HOOK_POINT = 0,	// Filled in PngFunctionAnalyser
					PNG_PARAM_EXIT_TRUE,
					PNG_PARAM_EXIT_FALSE,
					PNG_PARAM_REGISTER,

					PNG_PARAM_OFFSET1,			// Filled in SerializedSizeAnalyser
					PNG_PARAM_OFFSET2,
					PNG_PARAM_OFFSET3,
				};
				 */
				result[PNG_PARAM_HOOK_POINT] = call_serializedSize->pc;
				result[PNG_PARAM_EXIT_TRUE] = call_serializedSize->pc + 4 + 1; 	// +1 for thumb mode, the next instruction of BLX erializedSize
				result[PNG_PARAM_EXIT_FALSE] = pInstruction->pc + 1; 			// +1 for thumb mode
				result[PNG_PARAM_REGISTER] = reg;

				LOGD("Arrived at POP, done");
				return true;
			} else if (pInstruction->type == ADD_SP_IMMEDIATE) {
				status++;
				LOGD("Arrived at ADD SP, SP, #imm, status --> 6");
			} else {
				LOGD("Illegal instruction after MOV R0, #1");
				return false;
			}
			break;
		case 6:
			if (pInstruction->type == STACK_POP) {

				result[PNG_PARAM_HOOK_POINT] = call_serializedSize->pc;
				result[PNG_PARAM_EXIT_TRUE] = call_serializedSize->pc + 4 + 1; 	// +1 for thumb mode, the next instruction of BLX erializedSize
				result[PNG_PARAM_EXIT_FALSE] = pInstruction->pc - 2 + 1; 		// +1 for thumb mode, set false exit to ADD SP, SP, #n
				result[PNG_PARAM_REGISTER] = reg;

				LOGD("Arrived at POP, done");
				return true;
			} else {
				LOGD("Illegal instruction after ADD SP, SP");
				return false;
			}
			break;
		}
	}

	return false;
}

bool SerializedSizeAnalyser::runFunctionAnalyser(unsigned int *result)
{
	LOGD("Running serializedSize Function Analyser...");

	if (!cpu) return false;

	runInstructionAnalyser();

	unsigned int offset[3];
	unsigned int offsetCount = 0;
	for (std::vector<Instruction *>::iterator
			it = instructions.begin(); it != instructions.end(); ++it) {

		Instruction *pInstruction = *it;

		if (pInstruction->type == LDRSB_IMMEDIATE) {
			offset[offsetCount++] = pInstruction->imm32;
			LOGD("Offset %d --> %d", offsetCount, pInstruction->imm32);
			if (offsetCount == 3) {

				/*
				 enum {
					PNG_PARAM_HOOK_POINT = 0,	// Filled in PngFunctionAnalyser
					PNG_PARAM_EXIT_TRUE,
					PNG_PARAM_EXIT_FALSE,
					PNG_PARAM_REGISTER,

				};
				 */

				if ((offset[0] > 3 || offset[0] < 1)
						|| (offset[1] > 3 || offset[1] < 1)
						|| (offset[2] > 3 || offset[2] < 1)
						|| (offset[0] + offset[1] + offset[2] != 6)
						|| (offset[0] * offset[1] * offset[2] != 6)) {
					return false;
				}

				return true;
			}
		} else {
			LOGD("Illegal instruction, give up");
			return false;
		}
	}

	return false;
}
