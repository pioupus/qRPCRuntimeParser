#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const unsigned char *data, int size) const {
	RPCRuntimeTransfer retval(*description, *this);
	retval.add_data(data, data + size);
	return retval;
}

RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() {
	return description;
}

const RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() const {
	return description;
}

void RPCRuntimeDecoder::set_reply_callback(const RPCRuntimeFunction &rpc_function,
										   std::function<void(const RPCRuntimeDecodedFunctionCall &)> callback_function) {
	//unfortunately we have to get the non-const rpc_function from the const rpc_function... one could use const_cast, but that is dirty
	callbacks[&rpc_function].emplace_back(std::move(callback_function));
}

void RPCRuntimeDecoder::set_description(RPCRunTimeProtocolDescription &description) {
	this->description = &description;
}
