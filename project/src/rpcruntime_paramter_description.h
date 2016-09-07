#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <string>

/*
 * A RPCRuntimeParameterDescription represents how a Parameter is encoded in a RPC-generated message
 */

class RPCRuntimeDecodedParam;

class RPCRuntimeParameterDescription
{
public:
	std::string parameter_name;
	std::string parameter_type; //TODO: this is probably not enough
	RPCRuntimeDecodedParam create_value() const;
};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
