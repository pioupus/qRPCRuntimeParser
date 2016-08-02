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
	char buffer[CHANNEL_CODEC_TX_BUFFER_SIZE];
	uint16_t indexInBlock;
	uint16_t bitMaskPositionInBuffer;
	uint16_t writePointer;
	uint16_t crc16;
}txState_t;

typedef struct{
	char buffer[CHANNEL_CODEC_RX_BUFFER_SIZE];
	uint8_t indexInBlock;
	uint8_t bitmask;
	uint16_t writePointer;
	uint8_t preambleBuffer[PREAMBLE_LENGTH];
	RPC_TRANSMISSION_SIZE_RESULT messageResult;
}rxState_t;

typedef struct{
	txState_t txState;
	rxState_t rxState;
	cc_channel_state_t ccChannelState;
	RPC_parse_request_t RPC_parse_request;
	RPC_parse_answer_t RPC_parse_answer;
	RPC_get_request_size_t RPC_get_request_size;
	RPC_get_answer_length_t RPC_get_answer_length;
	RPC_Parser_init_t RPC_Parser_init;
}instances_t;



instances_t instances[channel_codec_instance_COUNT];

static void channel_decode(channel_codec_instance_index_t instance_index,unsigned char byte);
static void channel_encode(channel_codec_instance_index_t instance_index,unsigned char byte);


static uint8_t searchForPreamble(channel_codec_instance_index_t instance_index, uint8_t byte);
static bool isRPCAnswer(channel_codec_instance_index_t instance_index, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult);
static bool isRPCRequest(channel_codec_instance_index_t instance_index, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult);

static void reset_rx(channel_codec_instance_index_t instance_index){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	instances[instance_index].rxState.writePointer=0;
	instances[instance_index].rxState.bitmask=0;
	instances[instance_index].rxState.indexInBlock=0;
	instances[instance_index].rxState.messageResult.result = RPC_TRANSMISSION_COMMAND_UNKNOWN;
	instances[instance_index].ccChannelState = csNone;
}

static void reset_tx(channel_codec_instance_index_t instance_index){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	instances[instance_index].txState.writePointer=0;
	instances[instance_index].txState.bitMaskPositionInBuffer = 0;
	instances[instance_index].txState.indexInBlock = 0;
	crc16_online_init(&instances[instance_index].txState.crc16);
}

static bool channelTestIfInitialized(channel_codec_instance_index_t instance_index){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	bool result= (instances[instance_index].RPC_parse_request) &&
	(instances[instance_index].RPC_parse_answer) &&
	(instances[instance_index].RPC_get_request_size) &&
	(instances[instance_index].RPC_get_answer_length) &&
	(instances[instance_index].RPC_Parser_init);
	if (!result){
		GEN_ASSERT(0,errlog_E_CHCODEC_RPC_Functions_uninitialized, "CC RPC Function pointers uninitialized\n");
	}
	return result;
}

void channel_init(void){
	for (uint8_t i=0; i<channel_codec_instance_COUNT;i++){
		reset_tx(i);
		reset_rx(i);
		instances[i].rxState.preambleBuffer[0] = 0;
		instances[i].rxState.preambleBuffer[1] = 0;
		instances[i].rxState.preambleBuffer[2] = 0;
	}

}

void channel_init_instance(channel_codec_instance_index_t instance_index,
		RPC_parse_request_t RPC_parse_request,
		RPC_parse_answer_t RPC_parse_answer,
		RPC_get_request_size_t RPC_get_request_size,
		RPC_get_answer_length_t RPC_get_answer_length,
		RPC_Parser_init_t RPC_Parser_init){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	instances[instance_index].RPC_parse_request = RPC_parse_request;
	instances[instance_index].RPC_parse_answer = RPC_parse_answer;
	instances[instance_index].RPC_get_request_size = RPC_get_request_size;
	instances[instance_index].RPC_get_answer_length = RPC_get_answer_length;
	instances[instance_index].RPC_Parser_init = RPC_Parser_init;
	if (channelTestIfInitialized(instance_index)){
		instances[instance_index].RPC_Parser_init();
	}
}

