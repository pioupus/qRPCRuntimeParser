#include "rpcruntime_paramter_description.h"

#include <cassert>

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeIntegerParameter integer)
	: type(RPCRuntimeParameterDescription::Type::integer)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.integer = std::move(integer);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeEnumerationParameter enumeration)
	: type(RPCRuntimeParameterDescription::Type::enumeration)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.enumeration = std::move(enumeration);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeStructureParameter structure)
	: type(RPCRuntimeParameterDescription::Type::structure)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.structure = std::move(structure);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeArrayParameter array)
	: type(RPCRuntimeParameterDescription::Type::array)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.array = std::move(array);
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeCharacterParameter character)
	: type(RPCRuntimeParameterDescription::Type::array)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.character = std::move(character);
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

const std::string &RPCRuntimeParameterDescription::get_parameter_type() const {
	return parameter_ctype;
}

int RPCRuntimeParameterDescription::get_parameter_position() const {
	return parameter_position;
}

const RPCRuntimeIntegerParameter &RPCRuntimeParameterDescription::as_integer() const {
	assert(type == Type::integer);
	return type_dependent_values.integer;
}

const RPCRuntimeEnumerationParameter &RPCRuntimeParameterDescription::as_enumeration() const {
	assert(type == Type::enumeration);
	return type_dependent_values.enumeration;
}

const RPCRuntimeStructureParameter &RPCRuntimeParameterDescription::as_structure() const {
	assert(type == Type::structure);
	return type_dependent_values.structure;
}

const RPCRuntimeArrayParameter &RPCRuntimeParameterDescription::as_array() const {
	assert(type == Type::array);
	return type_dependent_values.array;
}

const RPCRuntimeCharacterParameter &RPCRuntimeParameterDescription::as_character() const {
	assert(type == Type::character);
	return type_dependent_values.character;
}

RPCRuntimeArrayParameter::RPCRuntimeArrayParameter() {}

RPCRuntimeArrayParameter::RPCRuntimeArrayParameter(const RPCRuntimeArrayParameter &other) {
	if (other.type != nullptr) {
		type = std::make_unique<RPCRuntimeParameterDescription>(*other.type);
		number_of_elements = other.number_of_elements;
	}
}
