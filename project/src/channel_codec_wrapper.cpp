#include "channel_codec_wrapper.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"

Channel_codec_wrapper::Channel_codec_wrapper(const RPCRuntimeDecoder &decoder)
	: decoder(&decoder) {
	transfers.emplace_back(*decoder.get_description());
}

void Channel_codec_wrapper::add_data(const std::vector<unsigned char> &buffer) {
	//TODO: add channel codec here, so it decodes the data before we feed it to the RPCRuntimeTransfer
	for (auto &byte : buffer) {
		auto &t = transfers.back();
		t.add_data(byte);
		if (t.is_complete()) {
			transfers.emplace_back(*decoder->get_description());
		}
	}
}

bool Channel_codec_wrapper::transfer_complete() const {
	return transfers.empty() == false;
}

RPCRuntimeDecodedFunctionCall Channel_codec_wrapper::pop() {
	if (transfers.front().is_complete() == false) {
		throw std::runtime_error("Tried to pop from an incomplete transfer");
	}
	auto transfer = std::move(transfers.front());
	transfers.pop_front();
	return transfer.decode();
}
