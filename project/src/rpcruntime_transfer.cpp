#include "rpcruntime_transfer.h"

RPCRuntimeTransfer::RPCRuntimeTransfer(const RPCRuntimeFunction &function)
	: function(&function) {
	// TODO: reserve space in data according to the size of the function
}
