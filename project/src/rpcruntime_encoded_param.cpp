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

void RPCRuntimeEncodedParam::set_integer_value(int64_t i) {
	data.clear();
	if (description->as_integer().is_signed) {
		for (int index = 0; index < description->get_bit_size() / 8; index++){
			data.push_back(i);
			i >>= 8;
		}
	}
	else{
		uint64_t u = i;
		for (int index = 0; index < description->get_bit_size() / 8; index++){
			data.push_back(u);
			u >>= 8;
		}
	}
}
