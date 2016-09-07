#include "rpcruntime_transfer.h"

RPCRuntimeTransfer::RPCRuntimeTransfer(RPCRuntimeFunction &function, TransferType transfer_type)
	: function(&function)
	, transfer_type(transfer_type) {
	// TODO: reserve space in data according to the size of the function
}
