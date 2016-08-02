#ifndef PHYLAYER_H_
#define PHYLAYER_H_



#include "channel_codec/channel_codec_config.h"

RPC_RESULT phyPushDataBuffer(channel_codec_instance_index_t instance_index, const char *buffer, size_t length);

#endif
