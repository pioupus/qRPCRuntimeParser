#include "rpcruntime_function.h"

#include <algorithm>
#include <numeric>

RPCRuntimeFunction::RPCRuntimeFunction(int request_id, int reply_id, std::vector<RPCRuntimeParameterDescription> request_parameters,
									   std::vector<RPCRuntimeParameterDescription> reply_parameters, std::string function_name,
									   std::string function_declaration)
	: request_id(request_id)
	, reply_id(reply_id)
	, request_parameters(std::move(request_parameters))
	, reply_parameters(std::move(reply_parameters))
	, function_name(std::move(function_name))
	, function_declaration(std::move(function_declaration)) {}

const std::string &RPCRuntimeFunction::get_function_name() const {
	return function_name;
}

const std::string &RPCRuntimeFunction::get_function_declaration() const {
	return function_declaration;
}

int RPCRuntimeFunction::get_request_id() const {
	return request_id;
}

int RPCRuntimeFunction::get_reply_id() const {
	return reply_id;
}

const std::vector<RPCRuntimeParameterDescription> &RPCRuntimeFunction::get_request_parameters() const {
	return request_parameters;
}

const std::vector<RPCRuntimeParameterDescription> &RPCRuntimeFunction::get_reply_parameters() const {
	return reply_parameters;
}

int RPCRuntimeFunction::get_request_size() const
{
	return std::accumulate(std::begin(request_parameters), std::end(request_parameters), 1, [](int sum, const RPCRuntimeParameterDescription &param){
		return sum + param.get_bit_size() / 8;
	});
}

int RPCRuntimeFunction::get_reply_size() const
{
	return std::accumulate(std::begin(reply_parameters), std::end(reply_parameters), 1, [](int sum, const RPCRuntimeParameterDescription &param){
		return sum + param.get_bit_size() / 8;
	});
}
