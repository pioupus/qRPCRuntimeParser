#ifndef RPCRUNTIMEENCODER_H
#define RPCRUNTIMEENCODER_H

class RPCRuntimeEncodedFunctionCall;
class RPCRunTimeProtocolDescription;

#include <string>

/*
 * The RPCRuntimeEncoder encodes values into valid binary data RPC-representation
 */

struct RPCRuntimeEncoder {
	RPCRuntimeEncoder(const RPCRunTimeProtocolDescription &description);

	RPCRuntimeEncodedFunctionCall encode(int id) const;
	RPCRuntimeEncodedFunctionCall encode(const std::string &function_name) const;

	private:
	const RPCRunTimeProtocolDescription *description;
};

#endif // RPCRUNTIMEENCODERESULT_H
