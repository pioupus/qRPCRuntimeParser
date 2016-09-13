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

RPCRuntimeDecodedParam::RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &parameter_description)
	: parameter_description(&parameter_description) {}

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
	throw std::domain_error("Invalid amount of data to parse signed integer");
}

std::vector<Decoded_struct> RPCRuntimeDecodedParam::as_struct() const
{
	std::vector<Decoded_struct> retval;
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::structure);
	//TODO: actually fill retval
	return retval;
}

std::vector<RPCRuntimeDecodedParam> RPCRuntimeDecodedParam::as_array() const
{
	std::vector<RPCRuntimeDecodedParam> retval;
	assert(parameter_description->get_type() == RPCRuntimeParameterDescription::Type::array);
	//TODO: actually fill retval
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

const RPCRuntimeParameterDescription *RPCRuntimeDecodedParam::get_desciption() const {
	return parameter_description;
}

void RPCRuntimeDecodedParam::set_data(std::vector<unsigned char> data) {
	this->data = std::move(data);
}

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param) {
	auto byte_size = param.get_desciption()->get_bit_size() / 8;
	std::vector<unsigned char> data;
	data.resize(byte_size);
	is.read(reinterpret_cast<char *>(data.data()), byte_size);
	if (is) {
		param.set_data(std::move(data));
	}
	return is;
}
