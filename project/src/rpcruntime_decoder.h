#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H

#include "rpcruntime_protocol_description.h"
#include "rpcruntime_transfer.h"

#include <functional>
#include <map>

/*
 * The RPCRuntimeDecoder decodes binary data into scrip-understandable Function calls and encodes the reply
 */
struct RPCCallbackHandle{
   const std::function<void (const RPCRuntimeDecodedFunctionCall &)> *callback_function;
   const RPCRuntimeFunction *rpc_function;
};

class RPCRuntimeDecoder {
	public:
	RPCRuntimeDecoder(RPCRunTimeProtocolDescription &description);
	template <std::size_t size>
	RPCRuntimeTransfer decode(const unsigned char (&data)[size]) {
		return decode(data, size);
	}

	RPCRuntimeTransfer decode(const unsigned char *data, int size) const;

    RPCRuntimeTransfer decode(const std::vector<unsigned char> &data) const;

	RPCRunTimeProtocolDescription *get_description();
	const RPCRunTimeProtocolDescription *get_description() const;
    void set_description(RPCRunTimeProtocolDescription &description);

    RPCCallbackHandle set_reply_callback(const RPCRuntimeFunction &rpc_function, std::function<void(const RPCRuntimeDecodedFunctionCall &)> callback_function);
    void remove_reply_callback(const RPCRuntimeFunction &rpc_function, const std::function<void (const RPCRuntimeDecodedFunctionCall &)> *callback_function);
    bool has_callbacks();


private:
	RPCRunTimeProtocolDescription *description = nullptr;
	friend class RPCRuntimeTransfer;
	std::map<const RPCRuntimeFunction *, std::vector<std::function<void(const RPCRuntimeDecodedFunctionCall &)>>> callbacks;
};

#endif // RPCRUNTIMEDECODERESULT_H
