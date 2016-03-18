/*
 * channel_codec.h
 *
 *  Created on: 30.03.2015
 *      Author: ak
 */

#ifndef CHANNELCODEC_CHANNELCODEC_H_
#define CHANNELCODEC_CHANNELCODEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rpc_transmission/client/generated_general/RPC_TRANSMISSION_types.h"

#define CHANNEL_BLOCKLENGTH 16

void channel_init(void);
void channel_start_message_from_RPC(size_t size);
void channel_push_byte_from_RPC(unsigned char byte);
RPC_TRANSMISSION_RESULT channel_commit_from_RPC(void);

#ifndef CHANNEL_CODEC_RUNS_IN_CPP_CLASS_ENVIRONMENT
void channel_push_byte_to_RPC(unsigned char byte);
#else
void channel_push_byte_to_RPC(unsigned char byte, void* RPC_SENDER_CLASS_INSTANCE);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CHANNELCODEC_CHANNELCODEC_H_ */
