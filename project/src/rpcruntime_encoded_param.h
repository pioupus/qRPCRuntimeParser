#ifndef RPCRUNTIMEENCODEDPARAM_H
#define RPCRUNTIMEENCODEDPARAM_H

#include "rpcruntime_paramter_description.h"

#include <cinttypes>
#include <initializer_list>
#include <vector>

/*
 * A RPCRuntimeEncodedParam represents a script-settable parameter for an RPC-Function call
 */

class RPCRuntimeEncodedParam {
	public:
	RPCRuntimeEncodedParam(const RPCRuntimeParameterDescription &parameter_description);

	bool are_all_values_set() const;

	void encode(std::vector<unsigned char> &buffer) const;

	const RPCRuntimeParameterDescription *get_description() const;

	void set_value(int64_t value);
	void set_value(const std::string &value);

	RPCRuntimeEncodedParam &get_parameter(int index);
	RPCRuntimeEncodedParam &get_parameter(const std::string &name);

	RPCRuntimeEncodedParam &operator[](int index);
	RPCRuntimeEncodedParam &operator[](const std::string &name);

	RPCRuntimeEncodedParam &operator=(int64_t value);
	RPCRuntimeEncodedParam &operator=(const std::string &name);
	RPCRuntimeEncodedParam &operator=(std::initializer_list<int64_t> data);

		private : void set_integer_value(int64_t value);
	void set_enum_value(int value);
	void set_enum_value(const std::string &value);
	void set_array_value(const std::string &array);

	const RPCRuntimeParameterDescription *description;
	std::vector<unsigned char> data;
	std::vector<RPCRuntimeEncodedParam> child_parameters;
};

#endif //RPCRUNTIMEENCODEDPARAM_H
