#ifndef RPCRUNTIMEENCODEDPARAM_H
#define RPCRUNTIMEENCODEDPARAM_H

#include "rpcruntime_paramter_description.h"

#include <cinttypes>
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

	void set_value(int64_t value) {
		switch (description->get_type()) {
			case RPCRuntimeParameterDescription::Type::integer:
				set_integer_value(value);
				break;
		case RPCRuntimeParameterDescription::Type::enumeration:
			set_enum_value(value);
			break;
		default:
			throw std::runtime_error("improper value type for this value");
		}
	}
	void set_value(const std::string &value) {
		switch (description->get_type()) {
			case RPCRuntimeParameterDescription::Type::enumeration:
				set_enum_value(value);
				break;
		default:
			throw std::runtime_error("improper value type for this value");
		}
	}

	private:
	void set_integer_value(int64_t value);
	void set_enum_value(int value);
	void set_enum_value(const std::string &value);

	const RPCRuntimeParameterDescription *description;
	std::vector<unsigned char> data;
};

#endif //RPCRUNTIMEENCODEDPARAM_H
