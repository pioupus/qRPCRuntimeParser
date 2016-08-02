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
#include "channel_codec/channel_codec_config.h"
#define CHANNEL_BLOCKLENGTH 16


typedef void (*RPC_parse_request_t)(const void *buffer, size_t size);
typedef void (*RPC_parse_answer_t)(const void *buffer, size_t size);
typedef RPC_SIZE_RESULT (*RPC_get_request_size_t)(const void *buffer, size_t size_bytes);
typedef RPC_SIZE_RESULT (*RPC_get_answer_length_t)(const void *buffer, size_t size_bytes);
typedef void (*RPC_Parser_init_t)(void);


void channel_init(void);
void channel_init_instance(channel_codec_instance_index_t instance_index,
		RPC_parse_request_t RPC_parse_request,
		RPC_parse_answer_t RPC_parse_answer,
		RPC_get_request_size_t RPC_get_request_size,
		RPC_get_answer_length_t RPC_get_answer_length,
		RPC_Parser_init_t RPC_Parser_init);

void channel_start_message_from_RPC(channel_codec_instance_index_t instance_index, size_t size);
void channel_push_byte_from_RPC(channel_codec_instance_index_t instance_index, unsigned char byte);
RPC_RESULT channel_commit_from_RPC(channel_codec_instance_index_t instance_index );

void channel_push_byte_to_RPC(channel_codec_instance_index_t instance_index, unsigned char byte);


#ifdef __cplusplus
}
#endif

#endif /* CHANNELCODEC_CHANNELCODEC_H_ */
