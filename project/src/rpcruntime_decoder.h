#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H

#include "rpcruntime_protocol_description.h"

class RPCRuntimeTransfer;

/*
 * The RPCRuntimeDecoder decodes binary data into scrip-understandable Function calls and encodes the reply
 */

struct RPCRuntimeDecoder {
	RPCRuntimeTransfer decode(const unsigned char *data, int size);

	private:
	RPCRunTimeProtocolDescription description;
};

#endif // RPCRUNTIMEDECODERESULT_H
