#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <string>

/*
 * A RPCRuntimeParameterDescription represents how a Parameter is encoded in a RPC-generated message
 */

class RPCRuntimeDecodedParam;

class RPCRuntimeParameterDescription {
	public:
	RPCRuntimeParameterDescription(int bit_size, std::string parameter_name, std::string parameter_type);

	RPCRuntimeDecodedParam create_value() const;
	int get_bit_size() const;
	const std::string &get_parameter_name() const;
	const std::string &get_parameter_type() const;

	private:
	int bit_size;
	std::string parameter_name;
	std::string parameter_type; //TODO: this is probably not enough
};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
