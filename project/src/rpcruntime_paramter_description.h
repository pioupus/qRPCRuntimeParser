#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <string>
#include <vector>

class RPCRuntimeDecodedParam;

/*
 * A RPCRuntimeParameterDescription represents how a Parameter is encoded in a RPC-generated message
 */

class RPCRuntimeParameterDescription;

struct RPCRuntimeIntegerParameter {
	bool is_signed;
};

struct RPCRuntimeEnumerationParameter {
	std::vector<std::pair<int, std::string>> values;
};

struct RPCRuntimeStructureParameter {
	std::vector<RPCRuntimeParameterDescription> members;
};

struct RPCRuntimeArrayParameter {
	std::vector<RPCRuntimeParameterDescription> members;
};


class RPCRuntimeParameterDescription {
	public:
	enum class Type { integer, enumeration, structure, array };

	RPCRuntimeParameterDescription(Type type, int bit_size, std::string parameter_name);

	RPCRuntimeDecodedParam create_value() const;
	Type get_type() const;
	int get_bit_size() const;
	const std::string &get_parameter_name() const;
	int get_parameter_position() const;

	const RPCRuntimeIntegerParameter &as_integer() const;
	const RPCRuntimeEnumerationParameter &as_enumeration() const;
	const RPCRuntimeStructureParameter &as_structure() const;
	const RPCRuntimeArrayParameter &as_array() const;

	private:
	Type type;
	int bit_size;
	std::string parameter_name;
	int parameter_position;
	struct Type_dependent_values{ //this could be a union, but the implementation difficulty is not justified by a couple of bytes
		RPCRuntimeIntegerParameter integer;
		RPCRuntimeEnumerationParameter enumeration;
		RPCRuntimeStructureParameter structure;
		RPCRuntimeArrayParameter array;
	}type_dependent_values;
};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
