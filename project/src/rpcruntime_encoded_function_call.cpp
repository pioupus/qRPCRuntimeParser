#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_encoded_param.h"
#include "rpcruntime_function.h"

#include <algorithm>

RPCRuntimeEncodedFunctionCall::RPCRuntimeEncodedFunctionCall(const RPCRuntimeFunction &function)
	: function(&function) {}

int RPCRuntimeEncodedFunctionCall::get_parameter_count() const {
	return params.size();
}

bool RPCRuntimeEncodedFunctionCall::all_values_set() const {
	return std::all_of(std::begin(params), std::end(params), [](const RPCRuntimeEncodedParam &param) { return param.are_all_values_set(); });
}

std::vector<unsigned char> RPCRuntimeEncodedFunctionCall::encode() const
{
	std::vector<unsigned char> retval;
	retval.push_back(function->get_request_id());
	for (auto &param : params){
		param.encode(retval);
	}
	return retval;
}
