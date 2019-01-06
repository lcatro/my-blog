
#include "Instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include "Common.h"

typedef Instruction *(*fnAnalyse)(CPUStatus *cpu);

Instruction *ADD32Analyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC;
	unsigned int ops = cpu->opCode;
	Instruction *pInstruction = NULL;


	// Only support add immediate (ARM A5.1.3)
	if (BITS32(ops, 21, 27) == BINARY(0, 0, 1, 0, 1, 0, 0)) {
		unsigned int cond = BITS32(ops, 28, 31);
		bool S = BITS32(ops, 20, 20);

		int Rn = BITS32(ops, 16, 19);
		int Rd = BITS32(ops, 12, 15);
		int rotate = 2 * BITS32(ops, 8, 11);
		int operand = BITS32(ops, 0, 7);
		operand = ROTATE_RIGHT(operand, rotate);

		// Current don't support conditional add
		if (cond == BINARY(1, 1, 1, 0)) {

			if (Rn == REG_PC) {

				pInstruction = new Instruction(ADD32, pc, 4,
										ADD32_ASM_STR);

				pInstruction->imm32 = pc + 8 + operand;		// Arm always +8 when using pc as operand
				pInstruction->Rd = Rd;
				pInstruction->Rn = Rn;

				cpu->registers[Rd] = pInstruction->imm32;
				cpu->registerStatus[Rd] = REGISTER_IMMEDIATE;

				LOGD_INSTRUCTION("%s, Rd= %d, Rn = %d, operand = %d, result = %d",
					pInstruction->ASMStr.c_str(), Rd, Rn, operand, pInstruction->imm32);

			} else if (cpu->registerStatus[Rn] == REGISTER_IMMEDIATE) {

				pInstruction = new Instruction(ADD32, pc, 4,
										ADD32_ASM_STR);

				pInstruction->imm32 = cpu->registers[Rn] + operand;		// Arm always +8 when using pc as operand
				pInstruction->Rd = Rd;
				pInstruction->Rn = Rn;

				cpu->registers[Rd] = pInstruction->imm32;
				cpu->registerStatus[Rd] = REGISTER_IMMEDIATE;

				LOGD_INSTRUCTION("%s, Rd= %d, Rn = %d, operand = %d, result = %d",
					pInstruction->ASMStr.c_str(), Rd, Rn, operand, pInstruction->imm32);
			}
		}
	}

	return pInstruction;
}

Instruction *LDR32Analyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC;
	unsigned int ops = cpu->opCode;
	Instruction *pInstruction = NULL;

	// Currently, only support LDR PC, [R12, #offset] !!!
	if (BITS32(ops, 12, 31) == 0xE5BCF) {

		if (cpu->registerStatus[12] == REGISTER_IMMEDIATE) {

			pInstruction = new Instruction(LDR32, pc, 4,
									LDR32_ASM_STR);

			unsigned int offset12 = BITS32(ops, 0, 11);
			pInstruction->Rd = 15;
			pInstruction->Rn = 12;
			pInstruction->label = cpu->registers[12] + offset12;

			LOGD_INSTRUCTION("%s, Rd= %d, Rn = %d, offset = 0x%08x, label = 0x%08x",
				pInstruction->ASMStr.c_str(), 15, 12, offset12, pInstruction->label);
		}
	}

	return pInstruction;
}

Instruction *CBNZAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;
	Instruction *pInstruction = NULL;

	if (BITS16(op1, 10, 15) == BINARY(1, 0, 1, 1, 1, 0)
			&& BITS16(op1, 8, 8) == 1) {

		int Rn = BITS16(op1, 0, 2);
		int imm5 = BITS16(op1, 3, 7);
		int i = BITS16(op1, 9, 9);

		int imm32 = 0;
		imm32 = BITSSET(imm32, 1, 5, imm5);
		imm32 = BITSSET(imm32, 6, 6, i);

		pInstruction = new Instruction(CBNZ, pc, 2,
								CBNZ_ASM_STR);
		pInstruction->Rn = Rn;
		pInstruction->label = pc + 4 + imm32;

		LOGD_INSTRUCTION("%s, Rn = %d, Imm32 = %d, label = 0x%08x",
			pInstruction->ASMStr.c_str(), Rn, imm32, pInstruction->label);
	}

	return pInstruction;
}


Instruction *SUBImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;
	int imm32 = 0;
	int d = 0;
	int n = 0;
	if (BITS16(op1, 9, 15) == BINARY(0, 0, 0, 1, 1, 1, 1)) {
		int Rd = BITS16(op1, 0, 2);
		int Rn = BITS16(op1, 3, 5);
		int imm3 = BITS16(op1, 6, 8);

		d = Rd;
		n = Rn;
		imm32 = imm3;
		pInstruction = new Instruction(SUB_IMMEDIATE, pc, 2,
								SUB_IMMEDIATE_ASM_STR1);

	} else if (BITS16(op1, 11, 15) == BINARY(0, 0, 1, 1, 1)) {
		int imm8 = BITS16(op1, 0, 7);
		int Rdn = BITS16(op1, 8, 10);

		d = Rdn;
		n = Rdn;
		imm32 = imm8;
		pInstruction = new Instruction(SUB_IMMEDIATE, pc, 2,
								SUB_IMMEDIATE_ASM_STR2);
	}

	if (pInstruction != NULL) {
		pInstruction->Rd = d;
		pInstruction->Rn = n;
		pInstruction->imm32 = imm32;
		LOGD_INSTRUCTION("%s, Rd= %d, Rn = %d, Imm32 = %d",
			pInstruction->ASMStr.c_str(), d, n, imm32);
	}
	return pInstruction;
}

