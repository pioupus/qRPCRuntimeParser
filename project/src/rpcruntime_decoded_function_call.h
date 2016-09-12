#ifndef RPCRUNTIMEDECODEDFUNCTIONCALL_H
#define RPCRUNTIMEDECODEDFUNCTIONCALL_H

#include "rpcruntime_decoded_param.h"

#include <vector>

/*
 * A RPCRuntimeDecodedFunctionCall represents function call made by the RPC-System, including parameters sent and their values and types
 */

class RPCRuntimeDecodedFunctionCall {
	public:
	RPCRuntimeDecodedFunctionCall(int id, std::vector<RPCRuntimeDecodedParam> params);
	std::vector<RPCRuntimeDecodedParam> get_decoded_parameters() const;
	int get_id() const;

	private:
	int id;
	std::vector<RPCRuntimeDecodedParam> params;
};

#endif //RPCRUNTIMEDECODEDFUNCTIONCALL_H
