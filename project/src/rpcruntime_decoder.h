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

	RPCRuntimeTransfer decode(const unsigned char *data, int size) const;

    RPCRuntimeTransfer decode(const std::vector<unsigned char> &data) const;

	RPCRunTimeProtocolDescription *get_description();
	const RPCRunTimeProtocolDescription *get_description() const;

	private:
	RPCRunTimeProtocolDescription *description;
};

#endif // RPCRUNTIMEDECODERESULT_H
