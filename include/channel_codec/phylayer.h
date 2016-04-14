#ifndef PHYLAYER_H_
#define PHYLAYER_H_

#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"
#include "rpc_transmission/client/generated_general/RPC_TRANSMISSION_types.h"

RPC_TRANSMISSION_RESULT phyPushDataBuffer(const char *buffer, size_t length);

#endif
