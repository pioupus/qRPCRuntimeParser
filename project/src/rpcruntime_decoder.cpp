#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const unsigned char *data, int size) const {
	RPCRuntimeTransfer retval(*description);
	retval.add_data(data, data + size);
    return retval;
}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const std::vector<unsigned char> &data) const
{
    return decode(data.data(), data.size());
}

RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description()
{
	return description;
}

const RPCRunTimeProtocolDescription *RPCRuntimeDecoder::get_description() const
{
	return description;
}