Instruction *STRImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 HM1S UrlRequest::RestartWithJob, the first MOV
	// Test Case : T2 Not yet
	// Test Case : T3 Not yet

	Instruction *pInstruction = NULL;

	int t = 0;
	int n = 13;
	int imm32 = 0;
	bool index = true;
	bool add = true;
	if (BITS16(op1, 11, 15) == BINARY(0, 1, 1, 0, 0)) {
		int Rt = BITS16(op1, 0, 2);
		int Rn = BITS16(op1, 3, 5);
		int imm5 = BITS16(op1, 6, 10);

		t = Rt;
		n = Rn;
		imm32 = imm5 * 4;

		pInstruction = new Instruction(STR_IMMEDIATE, pc, 2,
								STR_IMMEDIATE_ASM_STR1);
	} else if (BITS16(op1, 11, 15) == BINARY(1, 0, 0, 1, 0)) {
		int Rt = BITS16(op1, 8, 10);
		int imm8 = BITS16(op1, 0, 7);

		t = Rt;
		imm32 = imm8 * 4;

		pInstruction = new Instruction(STR_IMMEDIATE, pc, 2,
								STR_IMMEDIATE_ASM_STR2);
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0)) {
		int Rt = BITS16(op2, 12, 15);
		int Rn = BITS16(op1, 0, 3);
		int imm12 = BITS16(op2, 0, 11);

		t = Rt;
		n = Rn;
		imm32 = imm12;

		pInstruction = new Instruction(STR_IMMEDIATE, pc, 4,
								STR_IMMEDIATE_ASM_STR2);
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0)
		&& BITS16(op2, 11, 11) == BINARY(1)) {
		int Rt = BITS16(op2, 12, 15);
		int Rn = BITS16(op1, 0, 3);
		int imm8 = BITS16(op2, 0, 7);

		index = BITS16(op2, 10, 10);
		add = BITS16(op2, 9, 9);
		imm32 = imm8;
		n = Rn;
		t= Rt;

		pInstruction = new Instruction(STR_IMMEDIATE, pc, 4,
								STR_IMMEDIATE_ASM_STR3);
	}

	if (pInstruction != NULL) {
		pInstruction->Rn = n;
		pInstruction->Rt = t;
		pInstruction->imm32 = imm32;
		pInstruction->add = add;
		LOGD_INSTRUCTION("%s, Rt = %d, Rn = %d, Imm32 = %d",
			pInstruction->ASMStr.c_str(), t, n, imm32);
	}

	return pInstruction;
}

Instruction *MOVRegisterAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 HM1S UrlRequest::RestartWithJob, the first MOV
	// Test Case : T2 Not yet
	// Test Case : T3 Not yet

	Instruction *pInstruction = NULL;
	int d = 0;
	int m = 0;
	bool setFlags = false;
	if (BITS16(op1, 8, 15) == BINARY(0, 1, 0, 0, 0, 1, 1, 0)) {
		int Rd = BITS16(op1, 0, 2);
		int Rm = BITS16(op1, 3, 6);
		int D = BITS16(op1, 7, 7);

		d = Rd;
		d = BITSET(d, 3, D);
		m = Rm;

		pInstruction = new Instruction(MOV_REGISTER, pc, 2,
								MOV_REGISTER_ASM_STR);
	} else if (BITS16(op1, 6, 15) == BINARY(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) {
		int Rd = BITS16(op1, 0, 2);
		int Rm = BITS16(op1, 3, 5);

		d = Rd;
		m = Rm;
		setFlags = true;

		pInstruction = new Instruction(MOV_REGISTER, pc, 2,
								MOV_REGISTER_ASM_STR);
	} else if (false) {	// MOV{S} <c>.W <Rd>, <Rm>

	}

	if (pInstruction != NULL) {
		pInstruction->Rm = m;
		pInstruction->Rd = d;

		if (cpu->registerStatus[m] == REGISTER_IMMEDIATE) {
			cpu->registers[d] = cpu->registers[m];
			cpu->registerStatus[d] = REGISTER_IMMEDIATE;
		} else {
			cpu->registers[d] = m;
			cpu->registerStatus[d] = REGISTER_FROMREGISTER;
		}

		LOGD_INSTRUCTION("%s, Rd = %d, Rm = %d",
			pInstruction->ASMStr.c_str(), d, m);
	}


	return pInstruction;
}

