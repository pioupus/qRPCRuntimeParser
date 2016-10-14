/* This file has been automatically generated by RPC-Generator
   https://github.com/Crystal-Photonics/RPC-Generator
   You should not modify this file manually. */

    #ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include <stdint.h>
#include <assert.h>

#include "..\..\..\include\rpc_transmission\client\generated_general\RPC_TRANSMISSION_network.h"



#ifdef __cplusplus
}
#endif /* __cplusplus */

    /* Get (expected) size of (partial) answer. */
RPC_SIZE_RESULT RPC_TRANSMISSION_get_answer_length(const void *buffer, size_t size_bytes){
	const unsigned char *current = (const unsigned char *)buffer;
	RPC_SIZE_RESULT returnvalue;

	if (size_bytes == 0){
		returnvalue.result = RPC_COMMAND_INCOMPLETE;
		returnvalue.size = 1;
		return returnvalue;
	}

	if((*current) & 1){
		if (*current == 0xFF){
			returnvalue.size = 51;
		}else{
			returnvalue.size = *current;
		}
	}else{
		returnvalue.size = 0;
		returnvalue.result = RPC_COMMAND_UNKNOWN;
		return returnvalue;
	}

	returnvalue.result = returnvalue.size > size_bytes ? RPC_COMMAND_INCOMPLETE : RPC_SUCCESS;
	return returnvalue;
}



void RPC_TRANSMISSION_Parser_init(void){

}

void RPC_TRANSMISSION_Parser_exit(){

}

#ifdef __cplusplus
}
#endif /* __cplusplus */
