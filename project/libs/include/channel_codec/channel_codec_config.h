/*
 * channel_codec_config.h
 *
 *  Created on: 02.08.2016
 *      Author: ak
 */

#ifndef TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_
#define TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_




struct RPCRuntimeDecoder;

typedef enum{
	channel_codec_comport_uart,
	channel_codec_comport_COUNT
} channel_codec_conf_comport_t;

typedef struct{
        channel_codec_conf_comport_t portindex;
        struct RPCRuntimeDecoder *RPCRuntimeDecoderInstance;
}channel_codec_conf_auxdata_t;




#endif /* TESTS_INCLUDE_CHANNEL_CODEC_CHANNEL_CODEC_CONFIG_H_ */
