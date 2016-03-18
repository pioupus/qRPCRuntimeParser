/*
 * channel.c
 *
 *  Created on: 30.03.2015
 *      Author: ak
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "errorlogger/generic_eeprom_errorlogger.h"
#include "channel_codec/channel_codec.h"
#include "channel_codec/crc16.h"
#include "channel_codec/phylayer.h"


typedef enum{csNone,csFoundPreamble,csLoadingPayload,csPayloadComplete,csCRCAndPackageComplete} cc_channel_state_t;

#define CHANNEL_TX_BUFFER_SIZE 64
#define CHANNEL_RX_BUFFER_SIZE 64
#define PREAMBLE_LENGTH 3
#define CRC_LENGTH 2


#if CHANNEL_BLOCKLENGTH == 16
#define BLOCK_SIZEFACTOR 2
#elif CHANNEL_BLOCKLENGTH == 8
#define BLOCK_SIZEFACTOR 1
#else
#error illegal CHANNEL_BLOCKLENGTH
#endif

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

txState_t txState;
rxState_t rxState;


static void channel_decode(unsigned char byte);
static void channel_encode(unsigned char byte);
static cc_channel_state_t ccChannelState;

static uint8_t searchForPreamble(uint8_t byte);
static bool isRPCAnswer(const void *buffer, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult);
static bool isRPCRequest(const void *buffer, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult);

static void reset_rx(void){
	rxState.writePointer=0;
	rxState.bitmask=0;
	rxState.indexInBlock=0;
	rxState.messageResult.result = RPC_TRANSMISSION_COMMAND_UNKNOWN;
	ccChannelState = csNone;
}

static void reset_tx(void){
	txState.writePointer=0;
	txState.bitMaskPositionInBuffer = 0;
	txState.indexInBlock = 0;
	crc16_online_init(&txState.crc16);
}

void channel_init(void){
	reset_tx();
	reset_rx();
	rxState.preambleBuffer[0] = 0;
	rxState.preambleBuffer[1] = 0;
	rxState.preambleBuffer[2] = 0;
	RPC_TRANSMISSION_Parser_init();
}

static void appendByteToTXBuffer(char byte){
	if (txState.writePointer < CHANNEL_TX_BUFFER_SIZE){
		txState.buffer[txState.writePointer] = byte;
		txState.writePointer++;
	}else{
		GEN_ASSERT(0,errlog_E_CHCODEC_exceeding_RPC_TX_Buffer, "CC Exceeding RPC TX Buffer %d\n",txState.writePointer);
	}
}

static void appendByteToRXBuffer(char byte){
	if (rxState.writePointer < CHANNEL_RX_BUFFER_SIZE){
#if 0
		printf("[%d] %02X\n",channel_rx_write_pointer,(unsigned char)byte);
#endif
		rxState.buffer[rxState.writePointer] = byte;
		rxState.writePointer++;
	}else{
		reset_rx();
		GEN_WARNING(errlog_W_CHCODEC_exceeding_RPC_RX_buffer,"CC Exceeding RPC RX Buffer %d. Buffer reset\n",rxState.writePointer);
	}
}

void channel_start_message_from_RPC(size_t size){
/*  This function is called when a new message starts. {size} is the number of
    bytes the message will require. In the implementation you can allocate  a
    buffers or write a preamble. The implementation can be empty if you do not
    need to do that. */
	(void)size;
	reset_tx();
	appendByteToTXBuffer(0xFF);
	appendByteToTXBuffer(0xFF);
	appendByteToTXBuffer(0xFF);
}

void channel_decode(unsigned char byte){

	//indexInBlock index == 0: incoming byte is a bitmask
	//indexInBlock index == 1: incoming byte is first Byte in Block
	if (rxState.indexInBlock == 0){
		rxState.bitmask = byte;
		#if 0
		printf("channel_rx_bitmask %02X\n",channel_rx_bitmask);
		#endif
	}else {/*  channel_rx_bit_pointer > 0 */
		uint8_t bit = rxState.bitmask;
		if ((BLOCK_SIZEFACTOR == 1) || (rxState.indexInBlock & (BLOCK_SIZEFACTOR-1))){
			bit >>= (rxState.indexInBlock-1)/BLOCK_SIZEFACTOR;
			bit &= 1;
		}else{
			bit = 0;
		}
		byte |= bit;
		appendByteToRXBuffer(byte);
	}
	rxState.indexInBlock++;
	if(rxState.indexInBlock==CHANNEL_BLOCKLENGTH-BLOCK_SIZEFACTOR+1)
		rxState.indexInBlock=0;
}

