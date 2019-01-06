
#ifndef _HOTPATCH_CPU_STATUS_H_
#define _HOTPATCH_CPU_STATUS_H_
#pragma once

enum {
	REGISTER_UNINITIALIZED,
	REGISTER_IMMEDIATE,
	REGISTER_FROMREGISTER,	// such as MOV Rd, Rm
	REGISTER_EXPRESSION,		// such as ADD Rd, Rn, Rm
};

enum {
	REGISTER_PC = 15,
	REGISTER_LR = 14,
	REGISTER_SP = 13,
};

class CPUStatus {
public:
	unsigned int PC;

	unsigned int opCode;
	unsigned int opCodeH;
	unsigned int opCodeL;

	unsigned int registers[16];
	unsigned int registerStatus[16];

	CPUStatus()  {
		CPUStatus(0);
	};

	CPUStatus (unsigned int pc) {
		for (int i = 0; i <= 15; i++) {
			registers[i] = 0;
			registerStatus[i] = REGISTER_UNINITIALIZED;
		}

		PC = pc;
		opCode = 0;
		opCodeH = 0;
		opCodeL = 0;
	};
};

#endif
