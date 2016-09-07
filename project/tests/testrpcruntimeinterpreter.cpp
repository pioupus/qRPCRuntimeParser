#include <QStringList>
#include <QByteArray>
#include <QPair>
#include <QString>
#include <functional>
#include "testrpcruntimeinterpreter.h"
#include "rpcruntime_protocol_description.h"
#include "rpcruntime_decoder.h"
#include <QTreeWidgetItem>


#define RUNTEST 1

void TestRPCRuntimeInterpreter::initTestCase(){
    callBack_FieldID = "";
    callBack_humanReadableName = "";
    callBack_plotIndex =  QPair<int,int>(0,0);
    callBack_timestamp = QDateTime();
    callBack_value = 0;
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt16EnumTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcInt16EnumTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();

    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].name, QString("rpcFunc_sendUint16AndEnumAndEnum"));
    QCOMPARE(funList[0].request.ID, 2);
    QCOMPARE(funList[0].request.isNull(), false);

    QCOMPARE(funList[0].request.paramList.count(), 3);
    QCOMPARE(funList[0].request.paramList[0].name, QString("param1"));
    QCOMPARE(funList[0].request.paramList[1].name, QString("param2"));
    QCOMPARE(funList[0].request.paramList[2].name, QString("param3"));

    QCOMPARE(funList[0].request.paramList[0].elementBitLength, 16);
    QCOMPARE(funList[0].request.paramList[0].isSigned, false);
    QCOMPARE(funList[0].request.paramList[1].elementBitLength, 8);
    QCOMPARE(funList[0].request.paramList[2].elementBitLength, 8);

    QCOMPARE(funList[0].request.paramList[0].typeName, QString("uint16_t"));
    QCOMPARE(funList[0].request.paramList[1].typeName, QString("enum1_t"));
    QCOMPARE(funList[0].request.paramList[2].typeName, QString("enum2_t"));

    QCOMPARE(funList[0].request.paramList[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].request.paramList[1].rpcParamType, RPCParamType_t::param_enum);
    QCOMPARE(funList[0].request.paramList[2].rpcParamType, RPCParamType_t::param_enum);

    QCOMPARE(funList[0].request.paramList[0].indexPosition, 1);
    QCOMPARE(funList[0].request.paramList[1].indexPosition, 2);
    QCOMPARE(funList[0].request.paramList[2].indexPosition, 3);


    QCOMPARE(funList[0].request.paramList[1].enumValues.count(), 4);

    QCOMPARE(funList[0].request.paramList[1].enumValues[0].first, 0);
    QCOMPARE(funList[0].request.paramList[1].enumValues[0].second, QString("enum1_None"));

    QCOMPARE(funList[0].request.paramList[1].enumValues[1].first, 1);
    QCOMPARE(funList[0].request.paramList[1].enumValues[1].second, QString("enum1_A"));

    QCOMPARE(funList[0].request.paramList[1].enumValues[2].first, 2);
    QCOMPARE(funList[0].request.paramList[1].enumValues[2].second, QString("enum1_B"));

    QCOMPARE(funList[0].request.paramList[1].enumValues[3].first, 3);
    QCOMPARE(funList[0].request.paramList[1].enumValues[3].second, QString("enum1_C"));

    QCOMPARE(funList[0].request.paramList[2].enumValues.count(), 2);

    QCOMPARE(funList[0].request.paramList[2].enumValues[0].first, 0);
    QCOMPARE(funList[0].request.paramList[2].enumValues[0].second, QString("enum2_A"));

    QCOMPARE(funList[0].request.paramList[2].enumValues[1].first, 1);
    QCOMPARE(funList[0].request.paramList[2].enumValues[1].second, QString("enum2_B"));

    QCOMPARE(funList[0].reply.isNull(), true);
#endif
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsignedTest()
{
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcSignedUnsignedTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].request.paramList.count(), 4);
    QCOMPARE(funList[0].request.paramList[0].isSigned, true);
    QCOMPARE(funList[0].request.paramList[1].isSigned, true);
    QCOMPARE(funList[0].request.paramList[2].isSigned, false);
    QCOMPARE(funList[0].request.paramList[3].isSigned, false);

    QCOMPARE(funList[0].reply.isNull(), true);
#endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt32ReplyTest()
{
#if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcInt32ReplyTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].name, QString("simpleTest"));
    QCOMPARE(funList[0].request.ID, 4);
    QCOMPARE(funList[0].request.paramList.count(), 1);
    QCOMPARE(funList[0].request.paramList[0].name, QString("i"));
    QCOMPARE(funList[0].request.paramList[0].elementBitLength, 32);
    QCOMPARE(funList[0].request.paramList[0].isSigned, true);
    QCOMPARE(funList[0].request.paramList[0].typeName, QString("int32_t"));
    QCOMPARE(funList[0].request.paramList[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].request.paramList[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.isNull(), false);
    QCOMPARE(funList[0].reply.ID, 5);
    QCOMPARE(funList[0].reply.paramList.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].name, QString("return_value_out"));
    QCOMPARE(funList[0].reply.paramList[0].elementBitLength, 32);
    QCOMPARE(funList[0].reply.paramList[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].indexPosition, 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].elementBitLength, 32);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].isSigned, true);
#endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInputTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/arrayInputTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].name, QString("arrayInputTest"));
    QCOMPARE(funList[0].request.ID, 10);
    QCOMPARE(funList[0].request.paramList.count(), 1);
    QCOMPARE(funList[0].request.paramList[0].name, QString("text_in"));
    QCOMPARE(funList[0].request.paramList[0].elementBitLength, 336);
    QCOMPARE(funList[0].request.paramList[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].request.paramList[0].elementCount, 42);
    QCOMPARE(funList[0].request.paramList[0].indexPosition, 1);

    QCOMPARE(funList[0].request.paramList[0].subParameters.count(), 1);
    QCOMPARE(funList[0].request.paramList[0].subParameters[0].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(funList[0].request.paramList[0].subParameters[0].elementBitLength, 8);
    QCOMPARE(funList[0].request.paramList[0].subParameters[0].typeName, QString("char"));

    QCOMPARE(funList[0].reply.isNull(), false);
    QCOMPARE(funList[0].reply.ID, 11);
    QCOMPARE(funList[0].reply.paramList.count(), 0);
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcMultiArrayInputTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/multiArrayInputTest.xml");
    QCOMPARE(result, true);


    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    auto &pl = funList[0].request.paramList;

    QCOMPARE(funList.count(), 1);

    QCOMPARE(funList[0].name, QString("multiArrayTest"));
    QCOMPARE(funList[0].request.ID, 8);
    QCOMPARE(pl.count(), 1);
    QCOMPARE(pl[0].name, QString("text_in"));
    QCOMPARE(pl[0].elementBitLength, 192);
    QCOMPARE(pl[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].elementCount, 2);
    QCOMPARE(pl[0].indexPosition, 1);
    QCOMPARE(pl[0].typeName, QString("char [2][3][4]"));

    QCOMPARE(pl[0].subParameters.count(), 1);

    QCOMPARE(pl[0].subParameters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].subParameters[0].elementBitLength, 96);
    QCOMPARE(pl[0].subParameters[0].typeName, QString("char [3][4]"));
    QCOMPARE(pl[0].subParameters[0].elementCount, 3);

    QCOMPARE(pl[0].subParameters[0].subParameters.count(), 1);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].elementBitLength, 32);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].typeName, QString("char [4]"));
    QCOMPARE(pl[0].subParameters[0].subParameters[0].elementCount, 4);

    QCOMPARE(pl[0].subParameters[0].subParameters[0].subParameters.count(), 1);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].subParameters[0].elementBitLength, 8);
    QCOMPARE(pl[0].subParameters[0].subParameters[0].subParameters[0].typeName, QString("char"));
    QCOMPARE(pl[0].subParameters[0].subParameters[0].subParameters[0].elementCount, 1);

    QCOMPARE(funList[0].reply.isNull(), false);
    QCOMPARE(funList[0].reply.ID, 9);
    QCOMPARE(funList[0].reply.paramList.count(), 0);
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcStructInputTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/structInputTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();

    QCOMPARE(funList.count(), 1);

    QCOMPARE(funList[0].name, QString("structInputTest"));
    QCOMPARE(funList[0].request.ID, 22);
    QCOMPARE(funList[0].request.paramList.count(), 0);


    QCOMPARE(funList[0].reply.ID, 23);
    QCOMPARE(funList[0].reply.paramList.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].name, QString("s_out"));
    QCOMPARE(funList[0].reply.paramList[0].elementBitLength, 64);
    QCOMPARE(funList[0].reply.paramList[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].elementCount, 1);
    QCOMPARE(funList[0].reply.paramList[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].rpcParamType, RPCParamType_t::param_struct);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].elementBitLength, 64);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].typeName, QString("struct TestStruct"));

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters.count(), 4);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].elementBitLength, 32);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].name, QString("n1"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].typeName, QString("uint32_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].elementBitLength, 16);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].name, QString("n2"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].typeName, QString("int16_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].isSigned, true);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].indexPosition, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].name, QString("n3"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].typeName, QString("char"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].indexPosition, 3);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].name, QString("n4"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[3].indexPosition, 4);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInStructTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/arrayInStructTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();

    QCOMPARE(funList.count(), 1);

    QCOMPARE(funList[0].name, QString("arrayInStructTest"));
    QCOMPARE(funList[0].request.ID, 24);
    QCOMPARE(funList[0].request.paramList.count(), 0);


    QCOMPARE(funList[0].reply.ID, 23);
    QCOMPARE(funList[0].reply.paramList.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].name, QString("s_inout"));
    QCOMPARE(funList[0].reply.paramList[0].elementBitLength, 400);
    QCOMPARE(funList[0].reply.paramList[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].elementCount, 1);
    QCOMPARE(funList[0].reply.paramList[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].rpcParamType, RPCParamType_t::param_struct);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].elementBitLength, 400);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].typeName, QString("TypedefTestStruct"));

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters.count(), 3);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].elementBitLength, 16);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].name, QString("n"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].typeName, QString("uint16_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].elementBitLength, 336);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].elementCount, 42);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].name, QString("ia"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].typeName, QString("uint8_t [42]"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].indexPosition, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters[0].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters[0].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].subParameters[0].elementCount, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].elementBitLength, 48);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].elementCount, 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].name, QString("iaa"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].typeName, QString("uint8_t [1][2][3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].indexPosition, 3);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].typeName, QString("uint8_t [2][3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].elementBitLength, 48);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].elementCount, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].typeName, QString("uint8_t [3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].elementBitLength, 24);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].elementCount, 3);


    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].subParameters[0].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].subParameters[0].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].subParameters[0].subParameters[0].subParameters[0].isSigned, false);



    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcEnumInArrayTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/enumInArray.xml");
    QCOMPARE(result, true);


    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    auto &pl = funList[0].reply.paramList;

    QCOMPARE(funList.count(), 1);

    QCOMPARE(funList[0].name, QString("enumInArray"));
    QCOMPARE(funList[0].request.ID, 24);

    QCOMPARE(pl.count(), 1);
    QCOMPARE(pl[0].name, QString("return_value_out"));
    QCOMPARE(pl[0].elementBitLength, 32);
    QCOMPARE(pl[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].elementCount, 1);
    QCOMPARE(pl[0].indexPosition, 1);
    QCOMPARE(pl[0].typeName, QString("TypedefTestEnum [1]"));





    QCOMPARE(pl[0].subParameters.count(), 1);
    QCOMPARE(pl[0].subParameters[0].rpcParamType, RPCParamType_t::param_enum);
    QCOMPARE(pl[0].subParameters[0].elementBitLength, 32);
    QCOMPARE(pl[0].subParameters[0].typeName, QString("TypedefTestEnum"));
    QCOMPARE(pl[0].subParameters[0].elementCount, 1);

    QCOMPARE(pl[0].subParameters[0].enumValues.count(), 3);

    QCOMPARE(pl[0].subParameters[0].enumValues[0].first, 0);
    QCOMPARE(pl[0].subParameters[0].enumValues[0].second, QString("TTEa"));

    QCOMPARE(pl[0].subParameters[0].enumValues[1].first, 1);
    QCOMPARE(pl[0].subParameters[0].enumValues[1].second, QString("TTEb"));

    QCOMPARE(pl[0].subParameters[0].enumValues[2].first, 5);
    QCOMPARE(pl[0].subParameters[0].enumValues[2].second, QString("TTEc"));




    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcNegValueInEnumTest()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/negValueInEnum.xml");
    QCOMPARE(result, true);


    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    auto &pl = funList[0].reply.paramList;

    QCOMPARE(funList.count(), 1);

    QCOMPARE(funList[0].name, QString("negValueInEnum"));
    QCOMPARE(funList[0].request.ID, 24);
    QCOMPARE(funList[0].reply.ID, 23);

    QCOMPARE(pl.count(), 1);
    QCOMPARE(pl[0].name, QString("testEnum"));

    QCOMPARE(pl[0].rpcParamType, RPCParamType_t::param_enum);
    QCOMPARE(pl[0].elementBitLength, 8);
    QCOMPARE(pl[0].typeName, QString("enum TestEnum"));
    QCOMPARE(pl[0].elementCount, 1);

    QCOMPARE(pl[0].enumValues.count(), 2);

    QCOMPARE(pl[0].enumValues[0].first, 0);
    QCOMPARE(pl[0].enumValues[0].second, QString("TEa"));

    QCOMPARE(pl[0].enumValues[1].first, 1);
    QCOMPARE(pl[0].enumValues[1].second, QString("TEb"));

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_uint32_t()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = { 0x04, 0x05, 0x10, 0x20, 0x30};
    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_uint32_t.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);
    QCOMPARE( decoder.transfer.getTotalLength(), 5);

    QCOMPARE( decoder.decodedParams.count() , (int)1);
    QCOMPARE( decoder.decodedParams[0].value , (int64_t)0x30201005);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_int8_t()
{
    #if RUNTEST
    const uint8_t inBinData_array[2] = {0x04, 0xF0};
    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_int8_t.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].value , (int64_t)-16);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_int16_t()
{
    #if RUNTEST
    const uint8_t inBinData_array[3] = {0x04, 0xF0, 0xFF};
    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_int16_t.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].value , (int64_t)-16);

    #endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_array_char()
{
    #if RUNTEST
    const uint8_t inBinData_array[42] = {0x06, 0x48 ,0x65 ,0x6c ,0x6c ,0x6f ,0x20 ,0x57 ,0x6f ,0x72
                                       ,0x6c ,0x64 ,0x21 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
                                       ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
                                       ,0x00 ,0x00};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_array_char.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].string , QString("Hello World!"));

    #endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};



    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList[0].reply.paramList.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].name, QString("s_inout"));
    QCOMPARE(funList[0].reply.paramList[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].elementCount, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParameters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].rpcParamType, RPCParamType_t::param_struct);

    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters.count(), 3);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[1].elementCount, 42);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParameters[0].subParameters[2].elementCount, 1);


    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams.count() , 3);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[0].value , (int64_t)43);

    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].subParams.count() , 42);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].subParams[0].value , (int64_t)0x48);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].getParamDescription().rpcParamType , RPCParamType_t::param_array);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].string , QString("0x48 0x61 0x6c 0x6c 0x6f 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x34 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38"));
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].subParams[0].getParamDescription().rpcParamType , RPCParamType_t::param_int);

    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams.count() , 2);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[0].subParams.count() , 3);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[0].subParams[0].value ,(int64_t) 0x00);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[0].subParams[1].value , (int64_t)0x10);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[0].subParams[2].value ,(int64_t) 0x20);

    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].subParams[0].value ,(int64_t)0x01);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].subParams[1].value ,(int64_t) 0x11);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].subParams[2].value ,(int64_t) 0x21);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_testID()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);
    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams.count() , 3);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[0].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?0"));
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[1].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?1"));
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?2"));
    //qDebug() << decoder.decodedParams[0].subParams[0].subParams[2].getParamDescription().typeName;
    //qDebug() << decoder.decodedParams[0].subParams[0].subParams[2].getParamDescription().name;

    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0"));

    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[0].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?0"));
    QCOMPARE( decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1"));
    qDebug() << decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].getParamDescription().typeName;
    qDebug() << decoder.decodedParams[0].subParams[0].subParams[2].subParams[0].subParams[1].getParamDescription().name;

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_TestByID()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);
    QCOMPARE( decoder.fieldExists("scripts/fail.xml?25?0?0?2?0?1") , false);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?25?0?0?2?0?1") , false);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1") , true);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?2?0?2") , false);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1?0") , true);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1?0?0") , false);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?0"), true);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?1"), true);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?2"), true);
    QCOMPARE( decoder.fieldExists("scripts/decodeTest_struct_int.xml?24?0?0?3"), false);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_AccessByID()
{
    #if RUNTEST || 0
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);
    RPCRuntimeDecodedParam decParam = decoder.getDecodedParamByFieldID("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1");
    QCOMPARE( decParam.isNull() , false);
    QCOMPARE( decParam.getParamDescription().name , QString(""));
    QCOMPARE( decParam.getParamDescription().typeName , QString("uint8_t [3]"));
    QCOMPARE( decParam.string , QString("0x01 0x11 0x21"));

    decParam = decoder.getDecodedParamByFieldID("scripts/decodeTest_struct_int.xml?24?0?0?2?0?0");
    QCOMPARE( decParam.isNull() , false);
    QCOMPARE( decParam.getParamDescription().name , QString(""));
    QCOMPARE( decParam.getParamDescription().typeName , QString("uint8_t [3]"));
    QCOMPARE( decParam.string , QString("0x00 0x10 0x20"));

    decParam = decoder.getDecodedParamByFieldID("scripts/decodeTest_struct_int.xml?24?0?0?2?0?2");
    QCOMPARE( decParam.isNull() , true);

    decParam = decoder.getDecodedParamByFieldID("scripts/decodeTest_struct_int.xml?24?0");
    QCOMPARE( decParam.isNull() , false);
    QCOMPARE( decParam.getParamDescription().name , QString("s_inout"));
    QCOMPARE( decParam.getParamDescription().typeName , QString("TypedefTestStruct [1]"));
    QCOMPARE( decParam.string , QString(""));
    QCOMPARE( decParam.subParams.count() , 1);
    QCOMPARE( decParam.subParams[0].subParams.count() , 3);

    RPCRuntimeTransfer decTransfer = decoder.getDecodedTransferByFieldID("scripts/decodeTest_struct_int.xml?24");
    QCOMPARE( decTransfer.isNull() , false);
    QCOMPARE( decTransfer.isReply()  , false);
    QCOMPARE( decTransfer.getName()  , QString("typedefStructTest"));


    #endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_watchpoint_callback(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, QDateTime timestamp, int64_t value){
    callBack_FieldID = FieldID;
    callBack_humanReadableName = humanReadableName;
    callBack_plotIndex = plotIndex;
    callBack_timestamp = timestamp;
    callBack_value = value;
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_watchpoint()