void channel_encode(unsigned char byte){
	if (txState.bitMaskPositionInBuffer == 0){
		appendByteToTXBuffer(0);
		txState.bitMaskPositionInBuffer = txState.writePointer-1;
		txState.indexInBlock = 0;
	}
	uint8_t bit = byte & 1;

	if ((txState.indexInBlock & (BLOCK_SIZEFACTOR-1))==0){
		uint8_t shiftBy = txState.indexInBlock/BLOCK_SIZEFACTOR;
		txState.buffer[txState.bitMaskPositionInBuffer] |= bit << shiftBy;
		byte &= 0xFE;
	}
	appendByteToTXBuffer(byte);
	txState.indexInBlock++;
	if (txState.indexInBlock == CHANNEL_BLOCKLENGTH-BLOCK_SIZEFACTOR){
#if 0
		printf("mask: %02X\n",(unsigned char)channel_tx_buffer[bitMaskBitPointer]);
#endif
		txState.bitMaskPositionInBuffer = 0;
		txState.indexInBlock = 0;
	}
#if 0
	printf("%02X\n",(unsigned char)byte);
#endif

}

void channel_push_byte_from_RPC(unsigned char byte){
/* Pushes a byte to be sent via network. You should put all the pushed bytes
   into a buffer and send the buffer when RPC_commit is called. If you run
   out of buffer you can send multiple partial messages as long as the other
   side puts them back together. */
	crc16_online(byte,&txState.crc16);
#if 0
	printf("%02X ",(unsigned char)byte);
#endif
	channel_encode(byte);
}

RPC_TRANSMISSION_RESULT channel_commit_from_RPC(void){
	appendByteToTXBuffer(txState.crc16);
	appendByteToTXBuffer(txState.crc16 >> 8) ;
	/*printf("%X\n",crc16val);*/
	//printf("commit %d\n",txState.buffer[4]);
	RPC_TRANSMISSION_RESULT result = phyPushDataBuffer(txState.buffer,txState.writePointer);
	reset_tx();
	return result;
}

static uint8_t searchForPreamble(uint8_t byte){


	rxState.preambleBuffer[0] = rxState.preambleBuffer[1];
	rxState.preambleBuffer[1] = rxState.preambleBuffer[2];
	rxState.preambleBuffer[2] = byte;
#if 0
	printf("%02X %02X %02X\n",channel_rx_preamblebuffer[0],
			channel_rx_preamblebuffer[2],
			channel_rx_preamblebuffer[1]
		);
#endif

	if(	(rxState.preambleBuffer[0]==0xFF)&&
		(rxState.preambleBuffer[2]==0xFF)&&
		(rxState.preambleBuffer[1]==0xFF)){

		return 1;
	}
	return 0;
}

bool isRPCAnswer(const void *buffer, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult){
	bool result=false;
	RPC_TRANSMISSION_SIZE_RESULT testResult;
	if(size_bytes == 0){
		testResult.result = RPC_TRANSMISSION_COMMAND_INCOMPLETE;
		testResult.size = 1;
	}else{
        testResult = RPC_TRANSMISSION_get_answer_length(buffer, size_bytes);
	}
	if (testResult.result == RPC_TRANSMISSION_SUCCESS){
		result = true;
		*sizeResult = testResult;
	}else if (testResult.result == RPC_TRANSMISSION_COMMAND_INCOMPLETE){
		*sizeResult = testResult;
    }else{//RPC_TRANSMISSION_FAILURE or RPC_TRANSMISSION_COMMAND_UNKNOWN
	}
	return result;
}

bool isRPCRequest(const void *buffer, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult){
	bool result=false;
	RPC_TRANSMISSION_SIZE_RESULT testResult;
	if(size_bytes == 0){
		testResult.result = RPC_TRANSMISSION_COMMAND_INCOMPLETE;
		testResult.size = 1;
	}else{
        testResult = RPC_TRANSMISSION_get_request_size(buffer, size_bytes);
	}
	if (testResult.result == RPC_TRANSMISSION_SUCCESS){
		result = true;
		*sizeResult = testResult;
	}else if (testResult.result == RPC_TRANSMISSION_COMMAND_INCOMPLETE){
		*sizeResult = testResult;
	}else{//RPC_TRANSMISSION_FAILURE or RPC_TRANSMISSION_COMMAND_UNKNOWN


	}
	return result;
}


