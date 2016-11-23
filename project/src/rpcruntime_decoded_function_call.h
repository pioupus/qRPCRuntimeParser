#ifndef RPCRUNTIMEDECODEDFUNCTIONCALL_H
#define RPCRUNTIMEDECODEDFUNCTIONCALL_H

#include "rpcruntime_decoded_param.h"

class RPCRuntimeFunction;

#include <functional>
#include <vector>

/*
 * A RPCRuntimeDecodedFunctionCall represents function call made by the RPC-System, including parameters sent and their values and types
 */

class RPCRuntimeDecodedFunctionCall {
	public:
	RPCRuntimeDecodedFunctionCall(int id, std::vector<RPCRuntimeDecodedParam> params, const RPCRuntimeFunction &function);
	std::vector<RPCRuntimeDecodedParam> get_decoded_parameters() const;
	const RPCRuntimeDecodedParam *get_parameter_by_name(const std::string &name) const;
	int get_id() const;
	const RPCRuntimeFunction *get_declaration() const;
	bool is_request() const;
	bool is_reply() const;

	private:
	int id;
	std::vector<RPCRuntimeDecodedParam> params;
	const RPCRuntimeFunction *function;
};

#endif //RPCRUNTIMEDECODEDFUNCTIONCALL_H
