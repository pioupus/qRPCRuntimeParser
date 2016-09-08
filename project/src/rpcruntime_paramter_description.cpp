#include "rpcruntime_paramter_description.h"

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, std::string parameter_type)
	: bit_size(bit_size)
	, parameter_name(std::move(parameter_name))
	, parameter_type(std::move(parameter_type)) {}

int RPCRuntimeParameterDescription::get_bit_size() const {
	return bit_size;
}

const std::string &RPCRuntimeParameterDescription::get_parameter_name() const {
	return parameter_name;
}

const std::string &RPCRuntimeParameterDescription::get_parameter_type() const {
	return parameter_type;
}