{
    #if RUNTEST || 0
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    QPair<int,int> testPlotIndex (0,1);
    QDateTime timeStamp = QDateTime(QDateTime::currentDateTime());

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.addWatchPoint("scripts/decodeTest_struct_int.xml?24?0?0?0", "humanReadableName", testPlotIndex,
            std::bind(&TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_watchpoint_callback, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3, std::placeholders::_4, std::placeholders::_5) );
    decoder.setTimeStamp(timeStamp);
    initTestCase();
    decoder.RPCDecodeRPCData(inBinData);



    QCOMPARE( callBack_FieldID , QString("scripts/decodeTest_struct_int.xml?24?0?0?0"));
    QCOMPARE( callBack_humanReadableName , QString("humanReadableName"));
    QCOMPARE( callBack_plotIndex , testPlotIndex);
    QCOMPARE( callBack_timestamp , timeStamp);
    QCOMPARE( callBack_value , (int64_t)43);

    decoder.removeWatchPoint("scripts/decodeTest_struct_int.xml?24?0?0?0");
    initTestCase();
    decoder.RPCDecodeRPCData(inBinData);
    QCOMPARE( callBack_value ,(int64_t) 0);

    decoder.addWatchPoint("scripts/decodeTest_struct_int.xml?24?30?0?0", "humanReadableName", testPlotIndex,
            std::bind(&TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_watchpoint_callback, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3, std::placeholders::_4, std::placeholders::_5) );

    initTestCase();
    decoder.RPCDecodeRPCData(inBinData);
    QCOMPARE( callBack_value , (int64_t)0);

    #endif
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_treewidgetreport()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                         0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                         0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                         0x01 ,0x11 ,0x21};



    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);


    QList<QTreeWidgetItem *> items = decoder.getTreeWidgetReport(NULL);


    QCOMPARE( items.count() , 1);
    QCOMPARE( items[0]->text(0),QString("typedefStructTest"));
    QCOMPARE( items[0]->text(1),QString("RPC_UART_RESULT typedefStructTest(TypedefTestStruct s_inout[1]);"));

    QCOMPARE( items[0]->child(0)->childCount() , 1);
    QCOMPARE( items[0]->child(0)->text(0),QString("Request"));
    QCOMPARE( items[0]->child(0)->text(1),QString(""));

    QCOMPARE( items[0]->child(0)->child(0)->data(0,Qt::UserRole).toString() , QString("scripts/decodeTest_struct_int.xml?24?0"));
    QCOMPARE( items[0]->child(0)->child(0)->childCount() , 1);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->childCount() , 3);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(0)->childCount() , 0);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(0)->text(0) , QString("n(uint16_t)"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(0)->text(1) , QString("43"));
    qDebug() << items[0]->child(0)->child(0)->child(0)->child(0)->data(0,Qt::UserRole).toString();

    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(0)->childCount() , 0);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(1)->text(0) , QString("ia(uint8_t [42])"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(1)->text(1).trimmed() , QString("0x48 0x61 0x6C 0x6C 0x6F 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x34 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38").trimmed());

    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->childCount() , 1);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->text(0) , QString("iaa(uint8_t [1][2][3])"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->text(1).trimmed() , QString("").trimmed());

    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->childCount() , 2);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->data(0,Qt::UserRole).toString() , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->text(0) , QString("[0]"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->text(1).trimmed() , QString("").trimmed());


    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(0)->childCount() , 0);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(0)->data(0,Qt::UserRole).toString() , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?0"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(0)->text(0) , QString("[0]"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(0)->text(1).trimmed() , QString("0x00 0x10 0x20").trimmed());


    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(1)->childCount() , 0);
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(1)->data(0,Qt::UserRole).toString() , QString("scripts/decodeTest_struct_int.xml?24?0?0?2?0?1"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(1)->text(0) , QString("[1]"));
    QCOMPARE( items[0]->child(0)->child(0)->child(0)->child(2)->child(0)->child(1)->text(1).trimmed() , QString("0x01 0x11 0x21").trimmed());


    #endif
}

