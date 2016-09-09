#include "rpcruntime_protocol_description.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <cassert>
#include <fstream>
#include <string>

#if 0
static RPCRuntimeParameterDescription parse_parameter(QXmlStreamReader &xml_reader){
	//TODO
	return {};
}
#endif

#if 0
static std::vector<RPCRuntimeParameterDescription> parse_parameters(QXmlStreamReader &xml_reader){

}
#endif

static std::ofstream debugoutput;

static RPCRuntimeFunction parse_function(QXmlStreamReader &xml_reader) {
	int request_id = -1;
	int reply_id = -1;
	std::vector<RPCRuntimeParameterDescription> request_parameters;
	std::vector<RPCRuntimeParameterDescription> reply_parameters;
	std::string function_name;
	std::string function_declaration;

	assert(xml_reader.name() == "function");
	const auto &function_attributes = xml_reader.attributes();
	function_name = function_attributes.value("name").toString().toStdString();
	debugoutput << "function_name: " << function_name << '\n';

	while (xml_reader.readNextStartElement()) {
		if (xml_reader.name() == "declaration") {
			function_declaration = xml_reader.readElementText().toStdString();
			debugoutput << "function_declaration: " << function_declaration << '\n';
		} else if (xml_reader.name() == "request") {
			bool ok;
			request_id = xml_reader.attributes().value("ID").toInt(&ok);
			if (!ok){
				request_id = -1;
			}
			// TODO: parse request
			debugoutput << "TODO: parse request\n";
			xml_reader.skipCurrentElement();
		} else if (xml_reader.name() == "reply") {
			bool ok;
			reply_id = xml_reader.attributes().value("ID").toInt(&ok);
			if (!ok){
				reply_id = -1;
			}
			// TODO: parse reply
			debugoutput << "TODO: parse reply\n";
			xml_reader.skipCurrentElement();
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
	debugoutput.open("C:/TEMP/debug_output.txt");
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
