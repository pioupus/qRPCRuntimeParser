#include "rpcruntime_encoded_param.h"

#include <algorithm>
#include <stdexcept>

RPCRuntimeEncodedParam::RPCRuntimeEncodedParam(const RPCRuntimeParameterDescription &parameter_description)
	: description(&parameter_description) {
	if (description->get_type() == RPCRuntimeParameterDescription::Type::array) {
		for (int i = 0; i < description->as_array().number_of_elements; i++) {
			child_parameters.emplace_back(description->as_array().type);
		}
	} else if (description->get_type() == RPCRuntimeParameterDescription::Type::structure) {
		for (auto &member : description->as_structure().members) {
			child_parameters.emplace_back(member);
		}
	}
}

bool RPCRuntimeEncodedParam::are_all_values_set() const {
	if (child_parameters.empty()) {
		return description->get_bit_size() / 8 == static_cast<int>(data.size());
	}
	return std::all_of(std::begin(child_parameters), std::end(child_parameters),
					   [](const RPCRuntimeEncodedParam &param) { return param.are_all_values_set(); });
}

bool RPCRuntimeEncodedParam::is_integral_type() const {
	auto type = description->get_type();
	return type == RPCRuntimeParameterDescription::Type::integer || type == RPCRuntimeParameterDescription::Type::enumeration ||
		   type == RPCRuntimeParameterDescription::Type::character;
}

void RPCRuntimeEncodedParam::encode(std::vector<unsigned char> &buffer) const {
	if (child_parameters.empty()) {
		buffer.insert(std::end(buffer), std::begin(data), std::end(data));
	} else {
		for (auto &child : child_parameters) {
			child.encode(buffer);
		}
	}
}

const RPCRuntimeParameterDescription *RPCRuntimeEncodedParam::get_description() const {
	return description;
}

void RPCRuntimeEncodedParam::set_value(int64_t value) {
	switch (description->get_type()) {
		case RPCRuntimeParameterDescription::Type::enumeration:
			set_enum_value(value);
			break;
		case RPCRuntimeParameterDescription::Type::integer:
			set_integer_value(value);
			break;
		case RPCRuntimeParameterDescription::Type::character:
			set_integer_value(value);
			break;
		case RPCRuntimeParameterDescription::Type::array:
            throw std::runtime_error("RPC: improper value type for this value: Given integer to array");
		case RPCRuntimeParameterDescription::Type::structure:
            throw std::runtime_error("RPC: improper value type for this value: Given integer to structure");
	}
}

void RPCRuntimeEncodedParam::set_value(const std::string &value) {
	switch (description->get_type()) {
		case RPCRuntimeParameterDescription::Type::enumeration:
			set_enum_value(value);
			break;
		case RPCRuntimeParameterDescription::Type::array:
			set_array_value(value);
			break;
		case RPCRuntimeParameterDescription::Type::integer:
            throw std::runtime_error("RPC: improper value type for this value: Given string to integer");
		case RPCRuntimeParameterDescription::Type::character:
            throw std::runtime_error("RPC: improper value type for this value: Given string to character");
		case RPCRuntimeParameterDescription::Type::structure:
            throw std::runtime_error("RPC: improper value type for this value: Given string to structure");
	}
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::get_parameter(int index) {
	switch (description->get_type()) {
		case RPCRuntimeParameterDescription::Type::array: {
			int elements = description->as_array().number_of_elements;
			if (index < 0 || index > elements) {
                throw std::runtime_error("RPC: invalid index for given array");
			}
			return child_parameters[index];
		} break;
		case RPCRuntimeParameterDescription::Type::structure:
            throw std::runtime_error("RPC: TODO");
			break;
		default:
            throw std::runtime_error("RPC: parameter does not have sub-parameters");
	}
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::get_parameter(const std::string &name) {
	if (description->get_type() != RPCRuntimeParameterDescription::Type::structure) {
        throw std::runtime_error("RPC: parameter does not have named sub-parameters");
	}
	for (auto &child : child_parameters) {
		if (child.get_description()->get_parameter_name() == name) {
			return child;
		}
	}
    throw std::runtime_error("RPC: invalid parameter name \"" + name + "\" for struct of type \"" + description->get_parameter_type() + "\"");
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::operator[](int index) {
	return get_parameter(index);
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::operator[](const std::string &name) {
	return get_parameter(name);
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::operator=(int64_t value) {
	set_value(value);
	return *this;
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::operator=(const std::string &name) {
	set_value(name);
	return *this;
}

RPCRuntimeEncodedParam &RPCRuntimeEncodedParam::operator=(std::initializer_list<int64_t> data) {
	int index = 0;
	if (child_parameters.size() < data.size()) {
        throw std::runtime_error("RPC: Gave " + std::to_string(data.size()) + " values to a parameter of type " + description->get_parameter_type());
	}
	for (auto &n : data) {
		child_parameters[index++] = n;
	}
	return *this;
}

void RPCRuntimeEncodedParam::set_integer_value(int64_t value) {
	data.clear();
	for (int index = 0; index < description->get_bit_size() / 8; index++) {
		data.push_back(value);
		value >>= 8;
	}
}

void RPCRuntimeEncodedParam::set_enum_value(int value) {
	set_integer_value(value);
}

void RPCRuntimeEncodedParam::set_enum_value(const std::string &value) {
	for (auto &e : description->as_enumeration().values) {
		if (e.name == value) {
			set_integer_value(e.to_int());
			return;
		}
	}
    throw std::runtime_error("RPC: invalid enum value " + value);
}

void RPCRuntimeEncodedParam::set_array_value(const std::string &array) {
	if (description->as_array().type.get_type() != RPCRuntimeParameterDescription::Type::character) {
        throw std::runtime_error("RPC: Assigning a string to an array is only supported for arrays of characters, but got " + description->get_parameter_type());
	}
	if (array.size() > child_parameters.size()) {
        throw std::runtime_error("RPC: Given string \"" + array + "\" of length " + std::to_string(array.size()) + " doesn't fit into " +
								 description->get_parameter_type());
	}
	for (unsigned int i = 0; i < array.size(); i++) {
		child_parameters[i].set_value(array[i]);
	}
	for (unsigned int i = array.size(); i < child_parameters.size(); i++) {
		child_parameters[i].set_value('\0');
	}
}
