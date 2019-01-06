#include "FunctionAnalyser.h"

class PngFunctionAnalyser : public FunctionAnalyser {

public:
	PngFunctionAnalyser(unsigned int startAddress, unsigned int maxLength, AddressHelper *addressHelper, ElfAnalyser *elfAnalyser)
		: FunctionAnalyser(startAddress, maxLength, addressHelper, elfAnalyser) {
	};

	virtual ~PngFunctionAnalyser() {};
	virtual bool runFunctionAnalyser(unsigned int *result);
};

class SerializedSizeAnalyser : public FunctionAnalyser {

public:
	SerializedSizeAnalyser(unsigned int startAddress, unsigned int maxLength, AddressHelper *addressHelper, ElfAnalyser *elfAnalyser)
		: FunctionAnalyser(startAddress, maxLength, addressHelper, elfAnalyser) {
	};

	virtual ~SerializedSizeAnalyser() {};
	virtual bool runFunctionAnalyser(unsigned int *result);
};
