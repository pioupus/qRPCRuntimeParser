#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H

#include "rpcruntime_protocol_description.h"
#include "rpcruntime_transfer.h"
//class RPCRuntimeTransfer;

/*
 * The RPCRuntimeDecoder decodes binary data into scrip-understandable Function calls and encodes the reply
 */

struct RPCRuntimeDecoder {
	RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description);
	template<std::size_t size>
	RPCRuntimeTransfer decode(const unsigned char (&data)[size]){
		return decode(data, size);
	}

	RPCRuntimeTransfer decode(const unsigned char *data, int size);

	private:
	RPCRunTimeProtocolDescription *description;
};

#endif // RPCRUNTIMEDECODERESULT_H