static void appendByteToTXBuffer(channel_codec_instance_index_t instance_index, char byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	if (instances[instance_index].txState.writePointer < CHANNEL_CODEC_TX_BUFFER_SIZE){
		instances[instance_index].txState.buffer[instances[instance_index].txState.writePointer] = byte;
		instances[instance_index].txState.writePointer++;
	}else{
		GEN_ASSERT(0,errlog_E_CHCODEC_exceeding_RPC_TX_Buffer, "CC Exceeding RPC TX Buffer %d\n",instances[instance_index].txState.writePointer);
	}
}

static void appendByteToRXBuffer(channel_codec_instance_index_t instance_index, char byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	if (instances[instance_index].rxState.writePointer < CHANNEL_CODEC_RX_BUFFER_SIZE){
#if 0
		printf("[%d] %02X\n",channel_rx_write_pointer,(unsigned char)byte);
#endif
		instances[instance_index].rxState.buffer[instances[instance_index].rxState.writePointer] = byte;
		instances[instance_index].rxState.writePointer++;
	}else{
		reset_rx(instance_index);
		GEN_WARNING(errlog_W_CHCODEC_exceeding_RPC_RX_buffer,"CC Exceeding RPC RX Buffer %d. Buffer reset\n",instances[instance_index].rxState.writePointer);
	}
}

void channel_start_message_from_RPC(channel_codec_instance_index_t instance_index, size_t size){
/*  This function is called when a new message starts. {size} is the number of
    bytes the message will require. In the implementation you can allocate  a
    buffers or write a preamble. The implementation can be empty if you do not
    need to do that. */
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}

	(void)size;
	reset_tx(instance_index);
	appendByteToTXBuffer(instance_index,0xFF);
	appendByteToTXBuffer(instance_index,0xFF);
	appendByteToTXBuffer(instance_index,0xFF);
}

void channel_decode(channel_codec_instance_index_t instance_index, unsigned char byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	//indexInBlock index == 0: incoming byte is a bitmask
	//indexInBlock index == 1: incoming byte is first Byte in Block
	if (instances[instance_index].rxState.indexInBlock == 0){
		instances[instance_index].rxState.bitmask = byte;
		#if 0
		printf("channel_rx_bitmask %02X\n",channel_rx_bitmask);
		#endif
	}else {/*  channel_rx_bit_pointer > 0 */
		uint8_t bit = instances[instance_index].rxState.bitmask;
		if ((BLOCK_SIZEFACTOR == 1) || (instances[instance_index].rxState.indexInBlock & (BLOCK_SIZEFACTOR-1))){
			bit >>= (instances[instance_index].rxState.indexInBlock-1)/BLOCK_SIZEFACTOR;
			bit &= 1;
		}else{
			bit = 0;
		}
		byte |= bit;
		appendByteToRXBuffer(instance_index,byte);
	}
	instances[instance_index].rxState.indexInBlock++;
	if(instances[instance_index].rxState.indexInBlock==CHANNEL_BLOCKLENGTH-BLOCK_SIZEFACTOR+1){
		instances[instance_index].rxState.indexInBlock=0;
	}
}

void channel_encode(channel_codec_instance_index_t instance_index, unsigned char byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	if (instances[instance_index].txState.bitMaskPositionInBuffer == 0){
		appendByteToTXBuffer(instance_index,0);
		instances[instance_index].txState.bitMaskPositionInBuffer = instances[instance_index].txState.writePointer-1;
		instances[instance_index].txState.indexInBlock = 0;
	}
	uint8_t bit = byte & 1;

	if ((instances[instance_index].txState.indexInBlock & (BLOCK_SIZEFACTOR-1))==0){
		uint8_t shiftBy = instances[instance_index].txState.indexInBlock/BLOCK_SIZEFACTOR;
		instances[instance_index].txState.buffer[instances[instance_index].txState.bitMaskPositionInBuffer] |= bit << shiftBy;
		byte &= 0xFE;
	}
	appendByteToTXBuffer(instance_index,byte);
	instances[instance_index].txState.indexInBlock++;
	if (instances[instance_index].txState.indexInBlock == CHANNEL_BLOCKLENGTH-BLOCK_SIZEFACTOR){
#if 0
		printf("mask: %02X\n",(unsigned char)channel_tx_buffer[bitMaskBitPointer]);
#endif
		instances[instance_index].txState.bitMaskPositionInBuffer = 0;
		instances[instance_index].txState.indexInBlock = 0;
	}
#if 0
	printf("%02X\n",(unsigned char)byte);
#endif

}

