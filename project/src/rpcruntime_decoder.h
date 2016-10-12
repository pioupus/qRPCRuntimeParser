#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H

#include "rpcruntime_protocol_description.h"
#include "rpcruntime_transfer.h"

#include <functional>

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

	RPCRunTimeProtocolDescription *get_description();
	const RPCRunTimeProtocolDescription *get_description() const;
	void set_reply_callback(const RPCRuntimeFunction &parameter, std::function<void(const RPCRuntimeDecodedFunctionCall &)> callback_function);

	private:
	RPCRunTimeProtocolDescription *description;
};

#endif // RPCRUNTIMEDECODERESULT_H