Instruction *MOVImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;

	UINT32 d = 0;
	int imm32 = 0;
	if (BITS16(op1,11, 15) == BINARY(0, 0, 1, 0, 0)) {
		d = BITS16(op1, 8, 10);
		imm32 = BITS16(op1, 0, 7);

		pInstruction = new Instruction(MOV_IMMEDIATE, pc, 2,
								MOV_IMMEDIATE_ASM_STR);
	} else {
		// T2, T3
	}

	if (pInstruction) {
		pInstruction->Rd = d;
		pInstruction->imm32 = imm32;
		LOGD_INSTRUCTION("%s, Rd = %d, imm32 = %d",
			pInstruction->ASMStr.c_str(), d, imm32);
	}

	return pInstruction;
}

Instruction *ADDSPRegisterAnanlyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;

	if (BITS16(op1, 7, 15) == BINARY(1, 0, 1, 1, 0, 0, 0, 0, 0)) {

		pInstruction = new Instruction(ADD_SP_IMMEDIATE, pc, 2,
								ADD_SP_IMMEDIATE_ASM_STR);
		LOGD_INSTRUCTION("%s", pInstruction->ASMStr.c_str());
	}

	return pInstruction;
}

Instruction *ADDRegisterAnanlyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 Not yet
	// Test Case : T2 HM1S UrlRequest::RestartWithJob, the first ADD
	// Test Case : T3 Not yet
	// Test Case : T4 Not yet

	Instruction *pInstruction = NULL;

	if (BITS16(op1, 9, 15) == BINARY(0, 0, 0, 1, 1, 0, 0)) {

		pInstruction = new Instruction(ADD_REGISTER, pc, 2,
								ADD_REGISTER_ASM_STR1);
		LOGD("%s", pInstruction->ASMStr.c_str());

	} else if (BITS16(op1, 8, 15) == BINARY(0, 1, 0, 0, 0, 1, 0, 0)) {

		pInstruction = new Instruction(ADD_REGISTER, pc, 2,
								ADD_REGISTER_ASM_STR2);
		int Rdn = BITS16(op1, 0, 2);
		int Rm = BITS16(op1, 3, 6);
		int DN = BITS16(op1, 7, 7);

		int d = Rdn;
		d = BITSET(d, 3, DN);
		int n = d;
		int m = Rm;

		if ((m == 15)
			&& (cpu->registerStatus[n] == REGISTER_IMMEDIATE)) {
			// Only this case we can calculate it statically

			cpu->registers[n] = (pc + 4) + cpu->registers[n];
			pInstruction->resultAvailable = true;
			pInstruction->result = cpu->registers[n];
		}

		pInstruction->Rm = Rm;
		pInstruction->Rdn = d;
		pInstruction->Rn = d;
		pInstruction->Rd = d;

		LOGD_INSTRUCTION("%s, Rd = %d, Rn = %d, Rm = %d, value = %s 0x%08x",
			pInstruction->ASMStr.c_str(),
			pInstruction->Rd, pInstruction->Rn, pInstruction->Rm,
			cpu->registerStatus[n] == REGISTER_IMMEDIATE ? "" : "Unknown",
			cpu->registerStatus[n] == REGISTER_IMMEDIATE ? cpu->registers[n] : 0);


	} else if (BITS16(op1, 5, 15) == BINARY(1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0)) {
		pInstruction = new Instruction(ADD_REGISTER, pc, 4,
								ADD_REGISTER_ASM_STR3);
		LOGD_INSTRUCTION("%s",
			pInstruction->ASMStr.c_str());

	}

	return pInstruction;
}

Instruction *LDRSBImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;

	int t = 0;
	int n = 0;
	int imm32 = 0;
	//								  15 14 13 12 11 10 9  8  7  6  5  4
	if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1)) {
		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);
		imm32 = BITS16(op2, 0, 11);

		if (t != 15 && n != 15) {
			pInstruction = new Instruction(LDRSB_IMMEDIATE, pc, 4,
									LDRSB_IMMEDIATE_ASM_STR);
		}
	//								  		 15 14 13 12 11 10 9  8  7  6  5  4
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1)
				&& BITS16(op2, 11, 11) == BINARY(1)) {
		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);
		imm32 = BITS16(op2, 0, 7);

		if (t != 15 && n != 15) {
			pInstruction = new Instruction(LDRSB_IMMEDIATE, pc, 4,
									LDRSB_IMMEDIATE_ASM_STR);
		}
	}

	if (pInstruction) {
		LOGD_INSTRUCTION("%s, Rt = %d, Rn = %d, imm = %d",
				pInstruction->ASMStr.c_str(), t, n, imm32);

		pInstruction->Rn = n;
		pInstruction->Rt = t;
		pInstruction->imm32 = imm32;
	}

	return pInstruction;
}

