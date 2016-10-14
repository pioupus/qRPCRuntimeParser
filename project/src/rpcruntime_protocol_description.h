#ifndef RPCRUNTIMEPROTOCOLDESCRIPTION_H
#define RPCRUNTIMEPROTOCOLDESCRIPTION_H

#include "rpcruntime_function.h"

#include <istream>
#include <vector>

/*
 * A RPCRunTimeProtocolDescription describes the Protocol created by the RPC-Generator loaded from an XML-File
*/

class RPCRunTimeProtocolDescription {
	public:
	RPCRunTimeProtocolDescription();
	RPCRunTimeProtocolDescription(std::istream &input);
	bool openProtocolDescription(std::istream &input);
	const std::vector<RPCRuntimeFunction> &get_functions() const;
	const std::string &get_hash() const;
	const std::string &get_project_name() const;
	int get_version_number() const;
	int get_command_id_start() const;
	const std::vector<RPCRuntimeParameterDescription> &get_parameters(int id) const;
	int get_parameter_size_bytes(int id) const;
	const RPCRuntimeFunction &get_function(int id) const;
	const RPCRuntimeFunction &get_function(const std::string &name) const;

	private:
	bool open_description(std::istream &input);
	void reset();
	std::vector<RPCRuntimeFunction> functions;
	std::string hash;
	std::string project_name;
	int version_number = -1;
	int command_id_start = -1;
};

#endif // RPCRUNTIMEINTERPRETER_H
