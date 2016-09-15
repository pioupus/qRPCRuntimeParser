#ifndef RPCRUNTIMEDECODEDPARAM_H
#define RPCRUNTIMEDECODEDPARAM_H

#include <cinttypes>
#include <istream>
#include <ostream>
#include <utility>
#include <vector>

class RPCRuntimeParameterDescription;

/*
 * A RPCRuntimeDecodedParam represents a script-understandable parameter in an RPC-Function call
 */

struct Decoded_struct;
struct Decoded_enum;

class RPCRuntimeDecodedParam {
	public:
	RPCRuntimeDecodedParam(const RPCRuntimeParameterDescription &parameter_description);
	uint64_t as_unsigned_integer() const;
	int64_t as_signed_integer() const;
	int64_t as_integer() const; //works for all integers except uint64_t
	Decoded_enum as_enum() const; //since we come from C we only need to support enums that have int as their underlying type
	std::vector<Decoded_struct> as_struct() const;
	std::vector<RPCRuntimeDecodedParam> as_array() const;
	std::string as_full_string() const;
	std::string as_string() const;

	const RPCRuntimeParameterDescription *get_desciption() const;
	void set_data(std::vector<unsigned char> data);
	void set_data(const unsigned char *begin, int size);

	private:
	const RPCRuntimeParameterDescription *parameter_description;
	std::vector<unsigned char> data;
};

struct Decoded_struct{
	std::string name;
	RPCRuntimeDecodedParam type;
};

struct Decoded_enum{
	std::string name; //may be empty
	int value;
};

std::istream &operator>>(std::istream &is, RPCRuntimeDecodedParam &param);

#endif //RPCRUNTIMEDECODEDPARAM_H
