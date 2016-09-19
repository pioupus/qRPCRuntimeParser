#ifndef RPCRUNTIMETRANSFER_H
#define RPCRUNTIMETRANSFER_H

#include <vector>

class RPCRunTimeProtocolDescription;
class RPCRuntimeParameterDescription;
class RPCRuntimeDecodedFunctionCall;

/*
 * A RPCRuntimeTransfer represents the Data in RPC-Generator encoding received or to be sent
 */

class RPCRuntimeTransfer {
	public:
	//Note: The passed in RPCRunTimeProtocolDescription must stay valid during the life-time of a RPCRuntimeTransfer
	RPCRuntimeTransfer(const RPCRunTimeProtocolDescription &protocol);

	//number of bytes that are at least required to make a complete message
	int get_min_number_of_bytes() const;
	//returns if we don't require more bytes for the transfer to be complete
	bool is_complete() const;

	template <class Iterator>
	void add_data(Iterator begin_it, Iterator end_it) {
		data.insert(data.end(), begin_it, end_it);
	}
	void add_data(unsigned char byte);

	const std::vector<unsigned char> encode() const;
	RPCRuntimeDecodedFunctionCall decode() const;

	//throw away all previously collected data
	void reset();

	private:
	std::vector<unsigned char> data;
	const RPCRunTimeProtocolDescription *protocol;
};

#endif //RPCRUNTIMETRANSFER_H
