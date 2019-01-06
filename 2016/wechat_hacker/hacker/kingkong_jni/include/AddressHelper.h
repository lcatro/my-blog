#ifndef _HOT_PATCH_ADDRESS_BOUNDARY_H__
#define _HOT_PATCH_ADDRESS_BOUNDARY_H__
#pragma once

#include "Common.h"
#include <vector>
#include <string>

class KK_EXPORT AddressHelper {
public:
	class MemoryArea {
	public:
		UINT32 startAddress;
		UINT32 endAddress;
		bool read;
		bool write;
		bool execute;

		MemoryArea(UINT32 start, UINT32 end, bool reable, bool writable, bool executable)
			: startAddress(start), endAddress(end),
			read(reable), write(writable), execute(executable)
		{

		};
	};

	std::vector<MemoryArea *> memoryAreas;
	std::string libraryName;
	unsigned int baseAddress;

	AddressHelper(const char *libraryName);
	~AddressHelper();

	bool checkAddress(UINT32 address,
		bool enforceReadable, bool enforceWritable, bool enforceExecutable);

	unsigned int getBaseAddress();

	static int getAddressProperty(UINT32 address);
	static bool makeWritable(UINT32 address);
private:
	void getMemoryAreas();
};

#endif
