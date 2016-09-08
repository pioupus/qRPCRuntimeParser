#ifndef RPCRUNTIMEPROTOCOLDESCRIPTION_H
#define RPCRUNTIMEPROTOCOLDESCRIPTION_H

#include "rpcruntime_function.h"

#include <istream>

/*
 * A RPCRunTimeProtocolDescription describes the Protocol created by the RPC-Generator loaded from an XML-File
*/

class RPCRunTimeProtocolDescription {
	public:
	bool openProtocolDescription(std::istream &input);
	const std::vector<RPCRuntimeFunction> get_functions() const;

	private:
	std::vector<RPCRuntimeFunction> functions;
};

#endif // RPCRUNTIMEINTERPRETER_H