Instruction *LDRBImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;

	int t = 0;
	int n = 0;
	int imm32 = 0;
	bool index = true;
	bool add = true;
	bool wback = false;
									//15 14 13 12 11
	if (BITS16(op1, 11, 15) == BINARY(0, 1, 1, 1, 1)) {
		t = BITS16(op1, 0, 2);
		n = BITS16(op1, 3, 5);
		imm32 = BITS16(op1, 6, 10);

		pInstruction = new Instruction(LDRB_IMMEDIATE, pc, 2,
								LDRB_IMMEDIATE_ASM_STR1);

										//  15 14 13 12 11 10 9  8  7  6  5  4
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1)
		&& BITS16(op1, 0, 3) != BINARY(1, 1, 1, 1)			// Rn = 15 is LDR Literal
		&& BITS16(op2, 12, 15) != BINARY(1, 1, 1, 1)) {		// Rt = 15 is PLD Immediate

		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);
		int imm12 = BITS16(op2, 0, 11);
		imm32 = imm12;

		pInstruction = new Instruction(LDRB_IMMEDIATE, pc, 4,
								LDRB_IMMEDIATE_ASM_STR2);
										//  15 14 13 12 11 10 9  8  7  6  5  4
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1)
		&& BITS16(op2, 11, 11) == BINARY(1)
		&& BITS16(op1, 0, 3) != BINARY(1, 1, 1, 1)) {

		LOGD_INSTRUCTION("Error, LDRB Immediate T3 not implemented yet~");
	}

	if (pInstruction) {
		LOGD_INSTRUCTION("%s, Rt = %d, Rn = %d, imm = %d, value = UNKNOWN",
				pInstruction->ASMStr.c_str(), t, n, imm32);

		pInstruction->Rn = n;
		pInstruction->Rt = t;
		pInstruction->imm32 = imm32;
	}

	return pInstruction;
}


Instruction *LDRImmediateAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 Not yet
	// Test Case : T2 Not yet
	// Test Case : T3 Not implement
	// Test Case : T4 Not implement

	Instruction *pInstruction = NULL;

	int t = 0;
	int n = 0;
	int imm32 = 0;
	bool index = true;
	bool add = true;
	bool wback = false;
	if (BITS16(op1, 11, 15) == BINARY(0, 1, 1, 0, 1)) {
		t = BITS16(op1, 0, 2);
		n = BITS16(op1, 3, 5);
		imm32 = BITS16(op1, 6, 10) * 4;

		pInstruction = new Instruction(LDR_IMMEDIATE, pc, 2,
								LDR_IMMEDIATE_ASM_STR1);

	} else if (BITS16(op1, 11, 15) == BINARY(1, 0, 0, 1, 1)) {
		t = BITS16(op1, 8, 10);
		n = 13;
		imm32 = BITS16(op1, 0, 7) * 4;

		pInstruction = new Instruction(LDR_IMMEDIATE, pc, 2,
								LDR_IMMEDIATE_ASM_STR2);

	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1)
		&& BITS16(op1, 0, 3) != BINARY(1, 1, 1, 1)) {	// Rn = 15 is LDR Literal

		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);
		int imm12 = BITS16(op2, 0, 11);
		imm32 = imm12;

		pInstruction = new Instruction(LDR_IMMEDIATE, pc, 4,
								LDR_IMMEDIATE_ASM_STR3);

	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1)
		&& BITS16(op2, 11, 11) == BINARY(1)
		&& BITS16(op1, 0, 3) != BINARY(1, 1, 1, 1)) {
		/*
		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);
		int imm8 = BITS16(op2, 0, 7);
		imm32 = imm8;

		pInstruction = new Instruction(LDR_IMMEDIATE, pc, 4,
						"LDR<c>.W <Rt>, [<Rn>, #+/-<imm8>]");
		*/
		LOGD_INSTRUCTION("Error, LDR Immediate T4 not implemented yet~");
	}

	if (pInstruction) {
		if (false) {	// Don't calculate the value
				// We don't need this value
				// And more importantly, the address maybe unavailable yet!
		//if (cpu->registerStatus[n] == REGISTER_IMMEDIATE) {
			unsigned int offset_addr = add ? (cpu->registers[n] + imm32) :
							    (cpu->registers[n] - imm32);
			unsigned int address = index ? offset_addr : (cpu->registers[n]);

			if (t == 15) { // TODO

			} else {
				cpu->registers[t] = MEMU4(address);
				cpu->registerStatus[t] = REGISTER_IMMEDIATE;
			}

			LOGD_INSTRUCTION("%s, Rt = %d, Rn = %d, imm = %d, value = 0x%08x",
				pInstruction->ASMStr.c_str(), t, n, imm32, cpu->registerStatus[t]);
		} else {
			LOGD_INSTRUCTION("%s, Rt = %d, Rn = %d, imm = %d, value = UNKNOWN",
				pInstruction->ASMStr.c_str(), t, n, imm32);
		}

		pInstruction->Rn = n;
		pInstruction->Rt = t;
		pInstruction->imm32 = imm32;
	}

	return pInstruction;
}

