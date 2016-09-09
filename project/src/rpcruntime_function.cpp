#include "rpcruntime_function.h"

RPCRuntimeFunction::RPCRuntimeFunction(int request_id, int reply_id, std::vector<RPCRuntimeParameterDescription> request_parameters,
									   std::vector<RPCRuntimeParameterDescription> reply_parameters, std::string function_name,
									   std::string function_declaration)
	: request_id(request_id)
	, reply_id(reply_id)
	, request_parameters(std::move(request_parameters))
	, reply_parameters(std::move(reply_parameters))
	, function_name(std::move(function_name))
	, function_declaration(std::move(function_declaration)) {}

std::string RPCRuntimeFunction::get_function_name() const {
	return function_name;
}

std::string RPCRuntimeFunction::get_function_declaration() const {
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
