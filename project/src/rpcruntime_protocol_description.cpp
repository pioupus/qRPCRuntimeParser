#include "rpcruntime_protocol_description.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <cassert>
#include <fstream>
#include <string>

struct Common_parameter_attributes {
	int bit_size;
	std::string parameter_name;
	std::string parameter_ctype;
	int parameter_position;
};

static Common_parameter_attributes parse_common_parameter_attributes(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes retval;
	const auto &parameter_attributes = xml_reader.attributes();

	retval.bit_size = parameter_attributes.value("bits").toInt();
	assert(retval.bit_size != 0); //this fails if bit_size doesn't exist or could not be parsed or has an invalid value

	retval.parameter_name = parameter_attributes.value("name").toString().toStdString();

	retval.parameter_ctype = parameter_attributes.value("ctype").toString().toStdString();

	retval.parameter_position = parameter_attributes.value("position").toInt();
	assert(retval.parameter_position);

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
	if (signed_string == "True") {
		integer_parameter.is_signed = true;
	} else if (signed_string == "False") {
		integer_parameter.is_signed = false;
	} else {
		throw std::runtime_error("Integer parameter has no signed attribute");
	}
	xml_reader.skipCurrentElement(); //integer
	xml_reader.skipCurrentElement(); //parameter
	return {common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
			common_attributes.parameter_position, std::move(integer_parameter)};
}

static RPCRuntimeParameterDescription parse_enum_parameter(QXmlStreamReader &xml_reader) {
	Common_parameter_attributes common_attributes = parse_common_parameter_attributes(xml_reader);
	RPCRuntimeEnumerationParameter enumeration;

	xml_reader.skipCurrentElement(); //parameter
	return {common_attributes.bit_size, std::move(common_attributes.parameter_name), std::move(common_attributes.parameter_ctype),
			common_attributes.parameter_position, std::move(enumeration)};
}

static RPCRuntimeParameterDescription parse_parameter(QXmlStreamReader &xml_reader) {
	assert(xml_reader.name() == "parameter");
	const auto &parameter_attributes = xml_reader.attributes();
	const auto &type_name = parameter_attributes.value("type");
	if (type_name == "integer") {
		return parse_integer_parameter(xml_reader);
	} else if (type_name == "enum") {
		return parse_enum_parameter(xml_reader);
	} else if (type_name == "struct") {
		//return parse_struct_parameter();
	} else if (type_name == "array") {
		//return parse_array_parameter();
	}
	//unknown type
	qDebug() << "unknown parameter type" << type_name;
	throw std::runtime_error("unknown parameter type: " + type_name.toString().toStdString());
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
	qDebug() << "should be function:" << xml_reader.name() << "name:" << function_attributes.value("name");

	assert(xml_reader.name() == "function");
	function_name = function_attributes.value("name").toString().toStdString();
	qDebug() << "function_name:" << function_name.c_str();

	while (xml_reader.readNextStartElement()) {
		if (xml_reader.name() == "declaration") {
			function_declaration = xml_reader.readElementText().toStdString();
			qDebug() << "function_declaration:" << function_declaration.c_str();
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

bool RPCRunTimeProtocolDescription::openProtocolDescription(std::istream &input) {
#define MAKESURE(X)                                                                                                                                            \
	do {                                                                                                                                                       \
		if (!(X)) {                                                                                                                                            \
			return false;                                                                                                                                      \
		}                                                                                                                                                      \
	} while (0)
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

const std::vector<RPCRuntimeFunction> RPCRunTimeProtocolDescription::get_functions() const {
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
