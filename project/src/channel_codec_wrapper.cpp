#include "channel_codec_wrapper.h"
#include "channel_codec/channel_codec.h"
#include "errorlogger/generic_eeprom_errorlogger.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_transfer.h"

#include <QDebug>
#include <iterator>
#include <map>

using Wrapper_instances = std::map<channel_codec_instance_t *, Channel_codec_wrapper *>;

namespace Wrapper_instance {
	//Workaround to avoid static initialization order fiasco
	auto &get() {
		static Wrapper_instances wrapper_instances;
		return wrapper_instances;
	}
	static auto &wrapper_instances = get();
}

Channel_codec_wrapper::Channel_codec_wrapper(const RPCRuntimeDecoder &decoder)
	: decoder(&decoder)
	, cci(std::make_unique<channel_codec_instance_t>())
	, log("log.dat", std::ios::binary) {
	transfers.emplace_back(*decoder.get_description(), decoder);
	channel_init_instance(cci.get(), input_buffer, sizeof input_buffer, output_buffer, sizeof output_buffer);
	Wrapper_instance::get()[cci.get()] = this;
}

Channel_codec_wrapper::~Channel_codec_wrapper() {
	if (cci != nullptr) {
		channel_uninit_instance(cci.get());
	}
}

void Channel_codec_wrapper::add_data(const std::vector<unsigned char> &buffer) {
	for (auto &c : buffer) {
		log.put(c);
		channel_push_byte_to_RPC(cci.get(), c);
	}
}

void Channel_codec_wrapper::add_data(const unsigned char *data, std::size_t size) {
	add_data({data, data + size});
}

bool Channel_codec_wrapper::transfer_complete() const {
	return transfers.front().is_complete();
}

RPCRuntimeDecodedFunctionCall Channel_codec_wrapper::pop() {
	return pop_completed_transfer().decode();
}

RPCRuntimeTransfer Channel_codec_wrapper::pop_completed_transfer() {
	if (transfers.front().is_complete() == false) {
        throw std::runtime_error("RPC channelcodec: Tried to pop from an incomplete transfer");
	}
	auto transfer = std::move(transfers.front());
	transfers.pop_front();
	return transfer;
}

const RPCRuntimeDecoder *Channel_codec_wrapper::get_decoder() const {
	return decoder;
}

void Channel_codec_wrapper::reset_current_transfer() {
	transfers.back().reset();
}

std::vector<unsigned char> Channel_codec_wrapper::encode(const RPCRuntimeEncodedFunctionCall &call) {
	encoded_data.clear();
	const auto &data = call.encode();
	channel_start_message_from_RPC(cci.get(), data.size());
	for (auto &byte : data) {
		channel_push_byte_from_RPC(cci.get(), byte);
	}
	channel_commit_from_RPC(cci.get());
	return std::move(encoded_data);
}

const channel_codec_instance_t *Channel_codec_wrapper::debug_get_instance() const {
	return cci.get();
}

const RPCRuntimeTransfer &Channel_codec_wrapper::current_transfer() const {
	return transfers.front();
}

const std::vector<unsigned char> &Channel_codec_wrapper::get_encoded_data() const {
    return encoded_data;
}

channel_codec_instance_t *Channel_codec_wrapper::get_cci()
{
    return cci.get();
}

void push_data(Channel_codec_wrapper &ccw, const unsigned char *data, std::size_t size) {
	//qDebug() << "ChannelCodec received" << size << "bytes";
	for (std::size_t i = 0; i < size; i++) {
		auto &byte = data[i];
		auto &t = ccw.transfers.back();
		t.add_data(byte);
		if (t.is_complete()) {
			//qDebug() << "ChannelCodecWrapper Transfer complete";
			ccw.transfers.emplace_back(*ccw.decoder->get_description(), *ccw.decoder);
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
	auto transfer = Wrapper_instance::wrapper_instances[instance]->get_decoder()->decode(static_cast<const unsigned char *>(buffer), size);
	retval.result = transfer.is_complete() ? RPC_SUCCESS : RPC_COMMAND_INCOMPLETE;
	retval.size = transfer.get_min_number_of_bytes();
	return retval;
}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_request_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	RPC_SIZE_RESULT retval;
	auto transfer = Wrapper_instance::wrapper_instances[instance]->get_decoder()->decode(static_cast<const unsigned char *>(buffer), size_bytes);
	retval.result = transfer.is_complete() ? RPC_SUCCESS : RPC_COMMAND_INCOMPLETE;
	retval.size = transfer.get_min_number_of_bytes();
	return retval;
}

EXTERNC void RPC_CHANNEL_CODEC_parse_answer(channel_codec_instance_t *instance, const void *buffer, size_t size) {
	push_data(*Wrapper_instance::wrapper_instances[instance], static_cast<const unsigned char *>(buffer), size);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_request(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	push_data(*Wrapper_instance::wrapper_instances[instance], static_cast<const unsigned char *>(buffer), size_bytes);
}

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t errNum) {
	Wrapper_instance::wrapper_instances[instance]->reset_current_transfer();
	(void)errNum;
}

void set_output(Channel_codec_wrapper &ccw, const unsigned char *data, std::size_t size) {
	std::copy(data, data + size, std::back_inserter(ccw.encoded_data));
}

EXTERNC RPC_RESULT phyPushDataBuffer(channel_codec_instance_t *instance, const char *buffer, size_t length) {
	set_output(*Wrapper_instance::wrapper_instances[instance], reinterpret_cast<const unsigned char *>(buffer), length);
	return RPC_SUCCESS;
}

#undef EXTERNC

std::ostream &operator<<(std::ostream &os, channel_codec_channel_state_t state) {
	switch (state) {
		case csNone:
			return os << "None";
		case csFoundPreamble:
			return os << "Found Preamble";
		case csLoadingPayload:
			return os << "Loading Payload";
		case csPayloadComplete:
			return os << "Payload Complete";
		case csCRCAndPackageComplete:
			return os << "CRC and Package Complete";
	}
	return os << "Invalid";
}
