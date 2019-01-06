
#include "AddressHelper.h"
#include <unistd.h>
#include <sys/mman.h>

#define LOG_TAG "AddressBoundary"

AddressHelper::AddressHelper(const char *name)
{
	libraryName = name;
	baseAddress = 0;
	getMemoryAreas();
}

AddressHelper::~AddressHelper()
{
	for (UINT32 i = 0; i < memoryAreas.size(); i++) {
		MemoryArea *pArea = memoryAreas[i];
		delete pArea;
	}
	memoryAreas.clear();
}

bool AddressHelper::checkAddress(UINT32 address,
		bool enforceReadable, bool enforceWriteable, bool enforceExecutable)
{

	for (UINT32 i = 0; i < memoryAreas.size(); i++) {
		MemoryArea *pArea = memoryAreas[i];
		if (pArea->startAddress <= address && pArea->endAddress > address) {
			if (enforceReadable && !(pArea->read)) return false;
			if (enforceWriteable && !(pArea->write)) return false;
			if (enforceExecutable && !(pArea->execute)) return false;
			return true;
		}
	}
	return false;
}

void AddressHelper::getMemoryAreas()
{
	char path[256];
	char buff[256];
	int lenLibName = libraryName.length();
	FILE* file;
	UINT32  addr = 0;

	snprintf(path, sizeof path, "/proc/%d/maps", getpid());
	file = fopen(path, "rt");

	if (file == NULL)
		return;

	while (fgets(buff, sizeof buff, file) != NULL) {
		int  len = strlen(buff);
		if (len > 0 && buff[len-1] == '\n') {
			buff[--len] = '\0';
		}

		if (len <= lenLibName ||
			libraryName.compare(buff + len - lenLibName) != 0) {
			continue;
		}

		UINT32 start, end, offset;
		char flags[4];
		if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end,
			&flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7) {
			continue;
		}

		MemoryArea *pArea = new MemoryArea
			(start, end, flags[0] == 'r', flags[1] == 'w', flags[2] == 'x');
		memoryAreas.push_back(pArea);

		if (baseAddress == 0 || baseAddress > start) {
			baseAddress = start;
			LOGD("Library %s base : 0x%08x", libraryName.c_str(), baseAddress);
		}

		LOGD("%08x-%08x, %c, %c %c, %s",
			start, end, flags[0] , flags[1], flags[2],
			libraryName.c_str());
	}

	fclose(file);
}

int AddressHelper::getAddressProperty(UINT32 address) {

	int property = 0;
	char path[256];
	char buff[256];
	FILE* file;
	UINT32  addr = 0;

	LOGD("Get property for address 0x%08x", address);

	snprintf(path, sizeof path, "/proc/%d/maps", getpid());
	file = fopen(path, "rt");

	if (file == NULL)
		return false;

	while (fgets(buff, sizeof buff, file) != NULL) {
		int  len = strlen(buff);
		if (len > 0 && buff[len-1] == '\n') {
			buff[--len] = '\0';
		}

		UINT32 start, end, offset;
		char flags[4];
		if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end,
					&flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7) {
			continue;
		}

		LOGD("%08x-%08x, %c, %c, %c",
			start, end, flags[0] , flags[1], flags[2]);

		if (start <= address && end > address) {
			LOGD("Find the property %c %c %c", flags[0], flags[1], flags[2]);

			if (flags[0] == 'r') property |= PROT_READ;
			if (flags[1] == 'w') property |= PROT_WRITE;
			if (flags[2] == 'x') property |= PROT_EXEC;
			break;
		}
	}

	fclose(file);
	return property;
}

unsigned int AddressHelper::getBaseAddress()
{
	return baseAddress;
}

bool AddressHelper::makeWritable(unsigned int addr) {

	unsigned int page = addr & (~(PAGESIZE - 1));
	if (mprotect((void *)page, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC) < 0) {
    		LOGD("Unable to change memory protect at %08x", addr);
    		return false;
  	} else {
  		return true;
  	}
}
