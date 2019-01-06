#ifndef _HOTPATCH_INSTRUCTION_H_
#define _HOTPATCH_INSTRUCTION_H_
#pragma once

#include "Common.h"
#include "CPUStatus.h"
#include <string>

#define BITS16(data, start, end) (((data & 0xFFFF) & (0xFFFF << start) & (0xFFFF >>  (15 - end))) >> start)
#define BITS32(data, start, end) (((data & 0xFFFFFFFF) & (0xFFFFFFFF << start) & (0xFFFFFFFF >>  (31 - end))) >> start)

// Should clear the specified bit first
#define BITSET(data, position, value) ((data & ~(1 << position)) | ((value) << position))
#define BITSSET(data, start, end, value) ((data & ~(BITSMASK(start, end))) | (((value) << start) & (BITSMASK(start, end))))
#define BITSMASK(start, end) ((0xFFFFFFFF << start) & (0xFFFFFFFF >>  (31 - end)))
#define SIGNEXTEND(sign, data, length) (BITSSET(data, length, 31, sign ? 0xFFFFFFFF : 0))
#define ALIGN4(data) ((data) & 0xFFFFFFFC)
#define MEMU4(address) *(unsigned int *)address
#define PATCH_DWORD(address, value) ((*(unsigned int *)(address)) = ((unsigned int)value))
#define ROTATE_LEFT(num, bits) (((num) << (bits)) | ((num) >> (32 - (bits))))
#define ROTATE_RIGHT(num, bits) (((num) >> (bits)) | ((num) << (32 - (bits))))

#define BINARY_1(_FIRST) (_FIRST)
#define BINARY_2(_FIRST, ...) ((_FIRST << 1) + BINARY_1(__VA_ARGS__))
#define BINARY_3(_FIRST, ...) ((_FIRST << 2)+ BINARY_2(__VA_ARGS__))
#define BINARY_4(_FIRST, ...) ((_FIRST << 3) + BINARY_3(__VA_ARGS__))
#define BINARY_5(_FIRST, ...) ((_FIRST << 4) + BINARY_4(__VA_ARGS__))
#define BINARY_6(_FIRST, ...) ((_FIRST << 5) + BINARY_5(__VA_ARGS__))
#define BINARY_7(_FIRST, ...) ((_FIRST << 6) + BINARY_6(__VA_ARGS__))
#define BINARY_8(_FIRST, ...) ((_FIRST << 7) + BINARY_7(__VA_ARGS__))
#define BINARY_9(_FIRST, ...) ((_FIRST << 8) + BINARY_8(__VA_ARGS__))
#define BINARY_10(_FIRST, ...) ((_FIRST << 9) + BINARY_9(__VA_ARGS__))
#define BINARY_11(_FIRST, ...) ((_FIRST << 10) + BINARY_10(__VA_ARGS__))
#define BINARY_12(_FIRST, ...) ((_FIRST << 11) + BINARY_11(__VA_ARGS__))
#define BINARY_13(_FIRST, ...) ((_FIRST << 12) + BINARY_12(__VA_ARGS__))
#define BINARY_14(_FIRST, ...) ((_FIRST << 13) + BINARY_13(__VA_ARGS__))
#define BINARY_15(_FIRST, ...) ((_FIRST << 14) + BINARY_14(__VA_ARGS__))
#define BINARY_16(_FIRST, ...) ((_FIRST << 15) + BINARY_15(__VA_ARGS__))

#define BINARY(...) BINARY_N(PP_NARG(__VA_ARGS__), __VA_ARGS__)
#define BINARY_N(...) BINARY_N_(__VA_ARGS__)
#define BINARY_N_(N, ...) BINARY_##N(__VA_ARGS__)

#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11,_12,_13,_14,_15,_16, N, ...) N
#define PP_RSEQ_N() 16,15,14,13,12,11,10, 9,8,7,6,5,4,3,2,1,0

#define REG_PC 15
#define REG_R0 0
#define REG_R1 1
#define REG_R2 2
#define REG_R3 3
#define REG_R4 4
#define REG_R5 5
#define REG_R6 6
#define REG_R7 7
#define REG_R8 8
#define REG_R9 9
#define REG_R10 10
#define REG_R11 11
#define REG_R12 12

