#include <QStringList>
#include <QByteArray>
#include <QPair>
#include <QString>

#include "testrpcruntimeinterpreter.h"
#include "rpcruntimeinterpreter.h"
#include "rpcruntimedecoderesult.h"




void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt16EnumTest()
{
    RPCRunTimeInterpreter rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcInt16EnumTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].name, QString("rpcFunc_sendUint16AndEnumAndEnum"));
    QCOMPARE(funList[0].request.ID, 2);

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
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsignedTest()
{
#if 1
    RPCRunTimeInterpreter rpcinterpreter;

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
#if 1
    RPCRunTimeInterpreter rpcinterpreter;

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
    QCOMPARE(funList[0].reply.paramList[0].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].elementBitLength, 32);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].isSigned, true);
#endif
}


void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInputTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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

    QCOMPARE(funList[0].request.paramList[0].subParamters.count(), 1);
    QCOMPARE(funList[0].request.paramList[0].subParamters[0].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(funList[0].request.paramList[0].subParamters[0].elementBitLength, 8);
    QCOMPARE(funList[0].request.paramList[0].subParamters[0].typeName, QString("char"));

    QCOMPARE(funList[0].reply.isNull(), false);
    QCOMPARE(funList[0].reply.ID, 11);
    QCOMPARE(funList[0].reply.paramList.count(), 0);
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcMultiArrayInputTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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

    QCOMPARE(pl[0].subParamters.count(), 1);

    QCOMPARE(pl[0].subParamters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].subParamters[0].elementBitLength, 96);
    QCOMPARE(pl[0].subParamters[0].typeName, QString("char [3][4]"));
    QCOMPARE(pl[0].subParamters[0].elementCount, 3);

    QCOMPARE(pl[0].subParamters[0].subParamters.count(), 1);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].elementBitLength, 32);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].typeName, QString("char [4]"));
    QCOMPARE(pl[0].subParamters[0].subParamters[0].elementCount, 4);

    QCOMPARE(pl[0].subParamters[0].subParamters[0].subParamters.count(), 1);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].subParamters[0].elementBitLength, 8);
    QCOMPARE(pl[0].subParamters[0].subParamters[0].subParamters[0].typeName, QString("char"));
    QCOMPARE(pl[0].subParamters[0].subParamters[0].subParamters[0].elementCount, 1);

    QCOMPARE(funList[0].reply.isNull(), false);
    QCOMPARE(funList[0].reply.ID, 9);
    QCOMPARE(funList[0].reply.paramList.count(), 0);
    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcStructInputTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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

    QCOMPARE(funList[0].reply.paramList[0].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].rpcParamType, RPCParamType_t::param_struct);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].elementBitLength, 64);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].typeName, QString("struct TestStruct"));

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters.count(), 4);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].elementBitLength, 32);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].name, QString("n1"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].typeName, QString("uint32_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].elementBitLength, 16);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].name, QString("n2"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].typeName, QString("int16_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].isSigned, true);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].indexPosition, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].name, QString("n3"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].typeName, QString("char"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].indexPosition, 3);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].name, QString("n4"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[3].indexPosition, 4);

    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInStructTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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

    QCOMPARE(funList[0].reply.paramList[0].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].rpcParamType, RPCParamType_t::param_struct);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].elementBitLength, 400);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].typeName, QString("TypedefTestStruct"));

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters.count(), 3);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].elementBitLength, 16);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].name, QString("n"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].typeName, QString("uint16_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[0].indexPosition, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].elementBitLength, 336);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].elementCount, 42);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].name, QString("ia"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].typeName, QString("uint8_t [42]"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].indexPosition, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters[0].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters[0].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters[0].isSigned, false);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[1].subParamters[0].elementCount, 1);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].elementBitLength, 48);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].elementCount, 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].name, QString("iaa"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].typeName, QString("uint8_t [1][2][3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].indexPosition, 3);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].typeName, QString("uint8_t [2][3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].elementBitLength, 48);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].elementCount, 2);

    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters.count(), 1);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_array);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].typeName, QString("uint8_t [3]"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].elementBitLength, 24);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].elementCount, 3);


    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].subParamters[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].subParamters[0].elementBitLength, 8);
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].subParamters[0].typeName, QString("uint8_t"));
    QCOMPARE(funList[0].reply.paramList[0].subParamters[0].subParamters[2].subParamters[0].subParamters[0].subParamters[0].isSigned, false);



    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcEnumInArrayTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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





    QCOMPARE(pl[0].subParamters.count(), 1);
    QCOMPARE(pl[0].subParamters[0].rpcParamType, RPCParamType_t::param_enum);
    QCOMPARE(pl[0].subParamters[0].elementBitLength, 32);
    QCOMPARE(pl[0].subParamters[0].typeName, QString("TypedefTestEnum"));
    QCOMPARE(pl[0].subParamters[0].elementCount, 1);

    QCOMPARE(pl[0].subParamters[0].enumValues.count(), 3);

    QCOMPARE(pl[0].subParamters[0].enumValues[0].first, 0);
    QCOMPARE(pl[0].subParamters[0].enumValues[0].second, QString("TTEa"));

    QCOMPARE(pl[0].subParamters[0].enumValues[1].first, 1);
    QCOMPARE(pl[0].subParamters[0].enumValues[1].second, QString("TTEb"));

    QCOMPARE(pl[0].subParamters[0].enumValues[2].first, 5);
    QCOMPARE(pl[0].subParamters[0].enumValues[2].second, QString("TTEc"));




    #endif
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcNegValueInEnumTest()
{
    #if 1
    RPCRunTimeInterpreter rpcinterpreter;

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

void TestRPCRuntimeInterpreter::loadXMLFile_rpcDecodeTest_int32_t()
{
    #if 1
    const uint8_t inBinData_array[5] = {0x04, 0x05, 0x00, 0x00, 0x00};
    RPCRunTimeInterpreter rpcinterpreter;

    QByteArray inBinData = QByteArray((char*)inBinData_array, sizeof(inBinData_array));

    bool result = rpcinterpreter.openProtocolDescription("scripts/decodeTest_int32_t.xml");
    QCOMPARE(result, true);

    RPCRuntimeDecodeResult decoderResult = rpcinterpreter.decode(inBinData);
    QCOMPARE( decoderResult.isNull(), false);
    QCOMPARE( decoderResult.transfer.isNull(), false);
    QCOMPARE( decoderResult.isReply(), false);
    QCOMPARE( decoderResult.name, QString("simpleTest"));
    QCOMPARE( decoderResult.transfer.getTotalLength(), 5);


    #endif
}
