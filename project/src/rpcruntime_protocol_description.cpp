#include "rpcruntime_protocol_description.h"

#include <fstream>

bool RPCRunTimeProtocolDescription::openProtocolDescription(std::istream &input) {
	(void)input;
	return false;
}

const std::vector<RPCRuntimeFunction> RPCRunTimeProtocolDescription::get_functions() const
{
	return functions;
}
