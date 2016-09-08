#include "rpcruntime_decoder.h"
#include "channel_codec/channel_codec.h"
#include "channel_codec/channel_codec_config.h"
#include "rpcruntime_transfer.h"

#include <assert.h>

#include "errorlogger/generic_eeprom_errorlogger.h"
#include "rpc_transmission/client/generated_general/RPC_types.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//returns if the given data is a complete message
static RPC_SIZE_RESULT get_transmission_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {

	assert(instance->aux.RPCRuntimeDecoderInstance);
	RPCRuntimeDecoder *sendingClass = instance->aux.RPCRuntimeDecoderInstance;

	const unsigned char *current = (const unsigned char *)buffer;
	RPC_SIZE_RESULT returnvalue;

	if (size_bytes == 0) {
		returnvalue.result = RPC_COMMAND_INCOMPLETE;
		returnvalue.size = 1;
		return returnvalue;
	}

	RPCRuntimeTransfer decoder = sendingClass->decode(current, size_bytes);
	int transferSize = decoder.get_min_number_of_bytes();

	if (transferSize > 0) {
		returnvalue.size = transferSize;
	} else {
		returnvalue.size = 0;
		returnvalue.result = RPC_COMMAND_UNKNOWN;
		return returnvalue;
	}

	returnvalue.result = returnvalue.size > size_bytes ? RPC_COMMAND_INCOMPLETE : RPC_SUCCESS;

	return returnvalue;
}

//parse_transmission only gets called when we have a complete message to parse
static void parse_transmission(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	assert(instance->aux.RPCRuntimeDecoderInstance);
	RPCRuntimeDecoder *sendingClass = instance->aux.RPCRuntimeDecoderInstance;

	RPCRuntimeTransfer decoder = sendingClass->decode(static_cast<const unsigned char *>(buffer), size_bytes);
	assert(decoder.get_min_number_of_bytes() == static_cast<int>(size_bytes)); //message must be complete or this function cannot be called
}

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t errNum) {
	//assert(sendingClassInstance);
	assert(instance->aux.RPCRuntimeDecoderInstance);
	(void)errNum;
#if 0
	RPCRuntimeDecoder *sendingClass = instance->aux.RPCRuntimeDecoderInstance;
	if (errNum == errlog_W_CHCODEC_RX_CRC_fail) {
		sendingClass->setErrorCRCHappened(true);
	} else {
		sendingClass->setErrorChannelCodecHappened(true);
	}
#endif
}

EXTERNC RPC_RESULT phyPushDataBuffer(channel_codec_instance_t *instance, const char *buffer, size_t length) {
	assert(instance->aux.RPCRuntimeDecoderInstance);
	(void)buffer;
	(void)length;
#if 0
	QByteArray codecOutput = QByteArray(buffer, (int)length);

	instance->aux.RPCRuntimeDecoderInstance->setChannelCodecOutput(codecOutput);
#endif
	RPC_RESULT result = RPC_SUCCESS;

	return result;
}

EXTERNC void RPC_CHANNEL_CODEC_parser_init(channel_codec_instance_t *instance) {
	(void)instance;
}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_answer_length(channel_codec_instance_t *instance, const void *buffer, size_t size) {
	return get_transmission_size(instance, buffer, size);
}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_request_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	return get_transmission_size(instance, buffer, size_bytes);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_answer(channel_codec_instance_t *instance, const void *buffer, size_t size) {
	parse_transmission(instance, buffer, size);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_request(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes) {
	parse_transmission(instance, buffer, size_bytes);
}
