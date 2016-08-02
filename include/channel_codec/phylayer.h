#ifndef PHYLAYER_H_
#define PHYLAYER_H_

#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"
#include "rpc_transmission/client/generated_general/RPC_TRANSMISSION_types.h"

#include "channel_codec/channel_codec_config.h"

RPC_TRANSMISSION_RESULT phyPushDataBuffer(channel_codec_instance_index_t instance_index, const char *buffer, size_t length);

#endif
