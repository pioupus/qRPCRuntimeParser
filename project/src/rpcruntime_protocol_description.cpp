#include "rpcruntime_protocol_description.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

struct Common_parameter_attributes {
	int bit_size;
	std::string parameter_name;
	std::string parameter_ctype;
	int parameter_position;
};

static RPCRuntimeParameterDescription parse_parameter(QXmlStreamReader &xml_reader);

static Common_parameter_attributes parse_common_parameter_attributes(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes retval;
	const auto &parameter_attributes = xml_reader.attributes();

	retval.bit_size = parameter_attributes.value("bits").toInt();
	assert(retval.bit_size != 0); //this fails if bit_size doesn't exist or could not be parsed or has an invalid value

	retval.parameter_name = parameter_attributes.value("name").toString().toStdString();

	retval.parameter_ctype = parameter_attributes.value("ctype").toString().toStdString();

	retval.parameter_position = parameter_attributes.value("position").toInt();
	if (retval.parameter_position == 0) {
		retval.parameter_position = parameter_attributes.value("memberpos").toInt();
	}

	return retval;
}

static RPCRuntimeParameterDescription parse_integer_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);
	RPCRuntimeIntegerParameter integer_parameter;

	const auto &parameter_attributes = xml_reader.attributes();
	assert(parameter_attributes.value("type") == "integer");

	xml_reader.readNextStartElement();
	assert(xml_reader.name() == "integer");
	auto signed_string = xml_reader.attributes().value("signed");
	if (signed_string == "True" || signed_string == "true") {
		integer_parameter.is_signed = true;
	} else if (signed_string == "False" || signed_string == "false") {
		integer_parameter.is_signed = false;
	} else {
        throw std::runtime_error("RPC: Integer parameter has no propper signed attribute");
	}
	xml_reader.skipCurrentElement(); //integer

	xml_reader.skipCurrentElement(); //parameter
    RPCRuntimeParameterDescription retval{common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
                common_attributes.parameter_position, std::move(integer_parameter)};
    //retval.set_field_id(parent_field_id+"."+retval.get_parameter_name());
    return retval;
}

static RPCRuntimeParameterDescription parse_enum_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);
	RPCRuntimeEnumerationParameter enumeration;
	enumeration.enum_name = common_attributes.parameter_ctype;

	while (xml_reader.readNextStartElement()) {
		assert(xml_reader.name() == "enum");
		const auto &enum_attributes = xml_reader.attributes();
		RPCRuntimeEnumerationParameter::Enum_value ev;
		ev.value = enum_attributes.value("value").toString().toStdString();
		ev.name = enum_attributes.value("name").toString().toStdString();

		enumeration.values.push_back(std::move(ev));

		xml_reader.skipCurrentElement(); //enum
	}

    RPCRuntimeParameterDescription retval{common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
                common_attributes.parameter_position, std::move(enumeration)};
    //retval.set_field_id(parent_field_id+"."+retval.get_parameter_name());
    return retval;
}

static RPCRuntimeParameterDescription parse_character_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);
	RPCRuntimeCharacterParameter character;

	xml_reader.skipCurrentElement();

    RPCRuntimeParameterDescription retval{common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
                common_attributes.parameter_position, std::move(character)};
    //retval.set_field_id(parent_field_id+"."+retval.get_parameter_name());
    return retval;
}

static RPCRuntimeParameterDescription parse_struct_parameter(QXmlStreamReader &xml_reader);

static RPCRuntimeParameterDescription parse_array_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);


	const auto &parameter_attributes = xml_reader.attributes();
	assert(parameter_attributes.value("type") == "array");

	int bit_size = parameter_attributes.value("bits").toInt();
	(void)bit_size;

	xml_reader.readNextStartElement();

	const auto &array_attributes = xml_reader.attributes();
	assert(xml_reader.name() == "array");

	int number_of_elements = array_attributes.value("elements").toInt();
	assert(number_of_elements);

    auto type = parse_parameter(xml_reader);
	if (type.get_type() == RPCRuntimeParameterDescription::Type::structure) {
		type.fix_array_bit_byte_bug();
	}
	RPCRuntimeArrayParameter array{std::move(type), number_of_elements};
	//array.type.bit_size = bit_size:
	xml_reader.skipCurrentElement();

    RPCRuntimeParameterDescription retval{common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
                common_attributes.parameter_position, std::move(array)};

    //retval.set_field_id(field_id);
    return retval;
}

