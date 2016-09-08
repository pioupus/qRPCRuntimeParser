#include "rpcruntime_transfer.h"

RPCRuntimeTransfer::RPCRuntimeTransfer(const RPCRunTimeProtocolDescription &protocol)
	: function(&protocol) {
	// TODO: reserve space in data according to the size of the function
}

int RPCRuntimeTransfer::get_min_number_of_bytes() const {
	if (data.empty()) {
		return 1; //need at least 1 byte for the ID
	}
	//TODO: check in protocol how long function with ID data[0] is
	return 42;
}
