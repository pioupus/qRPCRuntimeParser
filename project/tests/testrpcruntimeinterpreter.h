#ifndef TESTRPCRUNTIMEINTERPRETER_H
#define TESTRPCRUNTIMEINTERPRETER_H
#include <QObject>
#include "autotest.h"

class TestRPCRuntimeInterpreter : public QObject
{
    Q_OBJECT


private slots:
    void initTestCase();
    void cleanupTestCase();

    void loadXMLFile();
};
DECLARE_TEST(TestRPCRuntimeInterpreter)



#endif // TESTRPCRUNTIMEINTERPRETER_H
