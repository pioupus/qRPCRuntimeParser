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
    bool function_exists_for_encoding(const std::string &function_name) const;

	void set_description(const RPCRunTimeProtocolDescription &description);



private:
	const RPCRunTimeProtocolDescription *description = nullptr;
};

#endif // RPCRUNTIMEENCODERESULT_H
