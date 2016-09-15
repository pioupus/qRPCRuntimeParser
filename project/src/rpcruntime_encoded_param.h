#ifndef RPCRUNTIMEENCODEDPARAM_H
#define RPCRUNTIMEENCODEDPARAM_H

class RPCRuntimeParameterDescription;

#include <vector>

/*
 * A RPCRuntimeEncodedParam represents a script-settable parameter for an RPC-Function call
 */

class RPCRuntimeEncodedParam {
	public:
	RPCRuntimeEncodedParam(const RPCRuntimeParameterDescription &parameter_description);

	bool are_all_values_set() const;

	void encode(std::vector<unsigned char> &buffer) const;
};

#endif //RPCRUNTIMEENCODEDPARAM_H