void channel_push_byte_from_RPC(channel_codec_instance_index_t instance_index, unsigned char byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
/* Pushes a byte to be sent via network. You should put all the pushed bytes
   into a buffer and send the buffer when RPC_commit is called. If you run
   out of buffer you can send multiple partial messages as long as the other
   side puts them back together. */
	crc16_online(byte,&instances[instance_index].txState.crc16);
#if 0
	printf("%02X ",(unsigned char)byte);
#endif
	channel_encode(instance_index,byte);
}

RPC_TRANSMISSION_RESULT channel_commit_from_RPC(channel_codec_instance_index_t instance_index){
	appendByteToTXBuffer(instance_index,instances[instance_index].txState.crc16);
	appendByteToTXBuffer(instance_index,instances[instance_index].txState.crc16 >> 8) ;
	/*printf("%X\n",crc16val);*/
	//printf("commit %d\n",txState.buffer[4]);
	RPC_TRANSMISSION_RESULT result = phyPushDataBuffer(instance_index, instances[instance_index].txState.buffer,instances[instance_index].txState.writePointer);
	reset_tx(instance_index);
	return result;
}

static uint8_t searchForPreamble(channel_codec_instance_index_t instance_index, uint8_t byte){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}

	instances[instance_index].rxState.preambleBuffer[0] = instances[instance_index].rxState.preambleBuffer[1];
	instances[instance_index].rxState.preambleBuffer[1] = instances[instance_index].rxState.preambleBuffer[2];
	instances[instance_index].rxState.preambleBuffer[2] = byte;
#if 0
	printf("%02X %02X %02X\n",channel_rx_preamblebuffer[0],
			channel_rx_preamblebuffer[2],
			channel_rx_preamblebuffer[1]
		);
#endif

	if(	(instances[instance_index].rxState.preambleBuffer[0]==0xFF)&&
		(instances[instance_index].rxState.preambleBuffer[2]==0xFF)&&
		(instances[instance_index].rxState.preambleBuffer[1]==0xFF)){

		return 1;
	}
	return 0;
}

