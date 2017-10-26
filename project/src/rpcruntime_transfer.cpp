#include "rpcruntime_transfer.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_protocol_description.h"

#include <algorithm>
#include <cassert>
#include <sstream>

RPCRuntimeTransfer::RPCRuntimeTransfer(const RPCRunTimeProtocolDescription &protocol, const RPCRuntimeDecoder &decoder)
	: protocol(&protocol)
	, decoder(&decoder) {}

int RPCRuntimeTransfer::get_min_number_of_bytes() const {
	if (data.empty()) {
		return 1; //need at least 1 byte for the ID
	}
	//TODO: check in protocol how long function with ID data[0] is
	return protocol->get_parameter_size_bytes(data[0]);
}

bool RPCRuntimeTransfer::is_complete() const {
	return get_min_number_of_bytes() == static_cast<int>(data.size());
}

void RPCRuntimeTransfer::add_data(unsigned char byte) {
	data.push_back(byte);
}

RPCRuntimeDecodedFunctionCall RPCRuntimeTransfer::decode() const {
	std::vector<RPCRuntimeDecodedParam> decoded_parameters;
	std::stringstream ss;
	ss.str({reinterpret_cast<const char *>(data.data()), data.size()});
	unsigned char id;
    ss >> std::noskipws >> id;
	ss.exceptions(std::stringstream::badbit | std::stringstream::failbit | std::stringstream::eofbit);
	if (!ss) {
        throw std::runtime_error("RPC: No available data to decode");
	}

	auto &function = protocol->get_function(id);
	auto &parameter_descriptions = protocol->get_parameters(id);
	for (auto &pd : parameter_descriptions) {
		decoded_parameters.emplace_back(pd, std::to_string(id) + "." + pd.get_parameter_name());
		ss >> decoded_parameters.back();
	}

	assert(ss);
	RPCRuntimeDecodedFunctionCall retval{id, std::move(decoded_parameters), function};
	auto callbacks_it = decoder->callbacks.find(&function);
	if (callbacks_it != std::end(decoder->callbacks)) {
		for (auto &callback : callbacks_it->second) {
			callback(retval);
		}
	}
	return retval;
}

void RPCRuntimeTransfer::reset() {
	data.clear();
}

const std::vector<unsigned char> &RPCRuntimeTransfer::get_raw_data() const {
	return data;
}
