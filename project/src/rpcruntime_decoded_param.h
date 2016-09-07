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
	enum class Type { integer, enumeration, structure, array };
	RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &param);
	uint64_t as_uint() const;
	int64_t as_int() const;
	std::pair<int64_t, std::string> as_enum() const;
	std::pair<uint64_t, std::string> as_uenum() const;
	std::vector<std::string, RPCRuntimeDecodedParam> as_struct() const;
	std::vector<RPCRuntimeDecodedParam> as_array() const;
private:
	const RPCRuntimeParameterDescription *param;
};

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param);
std::ostream &operator<<(std::ostream &os, const RPCRuntimeDecodedParam &param);

#endif //RPCRUNTIMEDECODEDPARAM_H
