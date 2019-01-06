#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

#include "Common.h"
#include "Instruction.h"
#include "CPUStatus.h"
#include "AddressHelper.h"
#include "ElfAnalyser.h"

Soinfo::Soinfo() :
		hdr(NULL), phdr(NULL), phdrCount(0), shdr(NULL), shdrCount(0),
		dynamicSegment(NULL), symtab(NULL), strtab(NULL), rel(NULL), pltRel(NULL),
		pltSection(0), pltSectionSize(0), relCount(0), pltRelCount(0), pltEntry(NULL), gotEntry(NULL)
{

}

Soinfo::~Soinfo()
{
	PltEntry *entry = this->pltEntry;
	while (entry) {
		PltEntry *next = entry->next;
		delete entry;
		entry = next;
	}

	GotEntry *entry2 = this->gotEntry;
	while (entry2) {
		GotEntry *next2 = entry2->next;
		delete entry2;
		entry2 = next2;
	}
}

ElfAnalyser::ElfAnalyser(const char *libraryName, IntrestFunction *intrestFunction)
			: mElfInfo(NULL), mFd(0), mSize(0), mMmapAddr(NULL),
			  mLibraryName(libraryName), mIntrestFunction(intrestFunction)
{

}

ElfAnalyser::~ElfAnalyser()
{
	if (mElfInfo) {
		delete mElfInfo;
	}
}

unsigned int ElfAnalyser::FindPltOffsetByName(const char *name)
{
	PltEntry *pltEntry = mElfInfo->pltEntry;
	while (pltEntry) {
		if (pltEntry->name.compare(std::string(name)) == 0) {
			return pltEntry->offset;
		}
		pltEntry = pltEntry->next;
	}
	return 0;
}

std::string ElfAnalyser::FindPltNameByOffset(unsigned int theOffset)
{
	PltEntry *pltEntry = mElfInfo->pltEntry;
	while (pltEntry) {
		if (pltEntry->offset == theOffset) {
			return pltEntry->name;
		}
		pltEntry = pltEntry->next;
	}
	return std::string("");
}

bool ElfAnalyser::readProgramHead(Soinfo *soinfo, unsigned int addr)
{
	unsigned int phdrCount = soinfo->phdrCount;

	Elf32_Phdr *pPhdr = soinfo->phdr;
	for (unsigned int i = 0; i < phdrCount; i++) {
		LOGD("Program Head %d p_type %d, p_offset 0x%08x, p_filesz %d p_memsz %d\n",
			i, pPhdr->p_type, pPhdr->p_offset, pPhdr->p_filesz, pPhdr->p_memsz);
		if (pPhdr->p_type == PT_DYNAMIC) {
			soinfo->dynamicSegment = (Elf32_Dyn *)(pPhdr->p_offset + addr);
			LOGD("Find Dynamic Segment, offset 0x%08x, address 0x%08x\n", pPhdr->p_offset, (unsigned int)(soinfo->dynamicSegment));
		}
		pPhdr++;
	}

	if (!soinfo->dynamicSegment) {
		LOGD("No dynamic segment found, give up!");
		return false;
	}

	return true;
}

bool ElfAnalyser::readDynamicSegment(Soinfo *soinfo, unsigned int addr)
{
	Elf32_Dyn *pDyn = soinfo->dynamicSegment;

	while (true) {
		if (pDyn->d_tag == DT_NULL) {
			break;
		}
		switch (pDyn->d_tag) {
		case DT_SYMTAB:
			soinfo->symtab = (Elf32_Sym *)(pDyn->d_un.d_val + addr);
			LOGD("Dynamic segment Symbol table 0x%08x",(unsigned int)soinfo->symtab);
			break;
		case DT_STRTAB:
			soinfo->strtab = (char *)(pDyn->d_un.d_val + addr);
			LOGD("Dynamic segment String table 0x%08x ", (unsigned int)soinfo->strtab);
			break;
		case DT_REL:
			soinfo->rel = (Elf32_Rel *)(pDyn->d_un.d_val + addr);
			LOGD("Dynamic segment Rel table 0x%08x", (unsigned int)soinfo->rel);
			break;
		case DT_RELSZ:
			soinfo->relCount = pDyn->d_un.d_val / 8;
			LOGD("Dynamic segment Rel count %d", soinfo->relCount);
			break;
		case DT_JMPREL:
			soinfo->pltRel = (Elf32_Rel *)(pDyn->d_un.d_val + addr);
			LOGD("Dynamic segment PLT Rel table 0x%08x", (unsigned int)soinfo->pltRel);
			break;
	    case DT_PLTRELSZ:
	    	soinfo->pltRelCount = pDyn->d_un.d_val / 8;
	        LOGD("Dynamic segment PLT Rel count 0x%08x", (unsigned int)soinfo->pltRelCount);
	        break;
		default:
			break;
		}
		pDyn++;
	}

	if (soinfo->symtab == NULL || soinfo->strtab == NULL || soinfo->rel == NULL
			|| soinfo->relCount == 0 || soinfo->pltRel == NULL || soinfo->pltRelCount == 0) {
		return false;
	}

	return true;
}

