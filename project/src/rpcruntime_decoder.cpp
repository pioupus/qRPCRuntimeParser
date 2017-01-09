#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"
#include <algorithm>

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const unsigned char *data, int size) const {
	RPCRuntimeTransfer retval(*description, *this);
	retval.add_data(data, data + size);
    return retval;
}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const std::vector<unsigned char> &data) const
{
    return decode(data.data(), data.size());
}

RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() {
	return description;
}

const RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() const {
	return description;
}

RPCCallbackHandle RPCRuntimeDecoder::set_reply_callback(const RPCRuntimeFunction &rpc_function,
										   std::function<void(const RPCRuntimeDecodedFunctionCall &)> callback_function) {
	//unfortunately we have to get the non-const rpc_function from the const rpc_function... one could use const_cast, but that is dirty

    callbacks[&rpc_function].emplace_back(std::move(callback_function));
    RPCCallbackHandle retval = {&callbacks[&rpc_function].back(), &rpc_function};

    return retval;
}

void RPCRuntimeDecoder::remove_reply_callback(const RPCRuntimeFunction &rpc_function,
                                          const std::function<void (const RPCRuntimeDecodedFunctionCall &)> *callback_function) {
    //unfortunately we have to get the non-const rpc_function from the const rpc_function... one could use const_cast, but that is dirty

    auto &vec = callbacks[&rpc_function];
    auto it = std::find_if(vec.begin(), vec.end(), [callback_function](std::function<void (const RPCRuntimeDecodedFunctionCall &)> &other)-> bool{
                               return callback_function == &other;
                           })  ;
    vec.erase(it);
    if (vec.size() == 0){
        callbacks.erase(&rpc_function);
    }
}

void RPCRuntimeDecoder::set_description(RPCRunTimeProtocolDescription &description) {
    this->description = &description;
}

bool RPCRuntimeDecoder::has_callbacks()
{
    return callbacks.size() > 0;
}
