
#ifndef _HOTPATCH_CACHEDLIBUTIL_INLUCDE_H_
#define _HOTPATCH_CACHEDLIBUTIL_INLUCDE_H_
#pragma once

#include <pthread.h>
#include <string>
#include <map>
#include <dlfcn.h>
#include "Common.h"

class CachedLibUtil
{
public:
	static std::map<std::string, void *> Libs;

	static void* openLib(const char *libName);
};

#endif