Instruction *LDRLiteralAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 HM1S URLRequest::RestartWithJob, the first two LDRs
	// Test Case : T2 Not yet

	Instruction *pInstruction = NULL;
	bool add = true;
	int imm32 = 0;
	int t = 0;
	if (BITS16(op1, 11, 15) == BINARY(0, 1, 0, 0, 1)) {
		int imm8 = BITS16(op1, 0, 7);

		t = BITS16(op1, 8, 10);
		imm32 = BITSSET(imm32, 0, 1, 0);
		imm32 = BITSSET(imm32, 2, 9, imm8);

		pInstruction = new Instruction(LDR_LITERAL, pc, 2,
								LDR_LITERAL_ASM_STR1);
	} else if (BITS16(op1, 8, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0)
		&& BITS16(op1, 0, 6) == BINARY(1, 0, 1, 1, 1, 1, 1)) {

		int imm12 = BITS16(op2, 0, 11);
		add = (bool)BITS16(op1, 7, 7);
		t = BITS16(op2, 12, 15);

		imm32 = imm12;

		pInstruction = new Instruction(LDR_LITERAL, pc, 4,
								LDR_LITERAL_ASM_STR2);
	}

	if (pInstruction != NULL) {
		unsigned int base = ALIGN4(pc + 4);	// Thumb mode, always add 4 when using PC for calculation
		if (add) 	base += imm32; else base -= imm32;
		// Don't calculate it directly
		// We need AddressBoundary to make sure the address is reachable

		//cpu->registers[t] = MEMU4(base);
		//cpu->registerStatus[t] = REGISTER_IMMEDIATE;
		pInstruction->imm32 = base;
		pInstruction->Rt = t;
		//LOGD("%s, imm32 = 0x%08x, Rt = R%d, value = 0x%08x",
		//	pInstruction->ASMStr.c_str(), imm32, t, cpu->registers[t]);
		LOGD_INSTRUCTION("%s, imm32 = 0x%08x, Rt = R%d, address = 0x%08x",
			pInstruction->ASMStr.c_str(), imm32, t, pInstruction->imm32);
	}

	return pInstruction;
}


Instruction *LDRRegisterAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	Instruction *pInstruction = NULL;
	UINT32 m = 0;
	UINT32 n = 0;
	UINT32 t = 0;
	if (BITS16(op1, 9, 15) == BINARY(0, 1, 0, 1, 1, 0, 0)) {
		m = BITS16(op1, 6, 8);
		n = BITS16(op1, 3, 5);
		t = BITS16(op1, 0, 2);

		pInstruction = new Instruction(LDR_REGISTER, pc, 2,
								LDR_REGISTER_ASM_STR1);
	} else if (BITS16(op1, 4, 15) == BINARY(1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1)
		&& BITS16(op2, 6, 11) == BINARY(0, 0, 0, 0, 0, 0)) {

		m = BITS16(op2, 0, 3);
		t = BITS16(op2, 12, 15);
		n = BITS16(op1, 0, 3);

		pInstruction = new Instruction(LDR_REGISTER, pc, 4,
								LDR_REGISTER_ASM_STR2);
	}

	if (pInstruction != NULL) {
		pInstruction->Rm = m;
		pInstruction->Rn = n;
		pInstruction->Rt = t;

		LOGD_INSTRUCTION("%s, Rm = %d, Rn = %d, Rt = %d",
			pInstruction->ASMStr.c_str(), m, n, t);
	}

	return pInstruction;
}

Instruction *BXAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : HM1S URLRequest::hashmapIntHash

	Instruction *pInstruction = NULL;
	int m = 0;
	if (BITS16(op1, 7, 15) == BINARY(0, 1, 0, 0, 0, 1, 1, 1, 0)) {
		int Rm = BITS16(op1, 3, 6);

		m = Rm;
		pInstruction = new Instruction(BRANCH_WITHOUT_LINK_REGISTER, pc, 2,
							BRANCH_WITHOUT_LINK_REGISTER_ASM_STR);
	}

	if (pInstruction != NULL) {
		pInstruction->Rm = m;
		LOGD_INSTRUCTION("%s, Rm = %d",
			pInstruction->ASMStr.c_str(), m);
	}

	return pInstruction;
}

