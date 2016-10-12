#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const unsigned char *data, int size) const {
	RPCRuntimeTransfer retval(*description);
	retval.add_data(data, data + size);
	return retval;
}

RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description()
{
	return description;
}

const RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() const
{
	return description;
}

void RPCRuntimeDecoder::set_reply_callback(const RPCRuntimeFunction &parameter, std::function<void (const RPCRuntimeDecodedFunctionCall &)> callback_function)
{
	(void)parameter;
	(void)callback_function;
}
