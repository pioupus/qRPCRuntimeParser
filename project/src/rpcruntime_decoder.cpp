#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description)
	: description(&description) {}

RPCRuntimeTransfer RPCRuntimeDecoder::decode(const unsigned char *data, int size) {
	RPCRuntimeTransfer retval(*description);
	retval.add_data(data, data + size);
	return retval;
}
