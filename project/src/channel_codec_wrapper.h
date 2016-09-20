#ifndef CHANNEL_CODEC_WRAPPER_H
#define CHANNEL_CODEC_WRAPPER_H

class RPCRuntimeDecoder;
class RPCRuntimeDecodedFunctionCall;
class RPCRuntimeEncodedFunctionCall;

struct channel_codec_instance_t;

#include "rpcruntime_transfer.h"

#include <cinttypes>
#include <deque>
#include <memory>
#include <vector>

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

class Channel_codec_wrapper {
	public:
	Channel_codec_wrapper(const RPCRuntimeDecoder &decoder);
	Channel_codec_wrapper(const Channel_codec_wrapper &) = delete;
	~Channel_codec_wrapper();
	void add_data(const std::vector<unsigned char> &buffer);
	template <std::size_t size>
	void add_data(const unsigned char (&data)[size]) {
		add_data({data, data + size});
	}
	void add_data(const unsigned char *data, std::size_t size);

	bool transfer_complete() const;
	RPCRuntimeDecodedFunctionCall pop();
	const RPCRuntimeDecoder *get_decoder() const;
	void reset_current_transfer();
	std::vector<unsigned char> encode(const RPCRuntimeEncodedFunctionCall &call);

	private:
	const RPCRuntimeDecoder *decoder;
	std::deque<RPCRuntimeTransfer> transfers;
	std::unique_ptr<channel_codec_instance_t> cci;
	char input_buffer[1024];
	char output_buffer[1024];
	std::vector<unsigned char> encoded_data;
	friend void push_data(Channel_codec_wrapper &ccw, const unsigned char *data, std::size_t size);
	friend void set_output(Channel_codec_wrapper &ccw, const unsigned char *data, std::size_t size);
};

#undef EXTERNC

#endif //CHANNEL_CODEC_WRAPPER_H
