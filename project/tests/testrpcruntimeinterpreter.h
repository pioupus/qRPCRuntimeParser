
#ifndef TESTRPCRUNTIMEINTERPRETER_H
#define TESTRPCRUNTIMEINTERPRETER_H
#include <QObject>
#include "autotest.h"

class TestRPCRuntimeInterpreter : public QObject
{
    Q_OBJECT

public:
    void loadXMLFile_rpcDecodeTest_struct_int_watchpoint_callback(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, QDateTime timestamp, int64_t value);
private slots:

    void initTestCase();
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

    void loadXMLFile_rpcDecodeTest_struct_int_treewidgetreport();


    void loadXMLFile_rpcDecodeTest_struct_int_testID();
    void loadXMLFile_rpcDecodeTest_struct_int_AccessByID();
    void loadXMLFile_rpcDecodeTest_struct_int_TestByID();
    void loadXMLFile_rpcDecodeTest_struct_int_watchpoint();

    void loadXMLFile_rpcAccessParamDescriptionByFieldID_struct_int_report();
    void loadXMLFile_rpcSignedUnsingedBug();
private:
    QString callBack_FieldID;
    QString callBack_humanReadableName;
    QPair<int,int> callBack_plotIndex;
    QDateTime callBack_timestamp;
    int64_t callBack_value;
};
DECLARE_TEST(TestRPCRuntimeInterpreter)



#endif // TESTRPCRUNTIMEINTERPRETER_H
