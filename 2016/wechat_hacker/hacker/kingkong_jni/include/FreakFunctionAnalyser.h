#include "FunctionAnalyser.h"

class FreakFunctionAnalyser : public FunctionAnalyser {

public:
	FreakFunctionAnalyser(unsigned int startAddress, unsigned int maxLength, AddressHelper *addressHelper, ElfAnalyser *elfAnalyser)
		: FunctionAnalyser(startAddress, maxLength, addressHelper, elfAnalyser) {
	};

	virtual ~FreakFunctionAnalyser() {};
	bool runFunctionAnalyser(unsigned int *result);
};