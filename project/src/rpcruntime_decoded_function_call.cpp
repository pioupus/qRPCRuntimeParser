#include "rpcruntime_decoded_function_call.h"

RPCRuntimeDecodedFunctionCall::RPCRuntimeDecodedFunctionCall(int id, std::vector<RPCRuntimeDecodedParam> params)
	: id(id)
	, params(std::move(params)) {}

std::vector<RPCRuntimeDecodedParam> RPCRuntimeDecodedFunctionCall::get_decoded_parameters() const {
	return params;
}

int RPCRuntimeDecodedFunctionCall::get_id() const {
	return id;
}
