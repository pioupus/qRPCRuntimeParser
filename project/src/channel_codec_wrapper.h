#ifndef CHANNEL_CODEC_WRAPPER_H
#define CHANNEL_CODEC_WRAPPER_H

class RPCRuntimeDecoder;
class RPCRuntimeDecodedFunctionCall;

#include "rpcruntime_transfer.h"

#include <cinttypes>
#include <deque>

class Channel_codec_wrapper {
	public:
	Channel_codec_wrapper(const RPCRuntimeDecoder &decoder);
	static std::vector<unsigned char> encode(const std::vector<unsigned char> &buffer);
	void add_data(const std::vector<unsigned char> &buffer);
	template <std::size_t size>
	void add_data(const uint8_t (&data)[size]) {
		add_data({data, data + size});
	}

	bool transfer_complete() const;
	RPCRuntimeDecodedFunctionCall pop();

	private:
	const RPCRuntimeDecoder *decoder;
	std::deque<RPCRuntimeTransfer> transfers;
};

#endif //CHANNEL_CODEC_WRAPPER_H
