#include "rpcruntime_decoder.h"
#include "rpcruntime_transfer.h"
#include "channel_codec/channel_codec.h"
#include "channel_codec/channel_codec_config.h"

#include <assert.h>
#include <QByteArray>

#include "rpc_transmission/client/generated_general/RPC_types.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"
#include "errorlogger/generic_eeprom_errorlogger.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//RPCRuntimeDecoder* sendingClassInstance = NULL;

static RPC_SIZE_RESULT get_transmission_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){
	int transferSize=0;

	assert(instance->aux.RPCRuntimeDecoderInstance);
	RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;

	const unsigned char *current = (const unsigned char *)buffer;
	RPC_SIZE_RESULT returnvalue;

	if (size_bytes == 0){
		returnvalue.result = RPC_COMMAND_INCOMPLETE;
		returnvalue.size = 1;
		return returnvalue;
	}

	RPCRuntimeTransfer decoder = sendingClass->decode(current, size_bytes);
	transferSize = decoder.get_min_number_of_missing_bytes();

	if (transferSize > 0){
		returnvalue.size = transferSize;
	}else{
		returnvalue.size = 0;
		returnvalue.result = RPC_COMMAND_UNKNOWN;
		return returnvalue;
	}

	returnvalue.result = returnvalue.size > size_bytes ? RPC_COMMAND_INCOMPLETE : RPC_SUCCESS;

	return returnvalue;
}

static void parse_transmission(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){

	assert(instance->aux.RPCRuntimeDecoderInstance);
	RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;

	QByteArray inBuffer = QByteArray((char*)buffer,(int)size_bytes);
	sendingClass->RPCDecodeRPCData(inBuffer);
}

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t errNum){
	//assert(sendingClassInstance);
	assert(instance->aux.RPCRuntimeDecoderInstance);
	RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;
	if (errNum == errlog_W_CHCODEC_RX_CRC_fail){
		sendingClass->setErrorCRCHappened(true);
	}else{
		sendingClass->setErrorChannelCodecHappened(true);
	}
}

EXTERNC RPC_RESULT phyPushDataBuffer(channel_codec_instance_t *instance, const char *buffer, size_t length){
	assert(instance->aux.RPCRuntimeDecoderInstance);

	QByteArray codecOutput = QByteArray(buffer,(int)length);

	instance->aux.RPCRuntimeDecoderInstance->setChannelCodecOutput(codecOutput);
	RPC_RESULT result = RPC_SUCCESS;

	return result;
}

EXTERNC void RPC_CHANNEL_CODEC_parser_init(channel_codec_instance_t *instance){
	(void)instance;

}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_answer_length(channel_codec_instance_t *instance, const void *buffer, size_t size){
	return  get_transmission_size(instance,buffer,  size);
}


EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_request_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){
	return  get_transmission_size(instance,buffer,  size_bytes);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_answer(channel_codec_instance_t *instance, const void *buffer, size_t size){
	parse_transmission(instance,buffer,size);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_request(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes ){
	parse_transmission(instance,buffer,size_bytes);
}
