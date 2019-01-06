#include "FreakFunctionAnalyser.h"

bool FreakFunctionAnalyser::runFunctionAnalyser(unsigned int *result)
{
	if (!cpu) return false;

	Instruction *call_RSA_new = findCallFunction("RSA_new", 1);
	Instruction *call_EC_KEY_get0_group = findCallFunction("EC_KEY_get0_group", 1);
	Instruction *call_EC_GROUP_get_degree = findCallFunction("EC_GROUP_get_degree", 1);

	if (!call_RSA_new || !call_EC_KEY_get0_group || !call_EC_GROUP_get_degree) return false;

	if (!(call_RSA_new->pc < call_EC_KEY_get0_group->pc) || !(call_EC_KEY_get0_group->pc < call_EC_GROUP_get_degree->pc)) {
		LOGD("Wrong calling sequence, give up!");
		return false;
	}

	unsigned int status = 0;
	unsigned int reg1 = 0;
	unsigned int reg2 = 0;
	unsigned int reg3 = 0;
	unsigned int const1 = 0;
	unsigned int const2 = 0;
	unsigned int const3 = 0;
	for (std::vector<Instruction *>::iterator
			it = instructions.begin(); it != instructions.end(); ++it) {

		Instruction *pInstruction = *it;

		switch (status) {
		case 0:
			if (pInstruction->type == MOV_REGISTER
					&& pInstruction->Rm == REG_R0
					&& pInstruction->Rd != REG_R0) {
				reg1 = pInstruction->Rd;
				status++;
				LOGD("reg1 = %d, status --> 1", reg1);
			}
			break;
		case 1:
			if (pInstruction == call_EC_KEY_get0_group) {
				LOGD("BLX EC_KEY_get0_group, status --> 2");
				status++;
			}
			break;
		case 2:
			if (pInstruction == call_EC_GROUP_get_degree) {
				LOGD("BLX EC_GROUP_get_degree at status 2, give up!");
				return false;
			}
			if ((pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE)
					&& pInstruction->Rn == reg1) {
				reg2 = pInstruction->Rt;
				const1 = pInstruction->imm32;
				LOGD("reg2 = %d, const1 = %d, status --> 3", reg2, const1);
				status++;
			}
			break;
		case 3:
			if (pInstruction == call_EC_GROUP_get_degree) {
				LOGD("BLX EC_GROUP_get_degree at status 3, give up!");
				return false;
			}
			if ((pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE)
					&& pInstruction->Rn == reg2) {
				reg3 = pInstruction->Rt;
				const2 = pInstruction->imm32;
				LOGD("reg3 = %d, const2 = %d, status --> 4", reg2, const2);
				status++;
			}
			break;
		case 4:
			if (pInstruction == call_EC_GROUP_get_degree) {
				LOGD("BLX EC_GROUP_get_degree at status 4, give up!");
				return false;
			}
			if ((pInstruction->type == LDR_IMMEDIATE
					|| pInstruction->type == LDRB_IMMEDIATE)
					&& pInstruction->Rn == reg3) {
				const3 = pInstruction->imm32;
				LOGD("const3 = %d, very good~", const3);

				/*	See definition in common.h
				enum {
					PARAM_HOOK_POINT = 0,
					PARAM_RSA_NEW_ADDRESS,
					PARAM_EXIT_TRUE,
					PARAM_EXIT_FALSE,
					PARAM_SSL_REGISTER,
					PARAM_SSL_OFFSET1,
					PARAM_SSL_OFFSET2,
					PARAM_SSL_OFFSET3,
					PARAM_MAX
				};
				*/
				result[FREAK_PARAM_HOOK_POINT] = call_RSA_new->pc;
				result[FREAK_PARAM_EXIT_TRUE] = (call_RSA_new->pc + 4) + 1;	// +1 for thumb mode, the next instruction of BLX RSA_new
				result[FREAK_PARAM_EXIT_FALSE] = (call_RSA_new->pc + 4) + 1;	// +1 for thumb mode
				result[FREAK_PARAM_SSL_REGISTER] = reg1;
				result[FREAK_PARAM_SSL_OFFSET1] = const1;
				result[FREAK_PARAM_SSL_OFFSET2] = const2;
				result[FREAK_PARAM_SSL_OFFSET3] = const3;
				return true;
			}
			break;
		}
	}

	return false;

}
