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

	template <class T>
	void set_value(T &&t) {
		switch (description->get_type()) {
			case RPCRuntimeParameterDescription::Type::integer:
				set_integer_value(std::move(t));
				break;
		case RPCRuntimeParameterDescription::Type::array:
			break;
		case RPCRuntimeParameterDescription::Type::character:
			break;
		case RPCRuntimeParameterDescription::Type::enumeration:
			break;
		case RPCRuntimeParameterDescription::Type::structure:
			break;
		}
	}

	private:
	void set_integer_value(int64_t i);

	const RPCRuntimeParameterDescription *description;
	std::vector<unsigned char> data;
};

#endif //RPCRUNTIMEENCODEDPARAM_H
