
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
    void loadXMLFile_rpcEnumInArrayTest();
    void loadXMLFile_rpcNegValueInEnumTest();
    void loadXMLFile_rpcDecodeTest_uint32_t();
    void loadXMLFile_rpcDecodeTest_int8_t();
    void loadXMLFile_rpcDecodeTest_int16_t();
    void loadXMLFile_rpcDecodeTest_array_char();

    void playWithChannelEncoding();
    void loadXMLFile_rpcDecodeTest_struct_int();
    void loadXMLFile_rpcDecodeTest_enum();
    void loadXMLFile_rpcDecodeTest_enum_report();
    void loadXMLFile_rpcDecodeTest_struct_int_report();
    void loadXMLFile_rpcDecodeTestFromChannelEncodedData_enum_report();
    void loadXMLFile_rpcDecodeTestFromChannelEncodedData_struct_int_report();
    void loadXMLFile_rpcDecodeTestFromChannelEncodedData_WrongCRC();



};
DECLARE_TEST(TestRPCRuntimeInterpreter)



#endif // TESTRPCRUNTIMEINTERPRETER_H