#ifdef ENABLE_DEBUG_LOG
#define BRANCH_WITHOUT_LINK_ASM_STR				"B<c> <label>"
#define BRANCH_WITHOUT_LINK_REGISTER_ASM_STR	"BX<c> <Rm>"
#define BRANCH_WITH_LINK_ASM_STR				""
#define BRANCH_WITH_LINK_REGISTER_ASM_STR		"BLX<c> <Rm>"
#define BRANCH_WITH_LINK_LABEL_ASM_STR			"BLX<c> <label>"
#define STACK_PUSH_ASM_STR						"PUSH<c> <registers> "
#define STACK_POP_ASM_STR			"POP<c> <registers> "
#define LDR_LITERAL_ASM_STR1		"LDR<c> <Rt> <label> or LDR<c> <Rt> [PC, #+/-<imm>]"
#define LDR_LITERAL_ASM_STR2		"LDR<c>.W <Rt> <label> or LDR<c>.W <Rt> [PC, #+/-<imm>]"
#define LDR_IMMEDIATE_ASM_STR1		"LDR<c> <Rt>, [<Rn>, #<imm>]"
#define LDR_IMMEDIATE_ASM_STR2		"LDR<c> <Rt>, [SP, #<imm>]"
#define LDR_IMMEDIATE_ASM_STR3		"LDR<c>.W <Rt>, [<Rn>, #<imm12>]"
#define LDR_REGISTER_ASM_STR1		"LDR<c> <Rt>, [<Rn>, <Rm>]"
#define LDR_REGISTER_ASM_STR2		"LDR<c>.W <Rt>, [<Rn>, <Rm>, LSL #<shift>]"
#define ADD_REGISTER_ASM_STR1		"ADDS <Rd>, <Rn>, <Rm>"
#define ADD_REGISTER_ASM_STR2		"ADD<c> <Rdn>, <Rm>"
#define ADD_REGISTER_ASM_STR3		"ADD{S}<c>.W <Rd>, <Rn>, <Rm>{, <shift>}"
#define ADD_SP_IMMEDIATE_ASM_STR	"ADD<c> SP, SP, #<imm>"
#define MOV_REGISTER_ASM_STR		"MOV<c> <Rd>, <Rm>"
#define MOV_IMMEDIATE_ASM_STR		"MOVS <Rd>, #<imm8>"
#define STR_IMMEDIATE_ASM_STR1		"STR<c> <Rt>, [<Rn>, #<imm>]"
#define STR_IMMEDIATE_ASM_STR2		"STR<c>.W <Rt>, [<Rn>, #<imm12>]"
#define STR_IMMEDIATE_ASM_STR3		"STR<c>.W <Rt>, [<Rn>, #+/-<imm8>]"
#define SUB_IMMEDIATE_ASM_STR1		"SUBS<c> <Rd>, <Rn>, #<imm3>"
#define SUB_IMMEDIATE_ASM_STR2		"SUBS<c> <Rdn>, #<imm8>"
#define CBNZ_ASM_STR				"CBNZ <Rn>, <label>"
#define LDRB_IMMEDIATE_ASM_STR1		"LDRB<c> <Rt>, [<Rn>, #<imm>]"
#define LDRB_IMMEDIATE_ASM_STR2		"LDRB<c>.W <Rt>, [<Rn>, #<imm12>]"
#define LDRSB_IMMEDIATE_ASM_STR		"LDRSB<c> <Rt>, [<Rn>, #<imm>]"
#define ADD32_ASM_STR				"ADD32<c> <Rd>, <Rn>, #<imm12>"
#define LDR32_ASM_STR				"LDR32<c> <Rd>, <Rn>, #<imm12>"
#else
#define BRANCH_WITHOUT_LINK_ASM_STR				""
#define BRANCH_WITHOUT_LINK_REGISTER_ASM_STR	""
#define BRANCH_WITH_LINK_ASM_STR				""
#define BRANCH_WITH_LINK_REGISTER_ASM_STR		""
#define BRANCH_WITH_LINK_LABEL_ASM_STR			""
#define STACK_PUSH_ASM_STR			""
#define STACK_PUSH_ASM_STR			""
#define STACK_POP_ASM_STR			""
#define LDR_LITERAL_ASM_STR1		""
#define LDR_LITERAL_ASM_STR2		""
#define LDR_REGISTER_ASM_STR1		""
#define LDR_REGISTER_ASM_STR2		""
#define LDR_IMMEDIATE_ASM_STR1		""
#define LDR_IMMEDIATE_ASM_STR2		""
#define LDR_IMMEDIATE_ASM_STR3		""
#define ADD_REGISTER_ASM_STR1		""
#define ADD_REGISTER_ASM_STR2		""
#define ADD_REGISTER_ASM_STR3		""
#define ADD_SP_IMMEDIATE_ASM_STR	""
#define MOV_REGISTER_ASM_STR		""
#define MOV_IMMEDIATE_ASM_STR		""
#define STR_IMMEDIATE_ASM_STR1		""
#define STR_IMMEDIATE_ASM_STR2		""
#define STR_IMMEDIATE_ASM_STR3		""
#define SUB_IMMEDIATE_ASM_STR1		""
#define SUB_IMMEDIATE_ASM_STR2		""
#define CBNZ_ASM_STR				""
#define LDRB_IMMEDIATE_ASM_STR1		""
#define LDRB_IMMEDIATE_ASM_STR2		""
#define LDRSB_IMMEDIATE_ASM_STR		""
#define ADD32_ASM_STR				""
#define LDR32_ASM_STR				""
#endif

