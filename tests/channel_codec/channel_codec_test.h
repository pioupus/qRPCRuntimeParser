/*
 * btchannelTest.h
 *
 *  Created on: 30.03.2015
 *      Author: ak
 */

#ifndef CHANNELCODEC_CHANNELCODEC_TEST_H_
#define CHANNELCODEC_CHANNELCODEC_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define CHANNEL_TX_BUFFER_SIZE 64
#define CHANNEL_RX_BUFFER_SIZE 64
#define PREAMBLE_LENGTH 3
#define CRC_LENGTH 2

RPC_TRANSMISSION_RESULT eunistonePushDataBuffer(const char *buffer, size_t length);
typedef struct{
	char buffer[CHANNEL_TX_BUFFER_SIZE];
	uint16_t indexInBlock;
	uint16_t bitMaskPositionInBuffer;
	uint16_t writePointer;
	uint16_t crc16;
}txState_t;

typedef struct{
	char buffer[CHANNEL_RX_BUFFER_SIZE];
	uint8_t indexInBlock;
	uint8_t bitmask;
	uint16_t writePointer;
	uint8_t preambleBuffer[PREAMBLE_LENGTH];
	RPC_TRANSMISSION_SIZE_RESULT messageResult;
}rxState_t;

extern txState_t txState;
extern rxState_t rxState;
#if 0
extern uint16_t channel_tx_bitMaskBitPointer;

extern uint16_t channel_tx_bitMaskPointer;
extern uint16_t channel_tx_write_pointer;
extern char channel_tx_buffer[];


extern uint16_t channel_rx_bitMaskBitPointer;
extern uint16_t channel_rx_bitMaskPointer;
extern uint16_t channel_rx_write_pointer;
extern char channel_rx_buffer[];
extern RPC_TRANSMISSION_SIZE_RESULT channel_rx_message_size;
#endif
#ifdef __cplusplus
}
#endif
#endif /* CHANNELCODEC_CHANNELCODEC_TEST_H_ */
