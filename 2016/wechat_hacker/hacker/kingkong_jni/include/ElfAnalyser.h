#ifndef _HOTPATCH_ELF_ANALYZER_H_
#define _HOTPATCH_ELF_ANALYZER_H_
#pragma once

#include <string>
#include <linux/elf.h>
#include "Common.h"

int ElfGetPltOffset(const char *, const char *);
struct _PltEntry;
struct _GotEntry;

typedef struct _PltEntry {
	std::string name;
	unsigned int offset;
	struct _GotEntry *gotEntry;
	struct _PltEntry *next;
}PltEntry;

typedef struct _GotEntry {
	std::string name;
	unsigned int offset;
	struct _PltEntry *pltEntry;
	struct _GotEntry *next;
}GotEntry;

class IntrestFunction {
public:
	IntrestFunction(const char **intrestFunction, unsigned int count)
		:mIntrestFunction(intrestFunction), mTotalFunctionCount(count) {};
	bool isIntrestFunction(const char *function) {
		for (unsigned int i = 0; i < mTotalFunctionCount; i++) {
			if (strcmp(function, mIntrestFunction[i]) == 0) {
				return true;
			}
		}
		return false;
	};
	bool isAllIntrestFunctionFound(unsigned int foundFunctionCount) {
		return foundFunctionCount == mTotalFunctionCount;
	};
private:
	unsigned int mTotalFunctionCount;
	const char **mIntrestFunction;
};

class KK_EXPORT Soinfo {

public:
	// Elf head (Absolute address)
	Elf32_Ehdr *hdr;

	// Program head	(Absolute address)
	Elf32_Phdr *phdr;
	unsigned int phdrCount;

	// Section head	(Absolute address)
	Elf32_Shdr *shdr;
	unsigned int shdrCount;

	// Dynamic segment	(Absolute address)
	Elf32_Dyn *dynamicSegment;

	// (Absolute address)
	Elf32_Sym *symtab;
	char *strtab;
	Elf32_Rel *rel;
	Elf32_Rel *pltRel;
	unsigned int pltSection;
	unsigned int pltSectionSize;
	unsigned int relCount;
	unsigned int pltRelCount;

	PltEntry *pltEntry;
	GotEntry *gotEntry;

	Soinfo();

	~Soinfo();
};

class KK_EXPORT ElfAnalyser {

public:
	Soinfo *mElfInfo;
	ElfAnalyser(const char *libraryName, IntrestFunction *intrestFunction);
	~ElfAnalyser();
	bool analyse();
	unsigned int FindPltOffsetByName(const char *name);
	std::string FindPltNameByOffset(unsigned int offset);

private:
	int mFd;
	long mSize;
	void *mMmapAddr;
	const char *mLibraryName;
	IntrestFunction *mIntrestFunction;

	bool readElf(Soinfo *soinfo, unsigned int addr);
	bool readProgramHead(Soinfo *soinfo, unsigned int addr);
	bool readDynamicSegment(Soinfo *soinfo, unsigned int addr);
	GotEntry *findGotOffset(Soinfo *soinfo, unsigned int gotOffset);
	bool readPltEntries(Soinfo *soinfo, unsigned int baseAddr);
	bool readGotEntries(Soinfo *soinfo, unsigned int baseAddr);
	bool findPltSection(Soinfo *soinfo, unsigned int baseAddr);
};

// Do simple check to make sure an offset is valid in ELF
#define VERIFY_ELF_OFFSET(offset) 	\
		do{							\
			if ((offset) >= mSize)	\
				return false;		\
		} while(0);

#endif
