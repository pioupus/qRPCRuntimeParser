#include "channel_codec_wrapper.h"
#include "channel_codec/channel_codec.h"
#include "errorlogger/generic_eeprom_errorlogger.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"

#include <map>

std::map<channel_codec_instance_t *, Channel_codec_wrapper *> wrapper_instances;

Channel_codec_wrapper::Channel_codec_wrapper(const RPCRuntimeDecoder &decoder)
	: decoder(&decoder)
	, cci(std::make_unique<channel_codec_instance_t>()) {
	transfers.emplace_back(*decoder.get_description());
	channel_init_instance(cci.get(), input_buffer, sizeof input_buffer, output_buffer, sizeof output_buffer);
	wrapper_instances[cci.get()] = this;
}

Channel_codec_wrapper::~Channel_codec_wrapper() {
	channel_uninit_instance(cci.get());
}

void Channel_codec_wrapper::add_data(const std::vector<unsigned char> &buffer) {
	//TODO: add channel codec here, so it decodes the data before we feed it to the RPCRuntimeTransfer

	for (auto &c : buffer) {
		channel_push_byte_to_RPC(cci.get(), c);
	}
}

bool Channel_codec_wrapper::transfer_complete() const {
	return transfers.front().is_complete();
}

RPCRuntimeDecodedFunctionCall Channel_codec_wrapper::pop() {
	if (transfers.front().is_complete() == false) {
		throw std::runtime_error("Tried to pop from an incomplete transfer");
	}
	auto transfer = std::move(transfers.front());
	transfers.pop_front();
	return transfer.decode();
}

const RPCRuntimeDecoder *Channel_codec_wrapper::get_decoder() const {
	return decoder;
}

void Channel_codec_wrapper::reset_current_transfer()
{
	transfers.back().reset();
}

void push_data(Channel_codec_wrapper &ccw, const unsigned char *data, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		auto &byte = data[i];
		auto &t = ccw.transfers.back();
		t.add_data(byte);
		if (t.is_complete()) {
			ccw.transfers.emplace_back(*ccw.decoder->get_description());
		}
	}
}

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void RPC_CHANNEL_CODEC_parser_init(channel_codec_instance_t *instance) {
	(void)instance;
}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_answer_length(channel_codec_instance_t *instance, const void *buffer, size_t size) {
	RPC_SIZE_RESULT retval;
	auto transfer = wrapper_instances[instance]->get_decoder()->decode(static_cast<const unsigned char *>(buffer), size);
	retval.result = transfer.is_complete() ? RPC_SUCCESS : RPC_COMMAND_INCOMPLETE;
	retval.size = transfer.get_min_number_of_bytes();
	return retval;
}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_request_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	RPC_SIZE_RESULT retval;
	auto transfer = wrapper_instances[instance]->get_decoder()->decode(static_cast<const unsigned char *>(buffer), size_bytes);
	retval.result = transfer.is_complete() ? RPC_SUCCESS : RPC_COMMAND_INCOMPLETE;
	retval.size = transfer.get_min_number_of_bytes();
	return retval;
}

EXTERNC void RPC_CHANNEL_CODEC_parse_answer(channel_codec_instance_t *instance, const void *buffer, size_t size) {
	push_data(*wrapper_instances[instance], static_cast<const unsigned char *>(buffer), size);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_request(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	push_data(*wrapper_instances[instance], static_cast<const unsigned char *>(buffer), size_bytes);
}

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t errNum) {
	wrapper_instances[instance]->reset_current_transfer();
	(void)errNum;
}

#undef EXTERNC
