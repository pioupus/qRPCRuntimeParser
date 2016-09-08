#include "rpcruntime_function.h"

int RPCRuntimeFunction::get_request_id() const
{
	return function_id * 2;
}

int RPCRuntimeFunction::get_reply_id() const
{
	return function_id * 2 + 1;
}

const std::vector<RPCRuntimeParameterDescription> &RPCRuntimeFunction::get_request_parameters() const
{
	return request_parameters;
}

const std::vector<RPCRuntimeParameterDescription> &RPCRuntimeFunction::get_reply_parameters() const
{
	return reply_parameters;
}
