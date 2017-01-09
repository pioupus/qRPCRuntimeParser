
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
	void loadXMLFile_rpcDecodeTest_struct_int();
    void loadXMLFile_rpcDecodeTest_struct_int_access_by_field_id();
	void loadXMLFile_rpcDecodeTest_struct_int_treewidgetreport();
	void loadXMLFile_rpcSignedUnsingedBug();
	void loadXMLFile_rpcDecodeTest_enum();
	void loadXMLFile_rpcDecodeTest_struct_int_report();
	void loadXMLFile_rpcDecodeTest_enum_report();

	void loadXMLFile_rpcDecodeTestFromChannelEncodedData_enum_report();
	void loadXMLFile_rpcDecodeTestFromChannelEncodedData_struct_int_report();
    void loadXMLFile_rpcDecodeTestFromChannelEncodedData_WrongCRC();
#if 0
	void playWithChannelEncoding();
#endif
	void create_request_without_parameter();
	void create_request_with_int_parameter();
	void create_request_with_multiple_int_parameters();
	void create_request_with_enum_parameter();
	void create_request_with_array_parameter();
	void create_request_with_struct_parameter();
	void create_request_with_complex_parameter();
	void encode_hash_request_without_protocol();
	void decode_hash_request_without_protocol();
	void create_callback();

private:
    QString callBack_FieldID;
    QString callBack_humanReadableName;
    QPair<int,int> callBack_plotIndex;
    QDateTime callBack_timestamp;
    int64_t callBack_value;
};
DECLARE_TEST(TestRPCRuntimeInterpreter)



#endif // TESTRPCRUNTIMEINTERPRETER_H
