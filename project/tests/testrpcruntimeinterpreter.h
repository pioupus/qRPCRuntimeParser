#ifndef TESTRPCRUNTIMEINTERPRETER_H
#define TESTRPCRUNTIMEINTERPRETER_H
#include <QObject>
#include "autotest.h"

class TestRPCRuntimeInterpreter : public QObject
{
    Q_OBJECT


private slots:


    void loadXMLFile_rpcInt32ReplyTest();
    void loadXMLFile_rpcInt16EnumTest();
    void loadXMLFile_rpcSignedUnsignedTest();


    void loadXMLFile_rpcArrayInputTest();
    void loadXMLFile_rpcMultiArrayInputTest();
    void loadXMLFile_rpcStructInputTest();
    void loadXMLFile_rpcArrayInStructTest();
    void loadXMLFile_rpcEnumInArrayTest();void loadXMLFile_rpcNegValueInEnumTest();


};
DECLARE_TEST(TestRPCRuntimeInterpreter)



#endif // TESTRPCRUNTIMEINTERPRETER_H
