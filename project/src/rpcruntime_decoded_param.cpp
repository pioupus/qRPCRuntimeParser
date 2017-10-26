#include "rpcruntime_decoded_param.h"
#include "rpcruntime_paramter_description.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>

template <class Container>
struct Reverse_container {
	Reverse_container(Container &container)
		: container(container) {}
	auto begin() {
		return container.rbegin();
	}

	auto end() {
		return container.rend();
	}

	private:
	Container &container;
};

template <class Container>
Reverse_container<Container> reverse_view(Container &container) {
	return {container};
}

RPCRuntimeDecodedParam::RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &parameter_description, std::string field_id)
    : parameter_description(&parameter_description) {
    this->field_id = field_id;
}


template <class T>
T parse_signed_int(const std::vector<unsigned char> &data) {
	T retval = 0;
	for (auto &chr : reverse_view(data)) {
		retval <<= 8;
		retval |= chr;
	}
	return retval;
}

template <>
uint8_t parse_signed_int<uint8_t>(const std::vector<unsigned char> &data) {
	return data[0];
}

uint64_t RPCRuntimeDecodedParam::as_unsigned_integer() const {
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::integer);
	assert(parameter_description->as_integer().is_signed == false);
	return parse_signed_int<uint64_t>(data);
}

int64_t RPCRuntimeDecodedParam::as_signed_integer() const {
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::integer);
	assert(parameter_description->as_integer().is_signed == true);
	switch (data.size()) {
		case 1:
			return parse_signed_int<int8_t>(data);
		case 2:
			return parse_signed_int<int16_t>(data);
		case 4:
			return parse_signed_int<int32_t>(data);
		case 8:
			return parse_signed_int<int64_t>(data);
	}
    throw std::domain_error("RPC: Invalid amount of data to parse signed integer");
}

int64_t RPCRuntimeDecodedParam::as_integer() const
{
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::integer);
	if (parameter_description->as_integer().is_signed == true){
		return as_signed_integer();
	}
	assert(parameter_description->get_bit_size() < 64); //make sure we don't have a uint64_t, because we cannot represent it fully in an int64_t
	return as_unsigned_integer();
}

Decoded_enum RPCRuntimeDecodedParam::as_enum() const
{
	int value = parse_signed_int<int>(data);
	std::string name;
	for (auto &enum_value : parameter_description->as_enumeration().values){
		if (enum_value.is_int()){
			if (enum_value.to_int() == value){
				name = enum_value.name;
				break;
			}
		}
	}
	return {name, value};
}

std::vector<Decoded_struct> RPCRuntimeDecodedParam::as_struct() const
{
	std::vector<Decoded_struct> retval;
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::structure);
	int current_data_position = 0;
	for (auto &member : parameter_description->as_structure().members){
        retval.push_back({member.get_parameter_name(), {member,field_id+"."+member.get_parameter_name()}});
		auto data_size = member.get_bit_size() / 8;
		retval.back().type.set_data(data.data() + current_data_position, data_size);
		current_data_position += data_size;
	}
	assert(current_data_position == static_cast<int>(data.size()));
	return retval;
}

std::vector<RPCRuntimeDecodedParam> RPCRuntimeDecodedParam::as_array() const
{
	std::vector<RPCRuntimeDecodedParam> retval;
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::array);
	for (int i = 0; i < parameter_description->as_array().number_of_elements; i++){
        retval.emplace_back(parameter_description->as_array().type, field_id+"."+std::to_string(i));
		retval.back().set_data(data.data() + i * parameter_description->as_array().type.get_bit_size() / 8, parameter_description->as_array().type.get_bit_size() / 8);
	}
	return retval;
}



std::string RPCRuntimeDecodedParam::as_full_string() const {
	std::string retval;
	retval.resize(data.size());
	std::copy(std::begin(data), std::end(data), std::begin(retval));
	return retval;
}

std::string RPCRuntimeDecodedParam::as_string() const {
	std::string retval;
	retval.reserve(data.size());
	for (auto &c : data) {
		if (c == '\0') {
			break;
		}
		retval.push_back(c);
	}
    return retval;
}

std::string RPCRuntimeDecodedParam::get_field_id() const
{
    return field_id;
}

const RPCRuntimeParameterDescription *RPCRuntimeDecodedParam::get_desciption() const {
	return parameter_description;
}

void RPCRuntimeDecodedParam::set_data(std::vector<unsigned char> data) {
	this->data = std::move(data);
}

void RPCRuntimeDecodedParam::set_data(const unsigned char *begin, int size)
{
	data.resize(size);
	std::copy(begin, begin + size, std::begin(data));
}

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param) {
	const auto byte_size = param.get_desciption()->get_bit_size() / 8;
    std::vector<unsigned char> data;
	data.resize(byte_size);
	is.read(reinterpret_cast<char *>(data.data()), byte_size);
	if (is) {
		param.set_data(std::move(data));
	}
	return is;
}
