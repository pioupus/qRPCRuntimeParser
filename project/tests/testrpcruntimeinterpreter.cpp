#include "testrpcruntimeinterpreter.h"
#include "channel_codec_wrapper.h"
#include "rpc_ui.h"
#include "rpcruntime_decoded_function_call.h"
#include "rpcruntime_decoder.h"
#include "rpcruntime_encoded_function_call.h"
#include "rpcruntime_encoder.h"
#include "rpcruntime_protocol_description.h"

#include <QByteArray>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>
#include <fstream>
#include <functional>
#include <vector>

using namespace std::string_literals;

#define RUNTEST 1

void TestRPCRuntimeInterpreter::initTestCase() {
    callBack_FieldID = "";
    callBack_humanReadableName = "";
	callBack_plotIndex = QPair<int, int>(0, 0);
    callBack_timestamp = QDateTime();
    callBack_value = 0;
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt16EnumTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/rpcInt16EnumTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();

	QCOMPARE(funList.size(), 1u);
	QCOMPARE(funList[0].get_function_name(), "rpcFunc_sendUint16AndEnumAndEnum"s);
	QCOMPARE(funList[0].get_request_id(), 2);
	QCOMPARE(funList[0].get_request_parameters().empty(), false);

	QCOMPARE(funList[0].get_request_parameters().size(), 3u);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_name(), "param1"s);
	QCOMPARE(funList[0].get_request_parameters()[1].get_parameter_name(), "param2"s);
	QCOMPARE(funList[0].get_request_parameters()[2].get_parameter_name(), "param3"s);

	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_type(), "uint16_t"s);
	QCOMPARE(funList[0].get_request_parameters()[1].get_parameter_type(), "enum1_t"s);
	QCOMPARE(funList[0].get_request_parameters()[2].get_parameter_type(), "enum2_t"s);

	QCOMPARE(funList[0].get_request_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_request_parameters()[1].get_type(), RPCRuntimeParameterDescription::Type::enumeration);
	QCOMPARE(funList[0].get_request_parameters()[2].get_type(), RPCRuntimeParameterDescription::Type::enumeration);

	QCOMPARE(funList[0].get_request_parameters()[0].get_bit_size(), 16);
	QCOMPARE(funList[0].get_request_parameters()[0].as_integer().is_signed, false);
	QCOMPARE(funList[0].get_request_parameters()[1].get_bit_size(), 8);
	QCOMPARE(funList[0].get_request_parameters()[2].get_bit_size(), 8);

	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_position(), 1);
	QCOMPARE(funList[0].get_request_parameters()[1].get_parameter_position(), 2);
	QCOMPARE(funList[0].get_request_parameters()[2].get_parameter_position(), 3);

	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values.size(), 4u);

	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[0].to_int(), 0);
	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[0].name, "enum1_None"s);

	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[1].to_int(), 1);
	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[1].name, "enum1_A"s);

	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[2].to_int(), 2);
	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[2].name, "enum1_B"s);

	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[3].to_int(), 3);
	QCOMPARE(funList[0].get_request_parameters()[1].as_enumeration().values[3].name, "enum1_C"s);

	QCOMPARE(funList[0].get_request_parameters()[2].as_enumeration().values.size(), 2u);

	QCOMPARE(funList[0].get_request_parameters()[2].as_enumeration().values[0].to_int(), 0);
	QCOMPARE(funList[0].get_request_parameters()[2].as_enumeration().values[0].name, "enum2_A"s);

	QCOMPARE(funList[0].get_request_parameters()[2].as_enumeration().values[1].to_int(), 1);
	QCOMPARE(funList[0].get_request_parameters()[2].as_enumeration().values[1].name, "enum2_B"s);

	QCOMPARE(funList[0].get_reply_parameters().empty(), true);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsignedTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/rpcSignedUnsignedTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	QCOMPARE(funList.size(), 1u);
	QCOMPARE(funList[0].get_request_parameters().size(), 4u);
	QCOMPARE(funList[0].get_request_parameters()[0].as_integer().is_signed, true);
	QCOMPARE(funList[0].get_request_parameters()[1].as_integer().is_signed, true);
	QCOMPARE(funList[0].get_request_parameters()[2].as_integer().is_signed, false);
	QCOMPARE(funList[0].get_request_parameters()[3].as_integer().is_signed, false);

	QCOMPARE(funList[0].get_reply_parameters().empty(), true);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt32ReplyTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/rpcInt32ReplyTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	QCOMPARE(funList.size(), 1u);
	QCOMPARE(funList[0].get_function_name(), "simpleTest"s);
	QCOMPARE(funList[0].get_request_id(), 4);
	QCOMPARE(funList[0].get_request_parameters().size(), 1u);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_name(), "i"s);
	QCOMPARE(funList[0].get_request_parameters()[0].get_bit_size(), 32);
	QCOMPARE(funList[0].get_request_parameters()[0].as_integer().is_signed, true);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_type(), "int32_t"s);
	QCOMPARE(funList[0].get_request_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_reply_parameters().empty(), false);
	QCOMPARE(funList[0].get_reply_id(), 5);
	QCOMPARE(funList[0].get_reply_parameters().size(), 1u);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_name(), "return_value_out"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_bit_size(), 32);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_position(), 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_bit_size(), 32);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_integer().is_signed, true);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInputTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/arrayInputTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	QCOMPARE(funList.size(), 1u);
	QCOMPARE(funList[0].get_function_name(), "arrayInputTest"s);
	QCOMPARE(funList[0].get_request_id(), 10);
	QCOMPARE(funList[0].get_request_parameters().size(), 1u);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_name(), "text_in"s);
	QCOMPARE(funList[0].get_request_parameters()[0].get_bit_size(), 336);
	QCOMPARE(funList[0].get_request_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_request_parameters()[0].as_array().number_of_elements, 42);
	QCOMPARE(funList[0].get_request_parameters()[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_request_parameters()[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::character);
	QCOMPARE(funList[0].get_request_parameters()[0].as_array().type.get_bit_size(), 8);
	QCOMPARE(funList[0].get_request_parameters()[0].as_array().type.get_parameter_type(), "char"s);

	QCOMPARE(funList[0].get_reply_parameters().empty(), true);
	QCOMPARE(funList[0].get_reply_id(), 11);
	QCOMPARE(funList[0].get_reply_parameters().size(), 0u);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcMultiArrayInputTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/multiArrayInputTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	auto &pl = funList[0].get_request_parameters();

	QCOMPARE(funList.size(), 1u);

	QCOMPARE(funList[0].get_function_name(), "multiArrayTest"s);
	QCOMPARE(funList[0].get_request_id(), 8);
	QCOMPARE(pl.size(), 1u);
	QCOMPARE(pl[0].get_parameter_name(), "text_in"s);
	QCOMPARE(pl[0].get_bit_size(), 192);
	QCOMPARE(pl[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(pl[0].as_array().number_of_elements, 2);
	QCOMPARE(pl[0].get_parameter_position(), 1);
	QCOMPARE(pl[0].get_parameter_type(), "char [2][3][4]"s);

	QCOMPARE(pl[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(pl[0].as_array().type.get_bit_size(), 96);
	QCOMPARE(pl[0].as_array().type.get_parameter_type(), "char [3][4]"s);
	QCOMPARE(pl[0].as_array().type.as_array().number_of_elements, 3);

	QCOMPARE(pl[0].as_array().type.as_array().type.get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(pl[0].as_array().type.as_array().type.get_bit_size(), 32);
	QCOMPARE(pl[0].as_array().type.as_array().type.get_parameter_type(), "char [4]"s);
	QCOMPARE(pl[0].as_array().type.as_array().type.as_array().number_of_elements, 4);

	QCOMPARE(pl[0].as_array().type.as_array().type.as_array().type.get_type(), RPCRuntimeParameterDescription::Type::character);
	QCOMPARE(pl[0].as_array().type.as_array().type.as_array().type.get_bit_size(), 8);
	QCOMPARE(pl[0].as_array().type.as_array().type.as_array().type.get_parameter_type(), "char"s);

	QCOMPARE(funList[0].get_reply_parameters().empty(), true);
	QCOMPARE(funList[0].get_reply_id(), 9);
	QCOMPARE(funList[0].get_reply_parameters().size(), 0u);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcStructInputTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/structInputTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();

	QCOMPARE(funList.size(), 1u);

	QCOMPARE(funList[0].get_function_name(), "structInputTest"s);
	QCOMPARE(funList[0].get_request_id(), 22);
	QCOMPARE(funList[0].get_request_parameters().size(), 0u);

	QCOMPARE(funList[0].get_reply_id(), 23);
	QCOMPARE(funList[0].get_reply_parameters().size(), 1u);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_name(), "s_out"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_bit_size(), 64);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::structure);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_bit_size(), 64);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_parameter_type(), "struct TestStruct"s);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members.size(), 4u);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_bit_size(), 32);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_name(), "n1"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].as_integer().is_signed, false);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_type(), "uint32_t"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_bit_size(), 16);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_name(), "n2"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_type(), "int16_t"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_integer().is_signed, true);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_position(), 2);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_type(), RPCRuntimeParameterDescription::Type::character);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_bit_size(), 8);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_name(), "n3"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_type(), "char"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_position(), 3);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].get_bit_size(), 8);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].get_parameter_name(), "n4"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].get_parameter_type(), "uint8_t"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].as_integer().is_signed, false);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[3].get_parameter_position(), 4);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInStructTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/arrayInStructTest.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();

	QCOMPARE(funList.size(), 1u);

	QCOMPARE(funList[0].get_function_name(), "arrayInStructTest"s);
	QCOMPARE(funList[0].get_request_id(), 24);
	QCOMPARE(funList[0].get_request_parameters().size(), 0u);

	QCOMPARE(funList[0].get_reply_id(), 23);
	QCOMPARE(funList[0].get_reply_parameters().size(), 1u);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_name(), "s_inout"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_bit_size(), 400);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::structure);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_bit_size(), 400);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_parameter_type(), "TypedefTestStruct"s);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members.size(), 3u);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_type(), RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_bit_size(), 16);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_name(), "n"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].as_integer().is_signed, false);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_type(), "uint16_t"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_parameter_position(), 1);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_bit_size(), 336);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().number_of_elements, 42);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_name(), "ia"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_type(), "uint8_t [42]"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_parameter_position(), 2);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().type.get_type(),
			 RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().type.get_parameter_type(), "uint8_t"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().type.get_bit_size(), 8);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().type.as_integer().is_signed, false);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().number_of_elements, 42);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_bit_size(), 48);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_name(), "iaa"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_type(), "uint8_t [1][2][3]"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_parameter_position(), 3);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.get_type(),
			 RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.get_parameter_type(), "uint8_t [2][3]"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.get_bit_size(), 48);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().number_of_elements, 2);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.get_type(),
			 RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.get_parameter_type(),
			 "uint8_t [3]"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.get_bit_size(), 24);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.as_array().number_of_elements, 3);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.as_array().type.get_type(),
			 RPCRuntimeParameterDescription::Type::integer);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.as_array().type.get_bit_size(), 8);
	QCOMPARE(
		funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.as_array().type.get_parameter_type(),
		"uint8_t"s);
	QCOMPARE(
		funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().type.as_array().type.as_array().type.as_integer().is_signed,
		false);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcEnumInArrayTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/enumInArray.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	auto &pl = funList[0].get_reply_parameters();

	QCOMPARE(funList.size(), 1u);

	QCOMPARE(funList[0].get_function_name(), "enumInArray"s);
	QCOMPARE(funList[0].get_request_id(), 24);

	QCOMPARE(pl.size(), 1u);
	QCOMPARE(pl[0].get_parameter_name(), "return_value_out"s);
	QCOMPARE(pl[0].get_bit_size(), 32);
	QCOMPARE(pl[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(pl[0].as_array().number_of_elements, 1);
	QCOMPARE(pl[0].get_parameter_position(), 1);
	QCOMPARE(pl[0].get_parameter_type(), "TypedefTestEnum [1]"s);

	QCOMPARE(pl[0].as_array().number_of_elements, 1);
	QCOMPARE(pl[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::enumeration);
	QCOMPARE(pl[0].as_array().type.get_bit_size(), 32);
	QCOMPARE(pl[0].as_array().type.get_parameter_type(), "TypedefTestEnum"s);

	QCOMPARE(pl[0].as_array().type.as_enumeration().values.size(), 4u);

	QCOMPARE(pl[0].as_array().type.as_enumeration().values[0].to_int(), 0);
	QCOMPARE(pl[0].as_array().type.as_enumeration().values[0].name, "TTEa"s);

	QCOMPARE(pl[0].as_array().type.as_enumeration().values[1].to_int(), 1);
	QCOMPARE(pl[0].as_array().type.as_enumeration().values[1].name, "TTEb"s);

	QCOMPARE(pl[0].as_array().type.as_enumeration().values[2].to_int(), 5);
	QCOMPARE(pl[0].as_array().type.as_enumeration().values[2].name, "TTEc"s);

	QCOMPARE(pl[0].as_array().type.as_enumeration().values[3].value, "TTEc + 1"s);
	QCOMPARE(pl[0].as_array().type.as_enumeration().values[3].name, "TTEd"s);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcNegValueInEnumTest() {
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/negValueInEnum.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	auto &funList = rpcinterpreter.get_functions();
	auto &pl = funList[0].get_reply_parameters();

	QCOMPARE(funList.size(), 1u);

	QCOMPARE(funList[0].get_function_name(), "negValueInEnum"s);
	QCOMPARE(funList[0].get_request_id(), 24);
	QCOMPARE(funList[0].get_reply_id(), 23);

	QCOMPARE(pl.size(), 1u);
	QCOMPARE(pl[0].get_parameter_name(), "testEnum"s);

	QCOMPARE(pl[0].get_type(), RPCRuntimeParameterDescription::Type::enumeration);
	QCOMPARE(pl[0].get_bit_size(), 8);
	QCOMPARE(pl[0].get_parameter_type(), "enum TestEnum"s);

	QCOMPARE(pl[0].as_enumeration().values.size(), 3u);

	QCOMPARE(pl[0].as_enumeration().values[0].to_int(), 0);
	QCOMPARE(pl[0].as_enumeration().values[0].name, "TEa"s);

	QCOMPARE(pl[0].as_enumeration().values[1].to_int(), 1);
	QCOMPARE(pl[0].as_enumeration().values[1].name, "TEb"s);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_uint32_t() {
#if RUNTEST
	const uint8_t inBinData_array[] = {0x04, 0x05, 0x10, 0x20, 0x30};
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_uint32_t.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_unsigned_integer(), 0x30201005ull);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_int8_t() {
#if RUNTEST
    const uint8_t inBinData_array[2] = {0x04, 0xF0};
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_int8_t.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_signed_integer(), -16);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_int16_t() {
#if RUNTEST
    const uint8_t inBinData_array[3] = {0x04, 0xF0, 0xFF};
    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_int16_t.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_signed_integer(), -16ll);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_array_char() {
#if RUNTEST
	const uint8_t inBinData_array[43] = {0x06, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_array_char.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_string(), "Hello World!"s);
    QCOMPARE(QString::fromStdString(function_call.get_decoded_parameters()[0].get_field_id()), QString("6.text_inout"));

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int() {
#if RUNTEST
	const uint8_t inBinData_array[] = {0x18, 0x2b, 0x00, 0x48, 0x61, 0x6c, 0x6c, 0x6f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
									   0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x34, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
									   0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x01, 0x11, 0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	auto &funList = rpcinterpreter.get_functions();
	QCOMPARE(funList[0].get_reply_parameters().size(), 1u);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_parameter_name(), "s_inout"s);
	QCOMPARE(funList[0].get_reply_parameters()[0].get_type(), RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);

	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().number_of_elements, 1);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.get_type(), RPCRuntimeParameterDescription::Type::structure);


	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members.size(), 3u);
	QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[0].get_type(), RPCRuntimeParameterDescription::Type::integer);
    QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].get_type(), RPCRuntimeParameterDescription::Type::array);
    QCOMPARE(                       funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[1].as_array().number_of_elements, 42);
    QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].get_type(), RPCRuntimeParameterDescription::Type::array);

    QCOMPARE(funList[0].get_reply_parameters()[0].as_array().type.as_structure().members[2].as_array().number_of_elements, 1);

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct().size(), 3u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[0].type.as_unsigned_integer(), 43ull);

	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.get_desciption()->get_type(),
			 RPCRuntimeParameterDescription::Type::array);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.as_array().size(), 42u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.as_array()[0].as_unsigned_integer(), 0x48ull);
    QCOMPARE(                       function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.as_string(),
			 "\x48\x61\x6c\x6c\x6f\x33\x34\x35\x36\x37\x38\x39\x30\x31\x32\x33\x34\x35\x36\x37\x34\x38\x39\x30\x31\x32\x33"
			 "\x34\x35\x36\x37\x38\x39\x30\x31\x32\x33\x34\x35\x36\x37\x38"s);

    QCOMPARE(QString::fromStdString(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.get_field_id()), QString("24.s_inout.0.ia"));

	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[1].type.as_array()[0].get_desciption()->get_type(),
			 RPCRuntimeParameterDescription::Type::integer);

	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array().size(), 2u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[0].as_array().size(), 3u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[0].as_array()[0].as_unsigned_integer(),
			 0x00ull);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[0].as_array()[1].as_unsigned_integer(),
			 0x10ull);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[0].as_array()[2].as_unsigned_integer(),
			 0x20ull);

	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[1].as_array()[0].as_unsigned_integer(),
			 0x01ull);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[1].as_array()[1].as_unsigned_integer(),
			 0x11ull);
    QCOMPARE(                       function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[1].as_array()[2].as_unsigned_integer(),
			 0x21ull);
    QCOMPARE(QString::fromStdString(function_call.get_decoded_parameters()[0].as_array()[0].as_struct()[2].type.as_array()[0].as_array()[1].as_array()[2].get_field_id()), QString("24.s_inout.0.iaa.0.1.2"));
#endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_access_by_field_id() {
#if RUNTEST
    const uint8_t inBinData_array[] = {0x18, 0x2b, 0x00, 0x48, 0x61, 0x6c, 0x6c, 0x6f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
                                       0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x34, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                                       0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x01, 0x11, 0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    {
        std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
        QVERIFY(static_cast<bool>(xmlfile));
        bool result = rpcinterpreter.openProtocolDescription(xmlfile);
        QVERIFY(result);
    }

    RPCRuntimeDecoder decoder(rpcinterpreter);
    RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

    QCOMPARE(transfer.is_complete(), true);

    RPCRuntimeDecodedFunctionCall function_call = transfer.decode();
    QVERIFY(function_call.get_param_by_field_id("24.s_inout.0.iaa.0.1.2").get());
    QCOMPARE(function_call.get_param_by_field_id("24.s_inout.0.iaa.0.1.2")->as_unsigned_integer(),       0x21ull);
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout.0.ia_.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("30.s_inout.0.ia_.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout_.0.ia_.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout.1.ia_.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout.1_.ia_.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout.1_.iaa.0.1.2").get());
    QVERIFY(!function_call.get_param_by_field_id("24.s_inout._1_.iaa.0.1.2").get());
#endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_treewidgetreport() {
#if RUNTEST
	const uint8_t inBinData_array[] = {0x18, 0x2b, 0x00, 0x48, 0x61, 0x6c, 0x6c, 0x6f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
									   0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x34, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
									   0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x01, 0x11, 0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	auto items = getTreeWidgetReport(function_call);

	auto function = items.get();
	QCOMPARE(function->text(0), QString("typedefStructTest"));
	QCOMPARE(function->text(1), QString("RPC_UART_RESULT typedefStructTest(TypedefTestStruct s_inout[1]);"));
	QCOMPARE(function->childCount(), 1);

	auto request = function->child(0);
	QCOMPARE(request->text(0), QString("Request"));
	QCOMPARE(request->text(1), QString(""));
	QCOMPARE(request->childCount(), 1);

	auto array = request->child(0);
	qDebug() << "array: " << array->data(1, Qt::UserRole).toString();
	QCOMPARE(array->data(0, Qt::UserRole).toString(), QString("array"));
    QCOMPARE(array->data(2, Qt::UserRole).toString(), QString("24.s_inout"));
	QCOMPARE(array->childCount(), 1);
	//QCOMPARE(array->data(0, Qt::UserRole).toString(), QString("scripts/decodeTest_struct_int.xml?24?0"));

	auto structure = array->child(0); //TypedefTestStruct s_inout
	qDebug() << "struct: " << structure->data(1, Qt::UserRole).toString();
	QCOMPARE(structure->data(0, Qt::UserRole).toString(), QString("structure"));
    QCOMPARE(structure->data(2, Qt::UserRole).toString(), QString("24.s_inout.0"));
	QCOMPARE(structure->childCount(), 3);

	auto parameter_n = structure->child(0);
	qDebug() << "parameter_n: " << parameter_n->data(1, Qt::UserRole).toString();
    QCOMPARE(parameter_n->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.n"));
	QCOMPARE(parameter_n->text(0), QString("n(uint16_t)"));
	QCOMPARE(parameter_n->text(1), QString("43"));
	QCOMPARE(parameter_n->childCount(), 0);

	auto parameter_ia = structure->child(1);
	qDebug() << "parameter_ia:" << parameter_ia->data(1, Qt::UserRole).toString();
    QCOMPARE(parameter_ia->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.ia"));
	QCOMPARE(parameter_ia->text(0), QString("ia(uint8_t [42])"));
	QCOMPARE(parameter_ia->text(1), QString("0x48 0x61 0x6C 0x6C 0x6F 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x34 0x38 "
											"0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38"));

	auto parameter_iaa = structure->child(2);
    QCOMPARE(parameter_iaa->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.iaa"));
	QCOMPARE(parameter_iaa->childCount(), 1);
	QCOMPARE(parameter_iaa->text(0), QString("iaa(uint8_t [1][2][3])"));
	QCOMPARE(parameter_iaa->text(1).trimmed(), QString("").trimmed());

	auto iaa_0 = parameter_iaa->child(0);
	QCOMPARE(iaa_0->childCount(), 2);
    QCOMPARE(iaa_0->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.iaa.0"));
	//QCOMPARE(iaa_1->data(0, Qt::UserRole).toString(), QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0"));
	QCOMPARE(iaa_0->text(0), QString("[0]"));
	QCOMPARE(iaa_0->text(1).trimmed(), QString("").trimmed());

	auto iaa_0_0 = iaa_0->child(0);
	QCOMPARE(iaa_0_0->childCount(), 0);
    QCOMPARE(iaa_0_0->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.iaa.0.0"));
	//QCOMPARE(iaa_0_0->data(0, Qt::UserRole).toString(), QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?0"));
	QCOMPARE(iaa_0_0->text(0), QString("[0]"));
	QCOMPARE(iaa_0_0->text(1).trimmed(), QString("0x00 0x10 0x20").trimmed());

	auto iaa_0_1 = iaa_0->child(1);
	QCOMPARE(iaa_0_1->childCount(), 0);
    QCOMPARE(iaa_0_1->data(2, Qt::UserRole).toString(), QString("24.s_inout.0.iaa.0.1"));
	//QCOMPARE(iaa_0_1->data(0, Qt::UserRole).toString(), QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1"));
	QCOMPARE(iaa_0_1->text(0), QString("[1]"));
	QCOMPARE(iaa_0_1->text(1).trimmed(), QString("0x01 0x11 0x21").trimmed());

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsingedBug() {
#if 0
#if RUNTEST || 1
	const uint8_t inBinData_array[] = {0xFF, 0xFF, 0xFF, 0x04, 0x02, 0x52, 0x00, 0x9A, 0x00, 0x50, 0x00, 0x3C, 0x00, 0xE3, 0x4D};

    RPCRunTimeProtocolDescription rpcinterpreter;



	{
		std::ifstream xmlfile{"scripts/rpcSignedUnsingedBug.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 4u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_integer(), 82ll);
	QCOMPARE(function_call.get_decoded_parameters()[1].as_integer(), 410ll);
	QCOMPARE(function_call.get_decoded_parameters()[2].as_integer(), 80ll);
	QCOMPARE(function_call.get_decoded_parameters()[3].as_integer(), 60ll);

#endif
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_enum() {
#if RUNTEST
	const uint8_t inBinData_array[] = {0x1a, 0x01};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_enum.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_enum().name, "TEb"s);
	QCOMPARE(function_call.get_decoded_parameters()[0].as_enum().value, 1);

#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_report() {
#if RUNTEST
	const uint8_t inBinData_array[] = {0x18, 0x2b, 0x00, 0x48, 0x61, 0x6c, 0x6c, 0x6f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
									   0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x34, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
									   0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x01, 0x11, 0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);

	QStringList report = get_report(function_call);

    QFile inFile("scripts/decodeTest_struct_int_report_mask.txt");

    QFile outfile("scripts/decodeTest_struct_int_report_output.txt");
    outfile.open(QIODevice::WriteOnly);
    inFile.open(QIODevice::ReadOnly);
	QTextStream out(&outfile);    // we will serialize the data into the file
	QTextStream in_mask(&inFile); // we will serialize the data into the file

    QStringList in;
	QString line_mask;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

	for (int i = 0; i < report.count(); i++) {
		out << report[i] + '\n';
    }
	QCOMPARE(in.count() - 1, report.count());
	for (int i = 0; i < report.count(); i++) {
		QCOMPARE(in[i], report[i]);
    }
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_enum_report() {
#if RUNTEST
    const uint8_t inBinData_array[] = {0x1a, 0x01};

    RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/decodeTest_enum.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
    RPCRuntimeDecoder decoder(rpcinterpreter);
	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();

	QStringList report = get_report(function_call);
    QFile inFile("scripts/decodeTest_enum_report_mask.txt");

    QFile outfile("scripts/decodeTest_enum_report_output.txt");
    outfile.open(QIODevice::WriteOnly);
    inFile.open(QIODevice::ReadOnly);
	QTextStream out(&outfile);    // we will serialize the data into the file
	QTextStream in_mask(&inFile); // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

	for (int i = 0; i < report.count(); i++) {
		out << report[i] + '\n';
    }
	QCOMPARE(in.count() - 1, report.count());
	for (int i = 0; i < report.count(); i++) {
		QCOMPARE(in[i], report[i]);
    }
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_enum_report() {
#if RUNTEST
    const uint8_t inBinData_array[] = {'J', 'U', 'N', 'K', 0xff, 0xff, 0xff, 0x00, 0x1a, 0x01, 0x96, 0xe1, 'J', 'U', 'N', 'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_enum.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	RPCRuntimeDecoder decoder(rpcinterpreter);
	Channel_codec_wrapper cc(decoder);

	cc.add_data(inBinData_array);

	QVERIFY(cc.transfer_complete());

	auto function_call = cc.pop();

	QStringList report = get_report(function_call);
    QFile inFile("scripts/decodeTest_enum_report_mask.txt");
    inFile.open(QIODevice::ReadOnly);
	QTextStream in_mask(&inFile); // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

	QCOMPARE(in.count() - 1, report.count());
	for (int i = 0; i < report.count(); i++) {
		QCOMPARE(in[i], report[i]);
    }
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_struct_int_report() {
#if RUNTEST
	const uint8_t inBinData_array[] = {'J',  'U',  'N',  'K',  0xff, 0xff, 0xff, 0x74, 0x18, 0x2b, 0x00, 0x48, 0x60, 0x6c, 0x6c, 0x6f, 0x32,
									   0x34, 0x34, 0x36, 0x36, 0x38, 0x1f, 0x38, 0x30, 0x30, 0x32, 0x32, 0x34, 0x34, 0x36, 0x36, 0x34, 0x38,
									   0x39, 0x30, 0x31, 0x00, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34,
									   0x35, 0x18, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x00, 0x11, 0x20, 0x93, 0x31, 'J',  'U',  'N',  'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	Channel_codec_wrapper cc(decoder);

	cc.add_data(inBinData_array);

	QVERIFY(cc.transfer_complete());

	auto function_call = cc.pop();

	QCOMPARE(function_call.get_decoded_parameters().size(), 1u);
	//QCOMPARE(decoder.getErrorCRCHappened(), false);
	QStringList report = get_report(function_call);

    QFile inFile("scripts/decodeTest_struct_int_report_mask.txt");

    inFile.open(QIODevice::ReadOnly);
	QTextStream in_mask(&inFile); // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

	QCOMPARE(in.count() - 1, report.count());
	for (int i = 0; i < report.count(); i++) {
		QCOMPARE(in[i], report[i]);
    }
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_WrongCRC() {
#if RUNTEST
	const uint8_t inBinData_array[] = {'J',  'U',  'N',  'K',  0xff, 0xff, 0xff, 0x74, 0x18, 0x2b, 0x00, 0x48, 0x60, 0x6c, 0x6c, 0x6f, 0x32,
									   0x34, 0x34, 0x36, 0x36, 0x38, 0x1f, 0x38, 0x30, 0x30, 0x32, 0x32, 0x34, 0x34, 0x36, 0x36, 0x34, 0x38,
									   0x39, 0x30, 0x31, 0x00, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34,
									   0x35, 0x18, 0x36, 0x37, 0x38, 0x00, 0x10, 0x20, 0x00, 0x11, 0x20, 0x93, 0x30, 'J',  'U',  'N',  'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_struct_int.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

    RPCRuntimeDecoder decoder(rpcinterpreter);
	Channel_codec_wrapper cc(decoder);

	cc.add_data(inBinData_array);

	QVERIFY(!cc.transfer_complete());

//auto function_call = cc.pop();

//QCOMPARE(decoder.getErrorCRCHappened(), true);
#endif
}

#if 0
void TestRPCRuntimeInterpreter::playWithChannelEncoding() {
#if RUNTEST
    const uint8_t inBinData_array[] = {0x1a, 0x01};


    RPCRunTimeProtocolDescription rpcinterpreter;
    rpcinterpreter.openProtocolDescription("scripts/decodeTest_enum.xml");

    RPCRuntimeDecoder decoder(rpcinterpreter);

    QByteArray outBinData = decoder.encodeToChannelCodedData(inBinData);

    QFile outfile("scripts/channelEncodedData.txt");
    outfile.open(QIODevice::WriteOnly);
	QTextStream out(&outfile); // we will serialize the data into the file
    QString line("{");
	for (int i = 0; i < outBinData.count() - 1; i++) {
		line += "0x%1, ").arg((uint8_t)outBinData[i], 2, 16, QChar('0'));
		if (line.length() > 100) {
			out << line + "\n";
            line = "";
        }
    }
	line += "0x%1}").arg((uint8_t)outBinData[outBinData.count() - 1], 2, 16, QChar('0'));
    out << line;
#endif
}
#endif

static QByteArray QB(const std::vector<unsigned char> data) {
	return {reinterpret_cast<const char *>(data.data()), static_cast<int>(data.size())};
}

template <int size>
static QByteArray QB(const unsigned char (&data)[size]) {
	return {reinterpret_cast<const char *>(data), size};
}

static QByteArray QB(const unsigned char *data, int size) {
	(void)(QByteArray(*)(const unsigned char *data, int size)) QB;
	return {reinterpret_cast<const char *>(data), size};
}

void TestRPCRuntimeInterpreter::create_request_without_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_without_parameters.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("emptyTest");

	QVERIFY(function_call.are_all_values_set());

	const uint8_t outBinData_array[] = {0x04};
	auto data = function_call.encode();

	QCOMPARE(sizeof outBinData_array, data.size());
	QCOMPARE(QB(outBinData_array), QB(data));
}

void TestRPCRuntimeInterpreter::create_request_with_int_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_with_int_parameter.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("intTest");
	function_call.get_parameter(0).set_value(1234567890);

	QVERIFY(function_call.are_all_values_set());

	const uint8_t outBinData_array[] = {0x04, 0xD2, 0x02, 0x96, 0x49};
	auto data = function_call.encode();

	QCOMPARE(sizeof outBinData_array, data.size());
	QCOMPARE(QB(outBinData_array), QB(data));
}

void TestRPCRuntimeInterpreter::create_request_with_multiple_int_parameters() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_with_multiple_int_parameters.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("multiIntTest");
	function_call.get_parameter(0).set_value(1234567890);

	QVERIFY(function_call.are_all_values_set() == false);

	function_call.get_parameter("i").set_value(987654321);

	QVERIFY(function_call.are_all_values_set() == false);

	function_call.get_parameter("j").set_value(0xabcd);

	QVERIFY(function_call.are_all_values_set());

	const uint8_t outBinData_array[] = {0x10, 0xB1, 0x68, 0xDE, 0x3A, 0xCD, 0xAB};
	auto data = function_call.encode();

	QCOMPARE(sizeof outBinData_array, data.size());
	QCOMPARE(QB(outBinData_array), QB(data));
}

void TestRPCRuntimeInterpreter::create_request_with_enum_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_with_enum_parameter.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("enumTest1");
	function_call.get_parameter("testEnum").set_value("TEb");

	QVERIFY(function_call.are_all_values_set());

	{
		const uint8_t outBinData_array[] = {0x1A, 0x01};
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}

	function_call.get_parameter("testEnum").set_value(42);

	QVERIFY(function_call.are_all_values_set());

	{
		const uint8_t outBinData_array[] = {0x1A, 0x2A};
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}
}

void TestRPCRuntimeInterpreter::create_request_with_array_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_with_char_array_parameter.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("arrayTest");

	{
		function_call.get_parameter("text_inout").set_value("Hello world!");
		QVERIFY(function_call.are_all_values_set());
		const uint8_t outBinData_array[43] =
			"\x06"
			"Hello world!";
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}

	{
		function_call.get_parameter("text_inout").get_parameter(6).set_value('W');
		function_call.get_parameter("text_inout").get_parameter(11).set_value('?');
		QVERIFY(function_call.are_all_values_set());
		const uint8_t outBinData_array[43] =
			"\x06"
			"Hello World?";
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}
}

void TestRPCRuntimeInterpreter::create_request_with_struct_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/create_with_struct_parameter.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	RPCRuntimeEncodedFunctionCall function_call = encoder.encode("structInputTest");

	{
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_in").get_parameter(0).get_parameter("n1").set_value(1234567890);
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_in").get_parameter(0).get_parameter("n2").set_value(0xABCD);
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_in").get_parameter(0).get_parameter("n3").set_value(42);
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_in").get_parameter(0).get_parameter("n4").set_value(15);
		QVERIFY(function_call.are_all_values_set());
		const uint8_t outBinData_array[9] = {0x16, 0xD2, 0x02, 0x96, 0x49, 0xCD, 0xAB, 0x2A, 0x0F};
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}
}

void TestRPCRuntimeInterpreter::create_request_with_complex_parameter() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	{
		std::ifstream xmlfile{"scripts/RPC_UART_Server.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}
	RPCRuntimeEncoder encoder(rpcinterpreter);

	{
		RPCRuntimeEncodedFunctionCall function_call = encoder.encode("typedefStructTest");
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_inout")[0]["n"] = 0xABCD;
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_inout")[0]["ia"] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
														   22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_inout")[0]["iaa"][0][0] = {1, 2, 3};
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("s_inout")[0]["iaa"][0][1] = {44, 55, 66};
		QVERIFY(function_call.are_all_values_set());
		const uint8_t outBinData_array[51] = {0x18, 0xCD, 0xAB, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
											  24,   25,   26,   27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 1,  2,  3,  44, 55, 66};
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}

	{
		RPCRuntimeEncodedFunctionCall function_call = encoder.encode("enumTest1");
		QVERIFY(!function_call.are_all_values_set());
		function_call.get_parameter("testEnum") = "TEb";
		QVERIFY(function_call.are_all_values_set());
		const uint8_t outBinData_array[2] = {0x1A, 1};
		auto data = function_call.encode();

		QCOMPARE(sizeof outBinData_array, data.size());
		QCOMPARE(QB(outBinData_array), QB(data));
	}
}

void TestRPCRuntimeInterpreter::encode_hash_request_without_protocol() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	RPCRuntimeEncoder encoder(rpcinterpreter);
	auto data = encoder.encode(0).encode();
	QCOMPARE(data.size(), 1u);
	QCOMPARE(static_cast<int>(data[0]), 0);
}

void TestRPCRuntimeInterpreter::decode_hash_request_without_protocol() {
	RPCRunTimeProtocolDescription rpcinterpreter;
	RPCRuntimeDecoder decoder(rpcinterpreter);
	const unsigned char data[] = {0};
	RPCRuntimeTransfer transfer = decoder.decode(data);
	QVERIFY(transfer.is_complete());
	RPCRuntimeDecodedFunctionCall function_call = transfer.decode();
	QCOMPARE(function_call.get_id(), 0);
	QCOMPARE(function_call.get_decoded_parameters().size(), 0u);
}

void TestRPCRuntimeInterpreter::create_callback() {
	const uint8_t inBinData_array[3] = {0x04, 0xF0, 0xFF};
	RPCRunTimeProtocolDescription rpcinterpreter;

	{
		std::ifstream xmlfile{"scripts/decodeTest_int16_t.xml"};
		QVERIFY(static_cast<bool>(xmlfile));
		bool result = rpcinterpreter.openProtocolDescription(xmlfile);
		QVERIFY(result);
	}

	RPCRuntimeDecoder decoder(rpcinterpreter);

	int value = -1;
	decoder.set_reply_callback(rpcinterpreter.get_function("simpleTest"), [&value](const RPCRuntimeDecodedFunctionCall &reply) {
		const auto &parameters = reply.get_decoded_parameters();
		QCOMPARE(parameters.size(), 1u);
		value = parameters[0].as_integer();
	});

	RPCRuntimeTransfer transfer = decoder.decode(inBinData_array);

	QCOMPARE(transfer.is_complete(), true);
	QCOMPARE(transfer.get_min_number_of_bytes(), static_cast<int>(sizeof inBinData_array));

	transfer.decode(); //without decoding the function is not called

	QCOMPARE(value, -16);
}
