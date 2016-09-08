#ifndef RPCRUNTIMEDECODEDPARAM_H
#define RPCRUNTIMEDECODEDPARAM_H

#include <cinttypes>
#include <istream>
#include <utility>
#include <vector>

class RPCRuntimeParameterDescription;

/*
 * A RPCRuntimeDecodedParam represents a script-understandable parameter in an RPC-Function call
 */

class RPCRuntimeDecodedParam {
	public:
	RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &param);
	uint64_t as_uint() const;
	int64_t as_int() const;
	std::pair<int, std::string> as_enum() const; //since we come from C we only need to support enums that have int as their underlying type
	std::vector<std::string, RPCRuntimeDecodedParam> as_struct() const;
	std::vector<RPCRuntimeDecodedParam> as_array() const;

	private:
	const RPCRuntimeParameterDescription *description;
};

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param);
std::ostream &operator<<(std::ostream &os, const RPCRuntimeDecodedParam &param);

#endif //RPCRUNTIMEDECODEDPARAM_H