GotEntry *ElfAnalyser::findGotOffset(Soinfo *soinfo, unsigned int gotOffset)
{
	GotEntry *gotEntry = soinfo->gotEntry;
	while (gotEntry) {
		if (gotEntry->offset == gotOffset)
			return gotEntry;
		else
			gotEntry = gotEntry->next;
	}
	return NULL;
}

bool ElfAnalyser::readPltEntries(Soinfo *soinfo, unsigned int baseAddr)
{
	unsigned int addr = soinfo->pltSection;
	unsigned int endAddr = soinfo->pltSection + soinfo->pltSectionSize;

	PltEntry *curEntry = NULL;
	unsigned int foundFunctionCount = 0;

	while (addr <= endAddr - 12) {

		// Start over to clear the registers
		CPUStatus *cpu = new CPUStatus(addr);
		if (!cpu) return false;

		Instruction *instr1 = InstructionAnalyser::analyse(cpu, false);
		cpu->PC = cpu->PC + 4;
		Instruction *instr2 = InstructionAnalyser::analyse(cpu, false);
		cpu->PC = cpu->PC + 4;
		Instruction *instr3 = InstructionAnalyser::analyse(cpu, false);

		if (instr1 != NULL && instr2 != NULL && instr3 != NULL) {
			if (instr1->type == ADD32 && instr1->Rd == REG_R12 && instr1->Rn == REG_PC
					&& instr2->type == ADD32 && instr2->Rd == REG_R12 && instr2->Rn == REG_R12
					&& instr3->type == LDR32 && instr3->Rd == REG_PC && instr3->Rn == REG_R12) {

				unsigned int label = instr3->label;
				GotEntry *gotEntry = findGotOffset(soinfo, label - baseAddr);

				if (gotEntry) {
					PltEntry *pltEntry = new PltEntry();
					if (!pltEntry) {
						delete cpu;
						return false;
					}
					pltEntry->offset = addr - baseAddr;
					pltEntry->gotEntry = gotEntry;
					pltEntry->next = NULL;
					if (pltEntry->gotEntry) {
						pltEntry->name = std::string(pltEntry->gotEntry->name);
						LOGD("PLT Entry offset 0x%08x to GOT 0x%08x, name %s",
								pltEntry->offset, label - baseAddr, pltEntry->name.c_str());
					} else {
						pltEntry->name = std::string("");
						LOGD("PLT Entry offset 0x%08x to GOT 0x%08x (Unable to find Got entry!!!)",
								pltEntry->offset, label - baseAddr);
					}

					if (!curEntry) {
						soinfo->pltEntry = pltEntry;
					} else {
						curEntry->next = pltEntry;
					}
					curEntry = pltEntry;

	            	if (mIntrestFunction->isAllIntrestFunctionFound(++foundFunctionCount)) {
	            		return true;
	            	}
				}
			}
		}
		addr += 4;
		delete cpu;
	}

	return true;
}

bool ElfAnalyser::readGotEntries(Soinfo *soinfo, unsigned int addr)
{
	char *strtab = soinfo->strtab;
	Elf32_Sym *symtab = soinfo->symtab;

	GotEntry *curEntry = NULL;
	unsigned int foundFunctionCount = 0;

	Elf32_Rel *rel = soinfo->pltRel;
	unsigned int count = soinfo->pltRelCount;
    for (size_t idx = 0; idx < count; ++idx, ++rel) {
        unsigned int type = ELF32_R_TYPE(rel->r_info);
        unsigned int sym = ELF32_R_SYM(rel->r_info);

        if (type == 0) continue; // R_*_NONE

        if (sym != 0) {
        	const char *name = (const char *)(strtab + symtab[sym].st_name);

            if (mIntrestFunction->isIntrestFunction(name)) {
                LOGD("Got Entry at .plt.rel index %d offset 0x%08x name %s",
                				idx, (unsigned int)rel->r_offset, name);

            	GotEntry *gotEntry = new GotEntry();
                if (!gotEntry) return false;
                gotEntry->offset = rel->r_offset;
                gotEntry->name = std::string(name);
                gotEntry->next = NULL;
                if (curEntry) {
                	curEntry->next = gotEntry;
                } else {
                	soinfo->gotEntry = gotEntry;
                }
            	curEntry = gotEntry;

            	if (mIntrestFunction->isAllIntrestFunctionFound(++foundFunctionCount)) {
            		return true;
            	}
            }
        }
    }

	rel = soinfo->rel;
	count = soinfo->relCount;
    for (size_t idx = 0; idx < count; ++idx, ++rel) {
        unsigned int type = ELF32_R_TYPE(rel->r_info);
        unsigned int sym = ELF32_R_SYM(rel->r_info);

        if (type == 0)	continue;	// R_*_NONE

        if (sym != 0) {

        	const char *name = (const char *)(strtab + symtab[sym].st_name);

            if (mIntrestFunction->isIntrestFunction(name)) {

                LOGD("Got Entry at .rel.dyn index %d offset 0x%08x name %s \n",
                				idx, (unsigned int)rel->r_offset, name);

				GotEntry *gotEntry = new GotEntry();
				if (!gotEntry) return false;
				gotEntry->offset = rel->r_offset;
				gotEntry->name = std::string(name);
				gotEntry->next = NULL;
				if (curEntry) {
					curEntry->next = gotEntry;
				} else {
					soinfo->gotEntry = gotEntry;
				}
				curEntry = gotEntry;

            	if (mIntrestFunction->isAllIntrestFunctionFound(++foundFunctionCount)) {
            		return true;
            	}
            }
        }
    }


    return true;
}