Instruction *BAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 - Not yet
	// Test Case : T2 -

	Instruction *pInstruction = NULL;
	int imm32 = 0;
	if (BITS16(op1, 12, 15) == BINARY(1, 1, 0, 1)) {
		bool S = (bool)BITS16(op1, 7, 7);
		int imm8 = BITS16(op1, 0, 7);

		imm32 = BITSET(imm32, 0, 0);
		imm32 = BITSSET(imm32, 1, 8, imm8);
		imm32 = SIGNEXTEND(S, imm32, 9);
		pInstruction = new Instruction(BRANCH_WITHOUT_LINK, pc, 2,
						BRANCH_WITHOUT_LINK_ASM_STR);
	} else if (BITS16(op1, 11, 15) == BINARY(1, 1, 1, 0, 0)) {

		bool S = (bool)BITS16(op1, 10, 10);
		int imm11 = BITS16(op1, 0, 10);

		imm32 = BITSET(imm32, 0, 0);
		imm32 = BITSSET(imm32, 1, 11, imm11);
		imm32 = SIGNEXTEND(S, imm32, 12);
		pInstruction = new Instruction(BRANCH_WITHOUT_LINK, pc, 2,
						BRANCH_WITHOUT_LINK_ASM_STR);
	} else if (BITS16(op1, 11, 15) == BINARY(1, 1, 1, 1, 0)
		&& BITS16(op2, 14, 15) == BINARY(1, 0)
		&& BITS16(op2, 12, 12) == BINARY(0)) {

		bool S = (bool)BITS16(op1, 10, 10);
		int imm6 = BITS16(op1, 0, 5);
		int imm11 = BITS16(op2, 0, 10);
		int J2 = BITS16(op2, 11, 11);
		int J1 = BITS16(op2, 13, 13);

		imm32 = BITSET(imm32, 0, 0);
		imm32 = BITSSET(imm32, 1, 11, imm11);
		imm32 = BITSSET(imm32, 12, 17, imm6);
		imm32 = BITSET(imm32, 18, J1);
		imm32 = BITSET(imm32, 19, J2);
		imm32 = SIGNEXTEND(S, imm32, 20);

		pInstruction = new Instruction(BRANCH_WITHOUT_LINK, pc, 4,
						BRANCH_WITHOUT_LINK_ASM_STR);
	} else if (BITS16(op1, 11, 15) == BINARY(1, 1, 1, 1, 0)
		&& BITS16(op2, 14, 15) == BINARY(1, 0)
		&& BITS16(op2, 12, 12) == BINARY(1)) {

		bool S = (bool)BITS16(op1, 10, 10);
		int imm6 = BITS16(op1, 0, 5);
		int imm11 = BITS16(op2, 0, 10);
		bool J2 = (bool)BITS16(op2, 11, 11);
		bool J1 = (bool)BITS16(op2, 13, 13);
		bool I1 = !(J1 ^ S);
		bool I2 = !(J2 ^ S);

		imm32 = BITSET(imm32, 0, 0);
		imm32 = BITSSET(imm32, 1, 11, imm11);
		imm32 = BITSSET(imm32, 12, 17, imm6);
		imm32 = BITSET(imm32, 18, I2);
		imm32 = BITSET(imm32, 19, I1);
		imm32 = SIGNEXTEND(S, imm32, 20);

		pInstruction = new Instruction(BRANCH_WITHOUT_LINK, pc, 4,
						BRANCH_WITHOUT_LINK_ASM_STR);
	}

	if (pInstruction) {
		pInstruction->label = pc + 4 + imm32;
		LOGD_INSTRUCTION("%s Label = 0x%08x ", pInstruction->ASMStr.c_str(), pInstruction->label);
	}

	return pInstruction;
}

Instruction *BLAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : Word T1, Jump back, HM1S UrlRequest::Start, the first BL
	// Test Case : Word T1, Jump forward, HM1S UrlRequest::Restart, the second BL
	// Test Case : Word T2 - Not yet
	// Test Case : Half-Word - Not yet

	Instruction *pInstruction = NULL;
	if (BITS16(op1, 11, 15) == BINARY(1, 1, 1, 1, 0)
		&& (BITS16(op2, 14, 15) == BINARY(1, 1))) {

		bool T1 = BITS16(op2, 12, 12) == 1;
		bool toARM = T1 ? false : true;
		bool S = (bool)BITS16(op1, 10, 10);
		bool J1 = (bool)BITS16(op2, 13, 13);
		bool J2 = (bool)BITS16(op2, 11, 11);
		bool I1 = !(J1 ^ S);
		bool I2 = !(J2 ^ S);

		int imm32 = 0;
		if (T1) {
			int imm10 = BITS16(op1, 0, 9);
			int imm11 = BITS16(op2, 0, 10);

			imm32 = BITSET(imm32, 0, 0);
			imm32 = BITSSET(imm32, 1, 11, imm11);
			imm32 = BITSSET(imm32, 12, 21, imm10);

			pc = pc + 4;

		} else {
			int imm10H = BITS16(op1, 0, 9);
			int imm10L  = BITS16(op2, 1, 10);

			imm32 = BITSSET(imm32, 0, 1, 0);
			imm32 = BITSSET(imm32, 2, 11, imm10L);
			imm32 = BITSSET(imm32, 12, 21, imm10H);

			pc = ALIGN4(pc + 4);	// For T2, pc should align to 4
		}

		imm32 = BITSET(imm32, 22, I2);
		imm32 = BITSET(imm32, 23, I1);
		imm32 = SIGNEXTEND(S, imm32, 23);

		pInstruction = new Instruction(BRANCH_WITH_LINK_LABEL, cpu->PC, 4,
								BRANCH_WITH_LINK_LABEL_ASM_STR);

		pInstruction->label = pc + imm32; 	// Remember PC should be the next instruction
													// And the length of the current instruction is 4 !
		pInstruction->toARM = toARM;

		LOGD_INSTRUCTION("%s Label = 0x%08x ", pInstruction->ASMStr.c_str(), pInstruction->label);

	} else if (BITS16(op1, 7, 15) == BINARY(0, 1, 0, 0, 0, 1, 1, 1, 1)) {
		pInstruction = new Instruction(BRANCH_WITH_LINK_REGISTER, cpu->PC, 2,
								BRANCH_WITH_LINK_REGISTER_ASM_STR);
		pInstruction->Rm = BITS16(op1, 3, 6);

		LOGD_INSTRUCTION("%s Rm = 0x%02x ", pInstruction->ASMStr.c_str(), pInstruction->Rm);
	}

	return pInstruction;
}

