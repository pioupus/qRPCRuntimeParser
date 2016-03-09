#include <QStringList>

#include "testrpcruntimeinterpreter.h"
#include "rpcruntimeinterpreter.h"


void TestRPCRuntimeInterpreter::initTestCase(){

}

void TestRPCRuntimeInterpreter::cleanupTestCase(){

}

void TestRPCRuntimeInterpreter::loadXMLFile()
{
    RPCRunTimeInterpreter rpcinterpreter;

    bool result = rpcinterpreter.openProtocolDescription("scripts/rpcInt16EnumTest.xml");
    QCOMPARE(result, true);

    QList<RPCRuntimeFunction> funList = rpcinterpreter.getFunctionList();
    QCOMPARE(funList.count(), 1);
    QCOMPARE(funList[0].name, QString("rpcFunc_sendUint16AndEnumAndEnum"));
    QCOMPARE(funList[0].ID, 2);
    QCOMPARE(funList[0].paramList.count(), 3);
    QCOMPARE(funList[0].paramList[0].name, QString("param1"));
    QCOMPARE(funList[0].paramList[1].name, QString("param2"));
    QCOMPARE(funList[0].paramList[3].name, QString("param3"));

    QCOMPARE(funList[0].paramList[0].elementBitLength, 16);
    QCOMPARE(funList[0].paramList[0].isSigned, false);
    QCOMPARE(funList[0].paramList[1].elementBitLength, 8);
    QCOMPARE(funList[0].paramList[3].elementBitLength, 8);

    QCOMPARE(funList[0].paramList[0].typeName, QString("uint16_t"));
    QCOMPARE(funList[0].paramList[1].typeName, QString("enum1_t"));
    QCOMPARE(funList[0].paramList[3].typeName, QString("enum2_t"));

    QCOMPARE(funList[0].paramList[0].rpcParamType, RPCParamType_t::param_int);
    QCOMPARE(funList[0].paramList[1].rpcParamType, RPCParamType_t::param_enum);
    QCOMPARE(funList[0].paramList[3].rpcParamType, RPCParamType_t::param_enum);

    QCOMPARE(funList[0].paramList[0].indexPosition, 1);
    QCOMPARE(funList[0].paramList[1].indexPosition, 2);
    QCOMPARE(funList[0].paramList[3].indexPosition, 3);
}