bool isRPCAnswer(channel_codec_instance_index_t instance_index, const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	bool result=false;
	(void)instance_index;
	RPC_TRANSMISSION_SIZE_RESULT testResult;
	if(size_bytes == 0){
		testResult.result = RPC_TRANSMISSION_COMMAND_INCOMPLETE;
		testResult.size = 1;
	}else{
		if(channelTestIfInitialized(instance_index)){
			testResult = instances[instance_index].RPC_get_answer_length(instances[instance_index].rxState.buffer, size_bytes);
		}
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

bool isRPCRequest(channel_codec_instance_index_t instance_index,  const size_t size_bytes, RPC_TRANSMISSION_SIZE_RESULT *sizeResult){
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	bool result=false;
	RPC_TRANSMISSION_SIZE_RESULT testResult;
	(void)instance_index;
	if(size_bytes == 0){
		testResult.result = RPC_TRANSMISSION_COMMAND_INCOMPLETE;
		testResult.size = 1;
	}else{
		if(channelTestIfInitialized(instance_index)){
			testResult = instances[instance_index].RPC_get_request_size(instances[instance_index].rxState.buffer, size_bytes);
		}
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


void channel_push_byte_to_RPC(channel_codec_instance_index_t instance_index, unsigned char byte)

{
	if (instance_index >= channel_codec_instance_COUNT){
		GEN_ASSERT(0,errlog_E_CHCODEC_instance_index_beyond_max, "CC instance index beyond max\n");
	}
	if(searchForPreamble(instance_index,byte)){
		reset_rx(instance_index);
		instances[instance_index].ccChannelState = csFoundPreamble;
		#if 0
		printf("found preamble\n");
		#endif

	}
	switch(instances[instance_index].ccChannelState){
	case csNone:
		break;

	case csFoundPreamble:
		instances[instance_index].ccChannelState = csLoadingPayload;
		break;

	case csLoadingPayload:
		channel_decode(instance_index,byte);
		#if 0
		printf("payload[%d] %02X\n",channel_rx_write_pointer-1, byte);
		#endif
		if (instances[instance_index].rxState.messageResult.result != RPC_TRANSMISSION_SUCCESS){

            if (isRPCAnswer(instance_index,instances[instance_index].rxState.writePointer,&instances[instance_index].rxState.messageResult) ) {

            }else if (isRPCRequest(instance_index,instances[instance_index].rxState.writePointer, &instances[instance_index].rxState.messageResult)){

            }

			#if 0
			printf("%d, %d %d\n",channel_rx_message_size.size,channel_rx_message_size.result, channel_rx_write_pointer);
			#endif
		}
		if ((instances[instance_index].rxState.messageResult.result==RPC_TRANSMISSION_SUCCESS) && (instances[instance_index].rxState.messageResult.size == instances[instance_index].rxState.writePointer)){
			instances[instance_index].ccChannelState = csPayloadComplete;
		}
		break;
	case csPayloadComplete:
		appendByteToRXBuffer(instance_index,byte);//receive CRC
		#if 0
		printf("channel_rx_payload_complete %d\n",channel_rx_write_pointer);
		#endif
		if (instances[instance_index].rxState.messageResult.size+CRC_LENGTH== instances[instance_index].rxState.writePointer){
			instances[instance_index].ccChannelState = csCRCAndPackageComplete;
		}else{
			break;
		}

	case csCRCAndPackageComplete:
		{

			uint16_t crc16val = crc16_buffer((uint8_t*)instances[instance_index].rxState.buffer,instances[instance_index].rxState.writePointer-CRC_LENGTH);
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
			if ((crc_16_msb == (unsigned char)instances[instance_index].rxState.buffer[instances[instance_index].rxState.writePointer-CRC_LENGTH+1])
					&& (crc_16_lsb == (unsigned char)instances[instance_index].rxState.buffer[instances[instance_index].rxState.writePointer-CRC_LENGTH])){
				RPC_TRANSMISSION_SIZE_RESULT rpcTRANSMISSIONSize;
				rpcTRANSMISSIONSize.result = RPC_TRANSMISSION_COMMAND_UNKNOWN;
                if (isRPCAnswer(instance_index, instances[instance_index].rxState.writePointer-CRC_LENGTH, &rpcTRANSMISSIONSize) ) {
					if (rpcTRANSMISSIONSize.result == RPC_TRANSMISSION_SUCCESS){

						if(channelTestIfInitialized(instance_index)){
							instances[instance_index].RPC_parse_answer(instances[instance_index].rxState.buffer, instances[instance_index].rxState.writePointer-CRC_LENGTH);
						}
					}
                }else if (isRPCRequest(instance_index, instances[instance_index].rxState.writePointer-CRC_LENGTH, &rpcTRANSMISSIONSize)){
					if (rpcTRANSMISSIONSize.result == RPC_TRANSMISSION_SUCCESS){

						if(channelTestIfInitialized(instance_index)){
							instances[instance_index].RPC_parse_request(instances[instance_index].rxState.buffer, instances[instance_index].rxState.writePointer-CRC_LENGTH);
						}
					}
				}else{
					GEN_ASSERT(0,errlog_E_CHCODEC_RPC_parse_answer_request_Fail,"CC RPC_parse_answer/request Fail");
				}
			}else{
				GEN_WARNING(errlog_W_CHCODEC_RX_CRC_fail,"CC RX CRC Fail");
			}
			reset_rx(instance_index);
			instances[instance_index].ccChannelState = csNone;
		}
		break;
	default:
	break;
	}
}
