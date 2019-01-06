
#include "CachedLibUtil.h"

static pthread_mutex_t libMutex;
std::map<std::string, void *> CachedLibUtil::Libs;

void* CachedLibUtil::openLib(const char *libName)
{

	pthread_mutex_lock(&libMutex);

	std::map<std::string, void *>::iterator iter = Libs.find(libName);
	void *handle = NULL;

	if (iter == Libs.end()) {
		handle = dlopen(libName, RTLD_NOW);
		Libs.insert(std::map<std::string, void *>::value_type(libName, handle));
		LOGD("Cached new library handle %s, 0x%08x", libName, (unsigned int)handle);
	} else {
		handle = iter->second;
		LOGD("Get cached library handle %s, 0x%08x", libName, (unsigned int)handle);
	}

	pthread_mutex_unlock(&libMutex);

	return handle;
}
