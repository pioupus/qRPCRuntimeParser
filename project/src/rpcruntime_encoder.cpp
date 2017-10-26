#include "rpcruntime_encoder.h"
#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_protocol_description.h"
#include <string>

RPCRuntimeEncoder::RPCRuntimeEncoder(const RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeEncodedFunctionCall RPCRuntimeEncoder::encode(int id) const {
	for (auto &function : description->get_functions()) {
		if (function.get_request_id() == id) {
            return RPCRuntimeEncodedFunctionCall{function};
		}
	}
    throw std::runtime_error("RPC: unknown function id "+std::to_string(id));
}

RPCRuntimeEncodedFunctionCall RPCRuntimeEncoder::encode(const std::string &function_name) const {
	for (auto &function : description->get_functions()) {
		if (function.get_function_name() == function_name) {
            return RPCRuntimeEncodedFunctionCall{function};
		}
	}
    throw std::runtime_error("RPC: unknown function name: " + function_name);
}

bool RPCRuntimeEncoder::function_exists_for_encoding(const std::string &function_name) const {
    for (auto &function : description->get_functions()) {
        if (function.get_function_name() == function_name) {
            return true;
        }
    }
    return false;
}

void RPCRuntimeEncoder::set_description(const RPCRunTimeProtocolDescription &description) {
	this->description = &description;
}
