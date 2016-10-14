#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_function.h"

RPCRuntimeDecodedFunctionCall::RPCRuntimeDecodedFunctionCall(int id, std::vector<RPCRuntimeDecodedParam> params, const RPCRuntimeFunction &function)
	: id(id)
	, params(std::move(params))
	, function(&function) {}

std::vector<RPCRuntimeDecodedParam> RPCRuntimeDecodedFunctionCall::get_decoded_parameters() const {
	return params;
}

int RPCRuntimeDecodedFunctionCall::get_id() const {
	return id;
}

const RPCRuntimeFunction *RPCRuntimeDecodedFunctionCall::get_declaration() const
{
	return function;
}

bool RPCRuntimeDecodedFunctionCall::is_request() const
{
	return id % 2 == 0;
}

bool RPCRuntimeDecodedFunctionCall::is_reply() const
{
	return id % 2 == 1;
}
