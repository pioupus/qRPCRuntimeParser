#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <string>
#include <vector>

/*
 * A RPCRuntimeParameterDescription represents how a Parameter is encoded in a RPC-generated message
 */

class RPCRuntimeDecodedParam;

struct RPCRuntimeIntegerParameter;
struct RPCRuntimeEnumerationParameter;
struct RPCRuntimeStructureParameter;
struct RPCRuntimeArrayParameter;

class RPCRuntimeParameterDescription {
	public:
	enum class Type { integer, enumeration, structure, array };

	RPCRuntimeParameterDescription(Type type, int bit_size, std::string parameter_name);

	RPCRuntimeDecodedParam create_value() const;
	Type get_type() const;
	int get_bit_size() const;
	const std::string &get_parameter_name() const;
	int get_parameter_position() const;

	RPCRuntimeIntegerParameter as_integer() const;
	RPCRuntimeEnumerationParameter as_enumeration() const;
	RPCRuntimeStructureParameter as_structure() const;
	RPCRuntimeArrayParameter as_array() const;

	private:
	Type type;
	int bit_size;
	std::string parameter_name;
	int parameter_position;
};

struct RPCRuntimeIntegerParameter {
	bool is_signed;
};

struct RPCRuntimeEnumerationParameter {
	std::vector<std::pair<std::string, int>> values;
};

struct RPCRuntimeStructureParameter {
	std::vector<RPCRuntimeParameterDescription> members;
};

struct RPCRuntimeArrayParameter {
	std::vector<RPCRuntimeParameterDescription> members;
};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