void channel_push_byte_to_RPC(unsigned char byte)

{
	if(searchForPreamble(byte)){
		reset_rx();
		ccChannelState = csFoundPreamble;
		#if 0
		printf("found preamble\n");
		#endif

	}
	switch(ccChannelState){
	case csNone:
		break;

	case csFoundPreamble:
		ccChannelState = csLoadingPayload;
		break;

	case csLoadingPayload:
		channel_decode(byte);
		#if 0
		printf("payload[%d] %02X\n",channel_rx_write_pointer-1, byte);
		#endif
		if (rxState.messageResult.result != RPC_TRANSMISSION_SUCCESS){

            if (isRPCAnswer(rxState.buffer,rxState.writePointer,&rxState.messageResult) ) {

            }else if (isRPCRequest(rxState.buffer,rxState.writePointer, &rxState.messageResult)){

            }

			#if 0
			printf("%d, %d %d\n",channel_rx_message_size.size,channel_rx_message_size.result, channel_rx_write_pointer);
			#endif
		}
		if ((rxState.messageResult.result==RPC_TRANSMISSION_SUCCESS) && (rxState.messageResult.size == rxState.writePointer)){
			ccChannelState = csPayloadComplete;
		}
		break;
	case csPayloadComplete:
		appendByteToRXBuffer(byte);//receive CRC
		#if 0
		printf("channel_rx_payload_complete %d\n",channel_rx_write_pointer);
		#endif
		if (rxState.messageResult.size+CRC_LENGTH== rxState.writePointer){
			ccChannelState = csCRCAndPackageComplete;
		}else{
			break;
		}

	case csCRCAndPackageComplete:
		{

			uint16_t crc16val = crc16_buffer((uint8_t*)rxState.buffer,rxState.writePointer-CRC_LENGTH);
			uint8_t crc_16_msb = crc16val >> 8;
			uint8_t crc_16_lsb = crc16val & 0xFF;
	#if 0
			for (int i=0;i<channel_rx_write_pointer-2;i++){
				printf("%02X ",(unsigned char)channel_rx_buffer[i]);
			}


			printf("\n   [%d]%02X  %02X  \n",channel_rx_write_pointer-1,(unsigned char)channel_rx_buffer[channel_rx_write_pointer-CRC_LENGTH+1],crc_16_msb);
			printf("   [%d]%02X  %02X  \n",channel_rx_write_pointer-2,(unsigned char)channel_rx_buffer[channel_rx_write_pointer-CRC_LENGTH],crc_16_lsb);

			printf("%04X ",crc16val);
	#endif
			if ((crc_16_msb == (unsigned char)rxState.buffer[rxState.writePointer-CRC_LENGTH+1])
					&& (crc_16_lsb == (unsigned char)rxState.buffer[rxState.writePointer-CRC_LENGTH])){
				RPC_TRANSMISSION_SIZE_RESULT rpcTRANSMISSIONSize;
				rpcTRANSMISSIONSize.result = RPC_TRANSMISSION_COMMAND_UNKNOWN;
                if (isRPCAnswer(rxState.buffer, rxState.writePointer-CRC_LENGTH, &rpcTRANSMISSIONSize) ) {
					if (rpcTRANSMISSIONSize.result == RPC_TRANSMISSION_SUCCESS){

                        RPC_TRANSMISSION_parse_answer(rxState.buffer, rxState.writePointer-CRC_LENGTH);

					}
                }else if (isRPCRequest(rxState.buffer, rxState.writePointer-CRC_LENGTH, &rpcTRANSMISSIONSize)){
					if (rpcTRANSMISSIONSize.result == RPC_TRANSMISSION_SUCCESS){

                        RPC_TRANSMISSION_parse_request(rxState.buffer, rxState.writePointer-CRC_LENGTH);

					}
				}else{
					GEN_ASSERT(0,errlog_E_CHCODEC_RPC_parse_answer_request_Fail,"CC RPC_parse_answer/request Fail");
				}
			}else{
				GEN_WARNING(errlog_W_CHCODEC_RX_CRC_fail,"CC RX CRC Fail");
			}
			reset_rx();
			ccChannelState = csNone;
		}
		break;
	default:
	break;
	}
}
