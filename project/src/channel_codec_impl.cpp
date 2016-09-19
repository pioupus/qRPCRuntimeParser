#include "channel_codec/channel_codec.h"
#include "channel_codec/channel_codec_config.h"
#include "rpcruntime_decoder.h"
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
