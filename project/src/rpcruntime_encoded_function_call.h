#ifndef RPCRUNTIMEENCODEDFUNCTIONCALL_H
#define RPCRUNTIMEENCODEDFUNCTIONCALL_H

#include "rpcruntime_encoded_param.h"

#include <string>
#include <vector>

class RPCRuntimeFunction;

/*
 * A RPCRuntimeEncodedFunctionCall represents a function call to be sent to the RPC-System, allowing setting required parameters
 */

class RPCRuntimeEncodedFunctionCall {
	public:
	RPCRuntimeEncodedFunctionCall(const RPCRuntimeFunction &function);

	int get_parameter_count() const;
	bool is_all_values_set() const;
	std::vector<unsigned char> encode() const;
	RPCRuntimeEncodedParam &get_parameter(int index);
	const RPCRuntimeEncodedParam &get_parameter(int index) const;
	RPCRuntimeEncodedParam &get_parameter(const std::string &name);
	const RPCRuntimeFunction *get_description() const;

	private:
	std::vector<RPCRuntimeEncodedParam> params;
	const RPCRuntimeFunction *function;
};

#endif //RPCRUNTIMEENCODEDFUNCTIONCALL_H
