#include "rpcruntime_paramter_description.h"

#include <sstream>
#include <cassert>

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeIntegerParameter integer)
	: type(RPCRuntimeParameterDescription::Type::integer)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.integer = new RPCRuntimeIntegerParameter(std::move(integer));
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeEnumerationParameter enumeration)
	: type(RPCRuntimeParameterDescription::Type::enumeration)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.enumeration = new RPCRuntimeEnumerationParameter(std::move(enumeration));
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeStructureParameter structure)
	: type(RPCRuntimeParameterDescription::Type::structure)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.structure = new RPCRuntimeStructureParameter(std::move(structure));
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeArrayParameter array)
	: type(RPCRuntimeParameterDescription::Type::array)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.array = new RPCRuntimeArrayParameter(std::move(array));
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position,
															   RPCRuntimeCharacterParameter character)
	: type(RPCRuntimeParameterDescription::Type::character)
	, bit_size(bit_size)
	, parameter_name(std::move(name))
	, parameter_ctype(std::move(ctype))
	, parameter_position(position) {
	type_dependent_values.character = new RPCRuntimeCharacterParameter(std::move(character));
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
	return *type_dependent_values.integer;
}

const RPCRuntimeEnumerationParameter &RPCRuntimeParameterDescription::as_enumeration() const {
	assert(type == Type::enumeration);
	return *type_dependent_values.enumeration;
}

const RPCRuntimeStructureParameter &RPCRuntimeParameterDescription::as_structure() const {
	assert(type == Type::structure);
	return *type_dependent_values.structure;
}

const RPCRuntimeArrayParameter &RPCRuntimeParameterDescription::as_array() const {
	assert(type == Type::array);
	return *type_dependent_values.array;
}

const RPCRuntimeCharacterParameter &RPCRuntimeParameterDescription::as_character() const {
	assert(type == Type::character);
	return *type_dependent_values.character;
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(const RPCRuntimeParameterDescription &other)
	: type(other.type)
	, bit_size(other.bit_size)
	, parameter_name(other.parameter_name)
	, parameter_ctype(other.parameter_ctype)
    , parameter_position(other.parameter_position)  {
	switch (type) {
		case Type::array:
			type_dependent_values.array = new RPCRuntimeArrayParameter(*other.type_dependent_values.array);
			break;
		case Type::character:
			type_dependent_values.character = new RPCRuntimeCharacterParameter(*other.type_dependent_values.character);
			break;
		case Type::enumeration:
			type_dependent_values.enumeration = new RPCRuntimeEnumerationParameter(*other.type_dependent_values.enumeration);
			break;
		case Type::integer:
			type_dependent_values.integer = new RPCRuntimeIntegerParameter(*other.type_dependent_values.integer);
			break;
		case Type::structure:
			type_dependent_values.structure = new RPCRuntimeStructureParameter(*other.type_dependent_values.structure);
			break;
	}
}

RPCRuntimeParameterDescription::RPCRuntimeParameterDescription(RPCRuntimeParameterDescription &&other)
	: type(other.type)
	, bit_size(other.bit_size)
	, parameter_name(std::move(other.parameter_name))
	, parameter_ctype(std::move(other.parameter_ctype))
    , parameter_position(other.parameter_position)   {
	switch (type) {
		case Type::array:
			type_dependent_values.array = other.type_dependent_values.array;
			other.type_dependent_values.array = nullptr;
			break;
		case Type::character:
			type_dependent_values.character = other.type_dependent_values.character;
			other.type_dependent_values.character = nullptr;
			break;
		case Type::enumeration:
			type_dependent_values.enumeration = other.type_dependent_values.enumeration;
			other.type_dependent_values.enumeration = nullptr;
			break;
		case Type::integer:
			type_dependent_values.integer = other.type_dependent_values.integer;
			other.type_dependent_values.integer = nullptr;
			break;
		case Type::structure:
			type_dependent_values.structure = other.type_dependent_values.structure;
			other.type_dependent_values.structure = nullptr;
			break;
	}
}

RPCRuntimeParameterDescription::~RPCRuntimeParameterDescription() {
	switch (type) {
		case Type::array:
			delete type_dependent_values.array;
			break;
		case Type::character:
			delete type_dependent_values.character;
			break;
		case Type::enumeration:
			delete type_dependent_values.enumeration;
			break;
		case Type::integer:
			delete type_dependent_values.integer;
			break;
		case Type::structure:
			delete type_dependent_values.structure;
			break;
	}
}

void RPCRuntimeParameterDescription::fix_array_bit_byte_bug()
{
	//the bits reported in an array are incorrectly reported in bytes, not in bits
	bit_size *= 8;
}

RPCRuntimeArrayParameter::RPCRuntimeArrayParameter(RPCRuntimeParameterDescription type, int number_of_elements)
    : type(std::move(type))
    , number_of_elements(number_of_elements) {}

int RPCRuntimeEnumerationParameter::Enum_value::to_int() const
{
	int retval;
	std::stringstream ss(value);
    if (!(ss >> std::noskipws >> retval)){
        throw std::domain_error("RPC: empty enum string cannot be converted to int");
	}
	return retval;
}

bool RPCRuntimeEnumerationParameter::Enum_value::is_int() const
{
	int retval = 0;
	std::stringstream ss(value);
    return static_cast<bool>(ss >> std::noskipws >> retval);
}
