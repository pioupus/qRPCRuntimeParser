#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoded_param.h"
#include "rpcruntime_function.h"

#include <memory>
#include <sstream>

RPCRuntimeDecodedFunctionCall::RPCRuntimeDecodedFunctionCall(int id, std::vector<RPCRuntimeDecodedParam> params, const RPCRuntimeFunction &function)
	: id(id)
	, params(std::move(params))
	, function(&function) {}

std::vector<RPCRuntimeDecodedParam> RPCRuntimeDecodedFunctionCall::get_decoded_parameters() const {
	return params;
}

const RPCRuntimeDecodedParam *RPCRuntimeDecodedFunctionCall::get_parameter_by_name(const std::string &name) const {
	for (auto &param : params) {
		if (param.get_desciption()->get_parameter_name() == name) {
			return &param;
		}
	}
	return nullptr;
}

int RPCRuntimeDecodedFunctionCall::get_id() const {
	return id;
}

const RPCRuntimeFunction *RPCRuntimeDecodedFunctionCall::get_declaration() const {
	return function;
}

bool RPCRuntimeDecodedFunctionCall::is_request() const {
	return id % 2 == 0;
}

bool RPCRuntimeDecodedFunctionCall::is_reply() const {
    return id % 2 == 1;
}

std::unique_ptr<RPCRuntimeDecodedParam> RPCRuntimeDecodedFunctionCall::get_param_by_field_id(const std::string &field_id) const {
    std::vector<std::string> tokens;
    std::istringstream f(field_id);
    std::string s;
    while (std::getline(f, s, '.')) {
        tokens.push_back(s);
    }

	if (tokens.size() == 0) {
        return nullptr;
    }
	if (tokens[0] != std::to_string(id)) {
        return nullptr;
    }

    const RPCRuntimeDecodedParam *retval_p = get_parameter_by_name(tokens[1]);
	if (retval_p == nullptr) {
        return nullptr;
    }

    auto retval = std::make_unique<RPCRuntimeDecodedParam>(*retval_p);

	for (unsigned int i = 2; i < tokens.size(); i++) {
		if (retval->get_desciption()->get_type() == RPCRuntimeParameterDescription::Type::array) {
			unsigned int array_index = 0;
			try {
				array_index = std::stoi(tokens[i]);
			} catch (std::invalid_argument &e) {
				(void)e;
				return nullptr;
            }

            std::vector<RPCRuntimeDecodedParam> array = retval->as_array();
			if (array.size() <= array_index) {
                return nullptr;
            }
            *retval = array[array_index];
		} else if (retval->get_desciption()->get_type() == RPCRuntimeParameterDescription::Type::structure) {
            std::vector<Decoded_struct> fields = retval->as_struct();
            bool found = false;
			for (Decoded_struct ds : fields) {
				if (ds.name == tokens[i]) {
                    *retval = ds.type;
                    found = true;
                    break;
                }
            }
			if (!found) {
                return nullptr;
            }
        }
    }
    return retval;
}