Instruction *PUSHAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : T1 - Not yet
	// Test Case : T2 HM1S UrlRequest::Start

	Instruction *pInstruction = NULL;
	if (BITS16(op1, 9, 15) == BINARY(1, 0, 1, 1, 0, 1, 0)) {
		unsigned int registers = BITS16(op1, 0, 7);
		pInstruction = new Instruction(STACK_PUSH, pc, 2,
								STACK_PUSH_ASM_STR);
		bool isPushLR = BITS16(op1, 8, 8);
		pInstruction->isPushLR = isPushLR;
		pInstruction->registers = registers;
	} else if(BITS16(op1, 0, 15) == BINARY(1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1)) {
		unsigned int registers = BITS16(op2, 0, 12);
		pInstruction = new Instruction(STACK_PUSH, pc, 4,
								STACK_PUSH_ASM_STR);
		bool isPushLR = BITS32(ops, 14, 14);
		pInstruction->isPushLR = isPushLR;
		pInstruction->registers = registers;
	}

	if (pInstruction)
		LOGD_INSTRUCTION("%s Registers = 0x%02x, isPushLR = %d ",
			pInstruction->ASMStr.c_str(), pInstruction->registers, pInstruction->isPushLR);

	return pInstruction;
}

Instruction *POPAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Test Case : WORD HM1S UrlRequest::Start
	// Test Case : Half-Word - Not yet

	Instruction *pInstruction = NULL;
	if (BITS16(op1, 9, 15) == BINARY(1, 0, 1, 1, 1, 1, 0)) {
		unsigned int registers = BITS16(op1, 0, 7);
		pInstruction =  new Instruction(STACK_POP, pc, 2,
								STACK_POP_ASM_STR);
		bool isPopPC = BITS16(op1, 8, 8);
		pInstruction->isPopPC = isPopPC;
		pInstruction->registers = registers;
	} else if(BITS16(op1, 0, 15) == BINARY(1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1)) {
		unsigned int registers = BITS16(op2, 0, 12);
		pInstruction =  new Instruction(STACK_POP, pc, 4,
								STACK_POP_ASM_STR);
		bool isPopPC = BITS16(op2, 15, 15);
		pInstruction->isPopPC = isPopPC;
		pInstruction->registers = registers;
	}

	if (pInstruction)
		LOGD_INSTRUCTION("%s Registers = 0x%02x isPopPC = %d",
			pInstruction->ASMStr.c_str(), pInstruction->registers, pInstruction->isPopPC);

	return pInstruction;
}

Instruction *UnknownInstructionAnalyser(CPUStatus *cpu)
{
	unsigned int pc = cpu->PC; unsigned short op1 = cpu->opCodeH;
	unsigned short op2 = cpu->opCodeL; unsigned int ops = cpu->opCode;

	// Althougth we don't know what's this
	// We should know the length of the instruction
	// So that we can continue analyse other instructions

	Instruction *pInstruction = NULL;
	if (BITS16(op1, 13, 15) == BINARY(1, 1, 1)
		&& BITS16(op1, 11, 12) != BINARY(0, 0)) {
		pInstruction =  new Instruction(UNKNOWN_INSTRUCTION, pc, 4,
					"UnkownInstruction.W ");
	} else {
		pInstruction =  new Instruction(UNKNOWN_INSTRUCTION, pc, 2,
					"UnkownInstruction");
	}
	return pInstruction;
}

//When an instruction reads the PC, the value read depends on which instruction set it comes from:
//For an ARM instruction, the value read is the address of the instruction plus 8 bytes.
//	Bits [1:0] of this value are always zero, because ARM instructions are always word-aligned.
//For a Thumb instruction, the value read is the address of the instruction plus 4 bytes.
//	Bit [0] of this value is always zero, because Thumb instructions are always halfword-aligned.

static const fnAnalyse kAnalysers[] = {
	PUSHAnalyser,
	POPAnalyser,
	BLAnalyser,
	BAnalyser,
	BXAnalyser,
	LDRLiteralAnalyser,
	LDRRegisterAnalyser,
	LDRImmediateAnalyser,
	LDRBImmediateAnalyser,
	ADDRegisterAnanlyser,
	ADDSPRegisterAnanlyser,
	MOVRegisterAnalyser,
	MOVImmediateAnalyser,
	STRImmediateAnalyser,
	SUBImmediateAnalyser,
	CBNZAnalyser,
	LDRSBImmediateAnalyser,

	// Add your analyser here

	// SHOULD put this at the bottom of this list!!!
	UnknownInstructionAnalyser
};

static const fnAnalyse kAnalysers32[] = {

	ADD32Analyser,
	LDR32Analyser,

	// SHOULD put this at the bottom of this list!!!
	UnknownInstructionAnalyser
};

Instruction *InstructionAnalyser::analyse(CPUStatus *cpu, bool thumb2)
{
	cpu->opCodeH = *(unsigned short *)cpu->PC;
	cpu->opCodeL = *(unsigned short *)(cpu->PC + 2);
	cpu->opCode = *(unsigned int *)cpu->PC;

	LOGD_INSTRUCTION("Analysing PC (0x%08x), opcode(0x%08x)", cpu->PC, cpu->opCode);

	Instruction *pInstruction = NULL;
	if (thumb2) {
		for (unsigned int i = 0; i < arraysize(kAnalysers); i++) {
			pInstruction = kAnalysers[i](cpu);
			if (pInstruction != NULL)
				break;
		}
	} else {
		for (unsigned int i = 0; i < arraysize(kAnalysers32); i++) {
			pInstruction = kAnalysers32[i](cpu);
			if (pInstruction != NULL)
				break;
		}
	}
	return pInstruction;
}

