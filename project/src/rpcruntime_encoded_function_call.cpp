#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_encoded_param.h"
#include "rpcruntime_function.h"

#include <algorithm>
#include <cassert>

RPCRuntimeEncodedFunctionCall::RPCRuntimeEncodedFunctionCall(const RPCRuntimeFunction &function)
	: function(&function) {
	for (auto &param : function.get_request_parameters()){
		params.push_back(param);
	}
}

int RPCRuntimeEncodedFunctionCall::get_parameter_count() const {
	return params.size();
}

bool RPCRuntimeEncodedFunctionCall::are_all_values_set() const {
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

RPCRuntimeEncodedParam &RPCRuntimeEncodedFunctionCall::get_parameter(int index)
{
	assert(index >= 0);
	assert(index < static_cast<int>(params.size()));
	return params[index];
}

const RPCRuntimeEncodedParam &RPCRuntimeEncodedFunctionCall::get_parameter(int index) const
{
	return params[index];
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedFunctionCall::get_parameter(const std::string &name)
{
	for (auto &param : params){
		if (param.get_description()->get_parameter_name() == name){
			return param;
		}
	}
    throw std::runtime_error("RPC: invalid parameter name: " + name);
}

const RPCRuntimeFunction *RPCRuntimeEncodedFunctionCall::get_description() const
{
	return function;
}