static RPCRuntimeParameterDescription parse_struct_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);
	RPCRuntimeStructureParameter structure;


	while (xml_reader.readNextStartElement()) {
		assert(xml_reader.name() == "parameter");
        structure.members.push_back(parse_parameter(xml_reader));
	}

    RPCRuntimeParameterDescription retval{common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
                common_attributes.parameter_position, std::move(structure)};

    //retval.set_field_id(field_id);
    return retval;
}

static RPCRuntimeParameterDescription parse_parameter(QXmlStreamReader &xml_reader) {
	assert(xml_reader.name() == "parameter" || xml_reader.name() == "array");
	const auto &parameter_attributes = xml_reader.attributes();
	const auto &type_name = parameter_attributes.value("type");
	if (type_name == "integer") {
       return parse_integer_parameter(xml_reader);
	} else if (type_name == "enum") {
        return parse_enum_parameter(xml_reader);
	} else if (type_name == "struct") {
        return parse_struct_parameter(xml_reader);
	} else if (type_name == "array") {
        return parse_array_parameter(xml_reader);
	} else if (type_name == "character") {
        return parse_character_parameter(xml_reader);
	}
	//unknown type
	qDebug() << "unknown parameter type" << type_name;
    throw std::runtime_error("RPC: unknown parameter type: " + type_name.toString().toStdString());
}

static std::vector<RPCRuntimeParameterDescription> parse_parameters(QXmlStreamReader &xml_reader) {
	std::vector<RPCRuntimeParameterDescription> retval;
	while (xml_reader.readNextStartElement()) {
		if (xml_reader.name() != "parameter") {
			qDebug() << "next element should be \"parameter\" but is " << xml_reader.name() << "instead";
		}
		assert(xml_reader.name() == "parameter");
		//const auto &parameter_attributes = xml_reader.attributes();
		//debugoutput << "parameter name: " << parameter_attributes.value("name").toString().toStdString() << '\n';
		//xml_reader.skipCurrentElement();
        retval.push_back(parse_parameter(xml_reader));
	}
	return retval;
}

static RPCRuntimeFunction parse_function(QXmlStreamReader &xml_reader) {
	int request_id = -1;
	int reply_id = -1;
	std::vector<RPCRuntimeParameterDescription> request_parameters;
	std::vector<RPCRuntimeParameterDescription> reply_parameters;
	std::string function_name;
	std::string function_declaration;

	const auto &function_attributes = xml_reader.attributes();
	if (xml_reader.name() != "function") {
		qDebug() << "next element should be \"function\" but is " << xml_reader.name() << "instead";
	}

	assert(xml_reader.name() == "function");
	function_name = function_attributes.value("name").toString().toStdString();

	while (xml_reader.readNextStartElement()) {
		if (xml_reader.name() == "declaration") {
			function_declaration = xml_reader.readElementText().toStdString();
		} else if (xml_reader.name() == "request") {
			bool ok;
			request_id = xml_reader.attributes().value("ID").toInt(&ok);
			if (!ok) {
				request_id = -1;
			}
            request_parameters = parse_parameters(xml_reader);
		} else if (xml_reader.name() == "reply") {
			bool ok;
			reply_id = xml_reader.attributes().value("ID").toInt(&ok);
			if (!ok) {
				reply_id = -1;
			}
            reply_parameters = parse_parameters(xml_reader);
		} else {
			qDebug() << "unknown function attribute" << xml_reader.name();
			xml_reader.skipCurrentElement();
		}
	}

	return {request_id, reply_id, std::move(request_parameters), std::move(reply_parameters), std::move(function_name), std::move(function_declaration)};
}

RPCRunTimeProtocolDescription::RPCRunTimeProtocolDescription() {
	//if we have no protocol description just load a dummy that contains only the hash function
	std::istringstream iss(R"(<?xml version='1.0' encoding='utf-8'?>
	<RPC>
       <function name="RPC_SERVICE_get_hash">
           <declaration>RPC_RESULT RPC_SERVICE_get_hash(unsigned char hash_inout[16], uint32_t hash_index_inout[1], unsigned char start_command_id_out[1], uint16_t version_out[1]);</declaration>
           <request ID="0">
               <parameter bits="128" ctype="unsigned char [16]" name="hash_inout" position="1" type="array">
                   <array bits="8" ctype="unsigned char" elements="16" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
               <parameter bits="32" ctype="uint32_t [1]" name="hash_index_inout" position="2" type="array">
                   <array bits="32" ctype="uint32_t" elements="1" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
           </request>
           <reply ID="1">
               <parameter bits="128" ctype="unsigned char [16]" name="hash_inout" position="1" type="array">
                   <array bits="8" ctype="unsigned char" elements="16" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
               <parameter bits="32" ctype="uint32_t [1]" name="hash_index_inout" position="2" type="array">
                   <array bits="32" ctype="uint32_t" elements="1" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
               <parameter bits="8" ctype="unsigned char [1]" name="start_command_id_out" position="3" type="array">
                   <array bits="8" ctype="unsigned char" elements="1" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
               <parameter bits="16" ctype="uint16_t [1]" name="version_out" position="4" type="array">
                   <array bits="16" ctype="uint16_t" elements="1" type="integer">
                       <integer signed="False"/>
                   </array>
               </parameter>
           </reply>
       </function>
	</RPC>)");
	auto success = open_description(iss);
	assert(success);
}