//

void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsingedBug()
{
    #if RUNTEST || 1
    const uint8_t inBinData_array[] = {0xFF,0xFF,0xFF,0x04,0x02,0x52,0x00,0x9A,0x00,0x50,0x00,0x3C,0x00,0xE3,0x4D};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcSignedUnsingedBug.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeChannelCodedData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 4);
    QCOMPARE( decoder.decodedParams[0].value ,(int64_t)82);
    QCOMPARE( decoder.decodedParams[1].value ,(int64_t)410);
    QCOMPARE( decoder.decodedParams[2].value ,(int64_t)80);
    QCOMPARE( decoder.decodedParams[3].value ,(int64_t)60);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_enum()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x1a, 0x01};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_enum.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.decodedParams[0].string , QString("TEb"));
    QCOMPARE( decoder.decodedParams[0].value ,(int64_t)1);

    #endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_struct_int_report()
{
    #if RUNTEST
const uint8_t inBinData_array[] = {0x18 ,0x2b ,0x00 ,0x48 ,0x61 ,0x6c ,0x6c ,0x6f ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x39 ,0x30,
                                     0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x34 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35,
                                     0x36 ,0x37 ,0x38 ,0x39 ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37 ,0x38 ,0x00 ,0x10 ,0x20,
                                     0x01 ,0x11 ,0x21};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);

    QStringList report=  decoder.getPrintableReport();


    QFile inFile("scripts/decodeTest_struct_int_report_mask.txt");

    QFile outfile("scripts/decodeTest_struct_int_report_output.txt");
    outfile.open(QIODevice::WriteOnly);
    inFile.open(QIODevice::ReadOnly);
    QTextStream out(&outfile);   // we will serialize the data into the file
    QTextStream in_mask(&inFile);   // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

    for(int i=0;i<report.count();i++){
        out << report[i]+'\n';
    }
    QCOMPARE(in.count()-1,report.count());
    for(int i=0;i<report.count();i++){
        QCOMPARE(in[i],report[i]);
    }
    #endif
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_enum_report()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {0x1a, 0x01};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_enum.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeRPCData(inBinData);

    QStringList report=  decoder.getPrintableReport();
    QFile inFile("scripts/decodeTest_enum_report_mask.txt");

    QFile outfile("scripts/decodeTest_enum_report_output.txt");
    outfile.open(QIODevice::WriteOnly);
    inFile.open(QIODevice::ReadOnly);
    QTextStream out(&outfile);   // we will serialize the data into the file
    QTextStream in_mask(&inFile);   // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

    for(int i=0;i<report.count();i++){
        out << report[i]+'\n';
    }
    QCOMPARE(in.count()-1,report.count());
    for(int i=0;i<report.count();i++){
        QCOMPARE(in[i],report[i]);
    }
    #endif
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_enum_report()
{
    #if RUNTEST
    const uint8_t inBinData_array[] = {'J', 'U', 'N', 'K', 0xff, 0xff, 0xff, 0x00, 0x1a, 0x01, 0x96, 0xe1, 'J', 'U', 'N', 'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_enum.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeChannelCodedData(inBinData);
    QCOMPARE( decoder.getErrorCRCHappened() , false);
    QStringList report=  decoder.getPrintableReport();
    QFile inFile("scripts/decodeTest_enum_report_mask.txt");
    inFile.open(QIODevice::ReadOnly);
    QTextStream in_mask(&inFile);   // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

    QCOMPARE(in.count()-1,report.count());
    for(int i=0;i<report.count();i++){
        QCOMPARE(in[i],report[i]);
    }
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_struct_int_report()
{
    #if RUNTEST
const uint8_t inBinData_array[] = {'J', 'U', 'N', 'K', 0xff, 0xff, 0xff, 0x74, 0x18, 0x2b, 0x00, 0x48, 0x60, 0x6c, 0x6c, 0x6f, 0x32, 0x34, 0x34, 0x36, 0x36,
                                   0x38, 0x1f, 0x38, 0x30, 0x30, 0x32, 0x32, 0x34, 0x34, 0x36, 0x36, 0x34, 0x38, 0x39, 0x30, 0x31, 0x00,
                                   0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x18, 0x36, 0x37,
                                   0x38, 0x00, 0x10, 0x20, 0x00, 0x11, 0x20, 0x93, 0x31, 'J', 'U', 'N', 'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeChannelCodedData(inBinData);

    QCOMPARE( decoder.decodedParams.count() , 1);
    QCOMPARE( decoder.getErrorCRCHappened() , false);
    QStringList report=  decoder.getPrintableReport();


    QFile inFile("scripts/decodeTest_struct_int_report_mask.txt");

    inFile.open(QIODevice::ReadOnly);
    QTextStream in_mask(&inFile);   // we will serialize the data into the file

    QString line_mask;
    QStringList in;
    do {
        line_mask = in_mask.readLine();
        in.append(line_mask);

    } while (!line_mask.isNull());

    QCOMPARE(in.count()-1,report.count());
    for(int i=0;i<report.count();i++){
        QCOMPARE(in[i],report[i]);
    }
    #endif
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcAccessParamDescriptionByFieldID_struct_int_report()
{
    #if RUNTEST
    RPCRunTimeProtocolDescription rpcinterpreter;



    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

	RPCRuntimeParameterDescription paramDesc = rpcinterpreter.getParamDescriptionByFieldID("scripts/decodeTest_struct_int.xml?24?0?0?0");

    QCOMPARE(paramDesc.rpcParamType,RPCParamType_t::param_int);

    paramDesc = rpcinterpreter.getParamDescriptionByFieldID("scripts/decodeTest_struct_int.xml?25");

    QCOMPARE(paramDesc.rpcParamType,RPCParamType_t::param_none);

    paramDesc = rpcinterpreter.getParamDescriptionByFieldID("scripts/decodeTest_struct_int.xml?24?1?0?0");

    QCOMPARE(paramDesc.rpcParamType,RPCParamType_t::param_none);

    paramDesc = rpcinterpreter.getParamDescriptionByFieldID("scripts/decodeTest_struct_int.xml?24?0?1?0");

    QCOMPARE(paramDesc.rpcParamType,RPCParamType_t::param_none);

    paramDesc = rpcinterpreter.getParamDescriptionByFieldID("scripts/decodeTest_struct_int.xml?24?0?0?1");

    QCOMPARE(paramDesc.rpcParamType,RPCParamType_t::param_array);
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTestFromChannelEncodedData_WrongCRC()
{
    #if RUNTEST
const uint8_t inBinData_array[] = {'J', 'U', 'N', 'K', 0xff, 0xff, 0xff, 0x74, 0x18, 0x2b, 0x00, 0x48, 0x60, 0x6c, 0x6c, 0x6f, 0x32, 0x34, 0x34, 0x36, 0x36,
                                   0x38, 0x1f, 0x38, 0x30, 0x30, 0x32, 0x32, 0x34, 0x34, 0x36, 0x36, 0x34, 0x38, 0x39, 0x30, 0x31, 0x00,
                                   0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x18, 0x36, 0x37,
                                   0x38, 0x00, 0x10, 0x20, 0x00, 0x11, 0x20, 0x93, 0x30, 'J', 'U', 'N', 'K'};

    RPCRunTimeProtocolDescription rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_struct_int.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecoder decoder(rpcinterpreter);
    decoder.RPCDecodeChannelCodedData(inBinData);

    QCOMPARE( decoder.getErrorCRCHappened() , true);
#endif
}

void TestRPCRuntimeInterpreter::playWithChannelEncoding(){

 #if RUNTEST
    const uint8_t inBinData_array[] = {0x1a, 0x01};
    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    RPCRunTimeProtocolDescription rpcinterpreter;
    rpcinterpreter.openProtocolDescription("scripts/decodeTest_enum.xml");

    RPCRuntimeDecoder decoder(rpcinterpreter);

    QByteArray outBinData = decoder.encodeToChannelCodedData(inBinData);

    QFile outfile("scripts/channelEncodedData.txt");
    outfile.open(QIODevice::WriteOnly);
    QTextStream out(&outfile);   // we will serialize the data into the file
    QString line("{");
    for(int i= 0;i<outBinData.count()-1;i++){
        line += QString("0x%1, ").arg((uint8_t)outBinData[i],2,16,QChar('0'));
        if (line.length() > 100){
            out << line+"\n";
            line = "";
        }
    }
    line += QString("0x%1}").arg((uint8_t)outBinData[outBinData.count()-1],2,16,QChar('0'));
    out << line;
#endif
}
