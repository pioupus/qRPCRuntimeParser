#include "rpcruntime_protocol_description.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <cassert>
#include <fstream>
#include <string>

static RPCRuntimeFunction parse_function(QXmlStreamReader &xml_reader) {
	RPCRuntimeFunction function;
	assert(xml_reader.name() == "function");
	const auto &rpc_attributes = xml_reader.attributes();
	//TODO: add all the proper attributes and put xml_reader in a state that it reads the next function next
	return function;
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

	if (xml_reader.hasError()) { //if openProtocolDescription returns false put a breakpoint here to see the error message
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