unsigned int InstructionMaker::makeBLX(unsigned int pc, unsigned int dest, bool toArm)
{
	unsigned short opcodeL = 0;
	unsigned short opcodeH = 0;

	if (toArm) {

		pc = ALIGN4(pc + 4);		// T2, PC should align to 4
		int offset = dest - pc;

		if (offset > 16777212 || offset < -16777216) {
			LOGD("Make BLX 0x%08x, 0x%08x, offset %d beyond range!", pc, dest, offset);
			return 0;
		}

		unsigned int imm10L = BITS32(offset, 2, 11);
		unsigned int imm10H = BITS32(offset, 12, 21);
		bool I2 = (BITS32(offset, 22, 22) == 1);
		bool I1 = (BITS32(offset, 23, 23) == 1);
		bool S = (offset < 0);

		bool J1 = (!I1) ? (!S) : S;
		bool J2 = (!I2) ? (!S) : S;

		opcodeL = BITSSET(opcodeL, 11, 15, BINARY(1, 1, 1, 1, 0));
		opcodeL = BITSSET(opcodeL, 10, 10, S ? 1 : 0);
		opcodeL = BITSSET(opcodeL, 0, 9, imm10H);

		opcodeH = BITSSET(opcodeH, 14, 15, BINARY(1, 1));
		opcodeH = BITSSET(opcodeH, 13, 13, J1 ? 1 : 0);
		opcodeH = BITSSET(opcodeH, 12, 12, 0);		// toArm bit
		opcodeH = BITSSET(opcodeH, 11, 11, J2 ? 1 : 0);
		opcodeH = BITSSET(opcodeH, 1, 10, imm10L);

		return (opcodeH << 16) | opcodeL;
	} else {

		pc = pc + 4;		// T1, PC don't have to align to 4
		int offset = dest - pc;

		if (offset > 16777214 || offset < -16777216) {
			LOGD("Make BLX 0x%08x, 0x%08x, offset %d beyond range!", pc, dest, offset);
			return 0;
		}

		unsigned int imm11 = BITS32(offset, 1, 11);
		unsigned int imm10 = BITS32(offset, 12, 21);
		bool I2 = (BITS32(offset, 22, 22) == 1);
		bool I1 = (BITS32(offset, 23, 23) == 1);
		bool S = (offset < 0);

		bool J1 = (!I1) ? (!S) : S;
		bool J2 = (!I2) ? (!S) : S;

		opcodeL = BITSSET(opcodeL, 11, 15, BINARY(1, 1, 1, 1, 0));
		opcodeL = BITSSET(opcodeL, 10, 10, S ? 1 : 0);
		opcodeL = BITSSET(opcodeL, 0, 9, imm10);

		opcodeH = BITSSET(opcodeH, 14, 15, BINARY(1, 1));
		opcodeH = BITSSET(opcodeH, 13, 13, J1 ? 1 : 0);
		opcodeH = BITSSET(opcodeH, 12, 12, 1);		// to thumb bit
		opcodeH = BITSSET(opcodeH, 11, 11, J2 ? 1 : 0);
		opcodeH = BITSSET(opcodeH, 0, 10, imm11);

		return (opcodeH << 16) | opcodeL;

	}

}

short InstructionMaker::makeBxReg(unsigned int reg) {

	short opcode = 0;

	//									   15 14 13 12 11 10 9  8  7
	opcode = BITSSET(opcode, 7, 15, BINARY(0, 1, 0, 0, 0, 1, 1, 1, 0));
	opcode = BITSSET(opcode, 3, 6, reg);

	return opcode;
}

// !!!! dataAddr should have been aligned to 4 !!!
// !!!! Only support data address > addr case !!!
// !!!! data addr - addr < 1024 !!!
unsigned int InstructionMaker::makeLdrReg(unsigned int reg, unsigned int addr, unsigned int dataAddr) {

	unsigned int pc = ALIGN4((addr + 4));
	unsigned int opcode1 = 0;
	unsigned int opcode2 = 0;
	unsigned int opcode = 0;

	unsigned int offset = dataAddr - pc;

	if (offset > 1024) {
		LOGD("Make LDR Reg offset = %d > 1024 !", offset);
		return 0;
	}

	//										15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	opcode1 = BITSSET(opcode1, 0, 15, BINARY(1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1));
	opcode2 = BITSSET(opcode2, 12, 15, reg);
	opcode2 = BITSSET(opcode2, 0, 11, offset);

	opcode = (opcode2 << 16) | opcode1;

	LOGD("0x%08x LDR R%d, [PC, #%d] <--- (0x%08x, 0x%08x)", opcode, reg, offset, addr, dataAddr);

	return opcode;

}
