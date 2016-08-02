/*
 * channel_codec_config.h
 *
 *  Created on: 02.08.2016
 *      Author: ak
 */

#ifndef TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_
#define TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_

typedef enum{
	channel_codec_instance_uart,
	channel_codec_instance_COUNT
} channel_codec_instance_index_t;

#define CHANNEL_CODEC_TX_BUFFER_SIZE 64
#define CHANNEL_CODEC_RX_BUFFER_SIZE 64
//#define CHANNEL_CODEC_MAX_INSTANCES 2

#endif /* TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_ */