RPCRunTimeProtocolDescription::RPCRunTimeProtocolDescription(std::istream &input) {
	if (!open_description(input)) {
		reset();
	}
}

bool RPCRunTimeProtocolDescription::openProtocolDescription(std::istream &input) {
	RPCRunTimeProtocolDescription other(input);
	if (other.get_functions().empty()) { //failed loading
		return false;
	}
	*this = std::move(other);
	return true;
}

const std::vector<RPCRuntimeFunction> &RPCRunTimeProtocolDescription::get_functions() const {
	return functions;
}

const std::string &RPCRunTimeProtocolDescription::get_hash() const {
	return hash;
}

const std::string &RPCRunTimeProtocolDescription::get_project_name() const {
	return project_name;
}

int RPCRunTimeProtocolDescription::get_version_number() const {
	return version_number;
}

int RPCRunTimeProtocolDescription::get_command_id_start() const {
	return command_id_start;
}

const std::vector<RPCRuntimeParameterDescription> &RPCRunTimeProtocolDescription::get_parameters(int id) const {
	if (id % 2) {
		return get_function(id).get_reply_parameters();
	}
	return get_function(id).get_request_parameters();
}

int RPCRunTimeProtocolDescription::get_parameter_size_bytes(int id) const {
	auto &parameters = get_parameters(id);
	return std::accumulate(std::begin(parameters), std::end(parameters), 1,
						   [](int sum, const RPCRuntimeParameterDescription &param) { return sum + param.get_bit_size() / 8; });
}

const RPCRuntimeFunction &RPCRunTimeProtocolDescription::get_function(int id) const {
	for (auto &function : functions) {
		if (function.get_reply_id() == id) {
			return function;
		}
		if (function.get_request_id() == id) {
			return function;
		}
	}
	//don't have a function with the appropriate ID
    throw std::runtime_error("RPC: " +std::to_string(id) + " is not a valid reply- or request ID");
}

const RPCRuntimeFunction &RPCRunTimeProtocolDescription::get_function(const std::string &name) const {
	for (auto &function : functions) {
		if (function.get_function_name() == name) {
			return function;
		}
	}
	//don't have a function with the appropriate name
    throw std::runtime_error("RPC: \"" + name + "\" is not a valid function name");
}

bool RPCRunTimeProtocolDescription::has_function(const std::string &name) const {
	for (auto &function : functions) {
		if (function.get_function_name() == name) {
			return true;
		}
	}
	return false;
}

bool RPCRunTimeProtocolDescription::open_description(std::istream &input) {
#define MAKESURE(X)                                                                                                                                            \
	do {                                                                                                                                                       \
		if (!(X)) {                                                                                                                                            \
			return false;                                                                                                                                      \
		}                                                                                                                                                      \
	} while (0)
	reset();
	QXmlStreamReader xml_reader;
	xml_reader.setNamespaceProcessing(false);
	for (std::string line; std::getline(input, line);) {
		xml_reader.addData(line.data());
	}

	MAKESURE(xml_reader.readNextStartElement());
	MAKESURE(xml_reader.name() == "RPC");

	const auto &rpc_attributes = xml_reader.attributes();

	hash = rpc_attributes.value("hash").toString().toStdString();
	project_name = rpc_attributes.value("hash").toString().toStdString();
	version_number = rpc_attributes.value("version_number").toInt();
	command_id_start = rpc_attributes.value("command_id_start").toInt();

	while (xml_reader.readNextStartElement()) {
		functions.push_back(parse_function(xml_reader));
	}

	if (xml_reader.hasError()) { // if openProtocolDescription returns false put a
								 // breakpoint here to see the error message
		auto error = xml_reader.errorString();
		(void)error;
		return false;
	}
	return true;
#undef MAKESURE
}

void RPCRunTimeProtocolDescription::reset() {
	functions.clear();
	hash.clear();
	project_name.clear();
	version_number = -1;
	command_id_start = -1;
}