enum InstructionType {
	UNDEFINED_INSTRUCTION = 0,

	// Thumb2 Instructions
	BRANCH_WITHOUT_LINK,
	BRANCH_WITHOUT_LINK_REGISTER,
	BRANCH_WITH_LINK,
	BRANCH_WITH_LINK_REGISTER,
	BRANCH_WITH_LINK_LABEL,
	STACK_PUSH,
	STACK_POP,
	DATA_PROCESSING,	// Add, substract, compare, move --> immediate, register
	LOAD_STORE,			// General load store, including byte, haf-word, word
	LOAD_STORE_MULTIPLE,
	LOAD_STORE_STACK,
	LDR_LITERAL,
	LDR_IMMEDIATE,
	LDR_REGISTER,
	ADD_REGISTER,
	ADD_SP_IMMEDIATE,
	MOV_REGISTER,
	MOV_IMMEDIATE,
	STR_IMMEDIATE,
	SUB_IMMEDIATE,
	CBNZ,
	LDRB_IMMEDIATE,
	LDRSB_IMMEDIATE,

	// Arm Instructions
	ADD32,
	LDR32,

	UNKNOWN_INSTRUCTION,
};

class KK_EXPORT Instruction {
public:

	InstructionType type;
	unsigned int pc;		// PC of the current instruction
	int length;				// in bytes

	unsigned int registers;			// each bit represent one register
	unsigned int opRegister;		// only one register in the operand
	unsigned int resultRegister;	// only one register in the result

	int result;			// e.g., the "result" of BL is the target address to jump
	bool resultAvailable;
	bool isPopPC;		// PC is popped from the stack (usually indicate the end of function)
	bool isPushLR;		// usually indicate the start of a function
	std::string ASMStr;	// string representation of ASM

	// ASM Oprand
	unsigned int Rm;
	unsigned int Rdn;
	unsigned int Rd;
	unsigned int Rn;
	unsigned int Rt;
	unsigned int label;
	bool toARM;
	bool add;
	int imm32;
	std::string destFuncName;	// Only available when branch with link to an external function

	// Be careful, I didn't initialize all the members!
	Instruction(InstructionType type, unsigned int pc, int length, const char *ASMStr) {
		this->pc = pc;
		this->type = type;
		this->length = length;

		this->ASMStr = ASMStr;
		this->isPopPC = false;
		this->isPushLR = false;
		this->resultAvailable = false;
		this->destFuncName = "";
	};
};


class KK_EXPORT InstructionAnalyser {
public:
	static Instruction *analyse(CPUStatus *cpu, bool thumb2 = false);
};

class InstructionMaker {
public:
	static unsigned int makeBLX(unsigned int pc, unsigned int dest, bool toArm = false);
	static short makeBxReg(unsigned int reg);
	static unsigned int makeLdrReg(unsigned int reg, unsigned int addr, unsigned int dataAddr);
};

#endif
