#include "rpcruntime_encoded_param.h"

#include <cassert>

RPCRuntimeEncodedParam::RPCRuntimeEncodedParam(const RPCRuntimeParameterDescription &parameter_description)
	: description(&parameter_description) {}

bool RPCRuntimeEncodedParam::are_all_values_set() const {
	return description->get_bit_size() / 8 == static_cast<int>(data.size());
}

void RPCRuntimeEncodedParam::encode(std::vector<unsigned char> &buffer) const {
	buffer.insert(std::end(buffer), std::begin(data), std::end(data));
}

const RPCRuntimeParameterDescription *RPCRuntimeEncodedParam::get_description() const {
	return description;
}

void RPCRuntimeEncodedParam::set_integer_value(int64_t value) {
	data.clear();
	for (int index = 0; index < description->get_bit_size() / 8; index++){
		data.push_back(value);
		value >>= 8;
	}
}

void RPCRuntimeEncodedParam::set_enum_value(int value)
{
	set_integer_value(value);
}

void RPCRuntimeEncodedParam::set_enum_value(const std::string &value)
{
	for (auto &e : description->as_enumeration().values){
		if (e.name == value){
			set_integer_value(e.to_int());
			return;
		}
	}
	throw std::runtime_error("invalid enum value " + value);
}
