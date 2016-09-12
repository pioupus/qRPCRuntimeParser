#include "rpcruntime_decoded_param.h"
#include "rpcruntime_paramter_description.h"

#include <sstream>

RPCRuntimeDecodedParam::RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &parameter_description)
	: parameter_description(&parameter_description) {}

template <class Container>
struct Reverse_container {
	Reverse_container(Container &container)
		: container(container) {}
	auto begin(){
		return container.rbegin();
	}

	auto end(){
		return container.rend();
	}

	private:
	Container &container;
};

template <class Container>
Reverse_container<Container> reverse_view(Container &container){
	return {container};
}


int64_t RPCRuntimeDecodedParam::as_int() const {
	int64_t retval = 0;
	for (auto &chr : reverse_view(data)) {
		retval <<= 8;
		retval |= chr;
	}
	return retval;
}

const RPCRuntimeParameterDescription *RPCRuntimeDecodedParam::get_desciption() const {
	return parameter_description;
}

void RPCRuntimeDecodedParam::set_data(std::vector<unsigned char> data) {
	this->data = std::move(data);
}

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param) {
	auto byte_size = param.get_desciption()->get_bit_size() / 8;
	std::vector<unsigned char> data;
	data.resize(byte_size);
	is.read(reinterpret_cast<char *>(data.data()), byte_size);
	if (is) {
		param.set_data(std::move(data));
	}
	return is;
}
