#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <memory>
#include <string>
#include <vector>

class RPCRuntimeDecodedParam;

/*
 * A RPCRuntimeParameterDescription represents how a Parameter is encoded in a RPC-generated message
 */

struct RPCRuntimeArrayParameter;
struct RPCRuntimeCharacterParameter;
struct RPCRuntimeEnumerationParameter;
struct RPCRuntimeIntegerParameter;
struct RPCRuntimeStructureParameter;

class RPCRuntimeParameterDescription {
	public:
	enum class Type { integer, enumeration, structure, array, character };

	RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeIntegerParameter integer);
	RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeEnumerationParameter enumeration);
	RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeStructureParameter structure);
	RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeArrayParameter array);
	RPCRuntimeParameterDescription(int bit_size, std::string name, std::string ctype, int position, RPCRuntimeCharacterParameter character);

	RPCRuntimeDecodedParam create_value() const;
	Type get_type() const;
	int get_bit_size() const;
	const std::string &get_parameter_name() const;
	const std::string &get_parameter_type() const;
	int get_parameter_position() const;

	const RPCRuntimeIntegerParameter &as_integer() const;
	const RPCRuntimeEnumerationParameter &as_enumeration() const;
	const RPCRuntimeStructureParameter &as_structure() const;
	const RPCRuntimeArrayParameter &as_array() const;
	const RPCRuntimeCharacterParameter &as_character() const;

	RPCRuntimeParameterDescription(const RPCRuntimeParameterDescription &other);
	RPCRuntimeParameterDescription(RPCRuntimeParameterDescription &&other);
	//RPCRuntimeParameterDescription &operator=(const RPCRuntimeParameterDescription &other);
	//RPCRuntimeParameterDescription &operator=(RPCRuntimeParameterDescription &&other);
	~RPCRuntimeParameterDescription();

	void fix_array_bit_byte_bug();

	private:
	Type type;
	int bit_size;
	std::string parameter_name;
	std::string parameter_ctype;
	int parameter_position;
	struct Type_dependent_values { //this could be a union, but the implementation difficulty is not justified by a couple of bytes
		RPCRuntimeIntegerParameter *integer;
		RPCRuntimeEnumerationParameter *enumeration;
		RPCRuntimeStructureParameter *structure;
		RPCRuntimeArrayParameter *array;
		RPCRuntimeCharacterParameter *character;
	} type_dependent_values;
};

struct RPCRuntimeIntegerParameter {
	bool is_signed;
};

struct RPCRuntimeEnumerationParameter {
	struct Enum_value{
		std::string value; //can usually be converted to an int, but not always
		std::string name;
		int to_int() const; //will throw a std::domain_error if value is not convertible to int
		bool is_int() const;
	};

	std::vector<Enum_value> values;
};

struct RPCRuntimeStructureParameter {
	std::vector<RPCRuntimeParameterDescription> members;
};

struct RPCRuntimeArrayParameter {
	RPCRuntimeArrayParameter(RPCRuntimeParameterDescription type, int number_of_elements);
	RPCRuntimeParameterDescription type;
	int number_of_elements = -1;
};

struct RPCRuntimeCharacterParameter {};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
