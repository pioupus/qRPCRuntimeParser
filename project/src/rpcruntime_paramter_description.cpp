#include "rpcruntime_paramter_description.h"

#include <cassert>

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, RPCRuntimeIntegerParameter integer)
	: type(RPCRuntimeParameterDescription::Type::integer)
	, bit_size(bit_size)
	, parameter_name(std::move(parameter_name)) {
	type_dependent_values.integer = std::move(integer);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, RPCRuntimeEnumerationParameter enumeration)
	: type(RPCRuntimeParameterDescription::Type::enumeration)
	, bit_size(bit_size)
	, parameter_name(std::move(parameter_name)) {
	type_dependent_values.enumeration = std::move(enumeration);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, RPCRuntimeStructureParameter structure)
	: type(RPCRuntimeParameterDescription::Type::structure)
	, bit_size(bit_size)
	, parameter_name(std::move(parameter_name)) {
	type_dependent_values.structure = std::move(structure);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, RPCRuntimeArrayParameter array)
	: type(RPCRuntimeParameterDescription::Type::array)
	, bit_size(bit_size)
	, parameter_name(std::move(parameter_name)) {
	type_dependent_values.array = std::move(array);
}

RPCRuntimeParameterDescription::Type RPCRuntimeParameterDescription::get_type() const {
	return type;
}

int RPCRuntimeParameterDescription::get_bit_size() const {
	return bit_size;
}

const std::string &RPCRuntimeParameterDescription::get_parameter_name() const {
	return parameter_name;
}

int RPCRuntimeParameterDescription::get_parameter_position() const {
	return parameter_position;
}

const RPCRuntimeIntegerParameter &RPCRuntimeParameterDescription::as_integer() const {
	assert(type == Type::integer);
	return type_dependent_values.integer;
}

const RPCRuntimeEnumerationParameter &RPCRuntimeParameterDescription::as_enumeration() const {
	assert(type == Type::integer);
	return type_dependent_values.enumeration;
}

const RPCRuntimeStructureParameter &RPCRuntimeParameterDescription::as_structure() const {
	return type_dependent_values.structure;
}

const RPCRuntimeArrayParameter &RPCRuntimeParameterDescription::as_array() const {
	return type_dependent_values.array;
}
