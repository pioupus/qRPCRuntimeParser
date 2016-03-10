#include <QStringList>
#include <QPair>
#include <QString>

#include "testrpcruntimeinterpreter.h"
#include "rpcruntimeinterpreter.h"




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

    QCOMPARE(funList[0].reply.isNull, true);
}



void TestRPCRuntimeInterpreter::loadXMLFile_rpcSignedUnsignedTest()
{
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

    QCOMPARE(funList[0].reply.isNull, true);
}

void TestRPCRuntimeInterpreter::loadXMLFile_rpcInt32ReplyTest()
{
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

    QCOMPARE(funList[0].reply.isNull, false);
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
}

#if 1
void TestRPCRuntimeInterpreter::loadXMLFile_rpcArrayInputTest()
{
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

    QCOMPARE(funList[0].request.paramList[0].subParamters[0].rpcParamType, RPCParamType_t::param_character);
    QCOMPARE(funList[0].request.paramList[0].subParamters[0].elementBitLength, 8);
    QCOMPARE(funList[0].request.paramList[0].subParamters[0].typeName, QString("char"));

    QCOMPARE(funList[0].reply.isNull, false);
    QCOMPARE(funList[0].reply.ID, 11);
    QCOMPARE(funList[0].reply.paramList.count(), 0);
}
#endif