bool ElfAnalyser::findPltSection(Soinfo *soinfo, unsigned int baseAddr)
{
	Elf32_Shdr *pShdr = soinfo->shdr;
	unsigned int count = soinfo->shdrCount;

	// Find the shstrtab first
	Elf32_Shdr *pShStrTab = NULL;
	for (unsigned int i = 0; i < count; i++) {
		LOGD("sh_type %d, sh_addr 0x%08x, sh_offset 0x%08x, sh_size %d sh_addralign %d\n",
			pShdr->sh_type, (unsigned int)(pShdr->sh_addr), (pShdr->sh_offset), pShdr->sh_size, pShdr->sh_addralign);
		if (pShdr->sh_type == SHT_STRTAB) {
			if (pShdr->sh_name < pShdr->sh_size) {
				char *name = (char *)((pShdr->sh_offset + baseAddr) + pShdr->sh_name);
				if (!strcmp(name, ".shstrtab")) {
					pShStrTab = pShdr;
					LOGD("Find .shstrtab !");
					break;
				}
			}
		}
		pShdr++;
	}

	if (!pShStrTab) return false;

	pShdr = soinfo->shdr;
	unsigned int strtab = pShStrTab->sh_offset + baseAddr;
	for (unsigned int i = 0; i < count; i++) {
		char *name = (char *)(strtab + pShdr->sh_name);
		LOGD("Section %s", name);
		if (!strcmp(name, ".plt")) {

			soinfo->pltSection = (unsigned int)pShdr->sh_offset + baseAddr;
			soinfo->pltSectionSize = pShdr->sh_size;
			LOGD("Find .plt section address 0x%08x, size %d", soinfo->pltSection, soinfo->pltSectionSize);

			return true;
		}
		pShdr++;
	}
	return false;
}

bool ElfAnalyser::readElf(Soinfo *soinfo, unsigned int addr)
{
	// Elf Head
	soinfo->hdr = (Elf32_Ehdr *)addr;

	// Program Head
	VERIFY_ELF_OFFSET(soinfo->hdr->e_phoff);
	soinfo->phdr = (Elf32_Phdr *)(soinfo->hdr->e_phoff + addr);
	soinfo->phdrCount = soinfo->hdr->e_phnum;

	// Section Header
	VERIFY_ELF_OFFSET(soinfo->hdr->e_shoff);
	soinfo->shdr = (Elf32_Shdr *)(soinfo->hdr->e_shoff + addr);
	soinfo->shdrCount = soinfo->hdr->e_shnum;

	LOGD("Elf Head 0x%08x ; Program Head 0x%08x, count %d ; Section Head 0x%08x, count %d",
			(unsigned int)soinfo->hdr,
			(unsigned int)soinfo->phdr, soinfo->phdrCount,
			(unsigned int)soinfo->shdr, soinfo->shdrCount);


	if (!readProgramHead(soinfo, addr)) return false;

	if (!readDynamicSegment(soinfo, addr)) return false;

	if (!findPltSection(soinfo, addr)) return false;

	// Should read got entries first
	// And then read plt entries
	if (!readGotEntries(soinfo, addr)) return false;
	if (!readPltEntries(soinfo, addr)) return false;

	return true;
}

bool ElfAnalyser::analyse()
{
	mElfInfo = new Soinfo();

	if (!mElfInfo) return false;

	mFd = open(mLibraryName, O_RDONLY);
	if (mFd == -1) {
		LOGE("Unable to open file %s", mLibraryName);
		goto bail;
	}

	struct stat st;
	if (fstat(mFd, &st) == -1) {
		LOGE("Unable to stat file %s", mLibraryName);
		goto bail1;
	}

	mMmapAddr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, mFd, 0);
	if (mMmapAddr == NULL) {
		LOGE("Unable to do mmap\n");
		goto bail1;
	}

	mSize = st.st_size;
	LOGD("Analysing %s ... mmap to 0x%08x, size %ld \n", mLibraryName, (unsigned int)mMmapAddr, mSize);

	if (!readElf(mElfInfo, (unsigned int)mMmapAddr)) goto bail2;

	munmap(mMmapAddr, mSize);
	close(mFd);

	LOGD("Great, well done~");
	return true;

bail2:
	munmap(mMmapAddr, mSize);
bail1:
	close(mFd);
bail:
	delete mElfInfo;
	return false;
}
