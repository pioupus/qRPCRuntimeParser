#ifndef RPCRUNTIMEINTERPRETER_H
#define RPCRUNTIMEINTERPRETER_H

#include <QString>
#include <QStringList>
#include "rpcruntimefunction.h"
//#include "rpcruntimedecoderesult.h"

class RPCRunTimeProtocolDescription
{
public:
    RPCRunTimeProtocolDescription();
    ~RPCRunTimeProtocolDescription();

    bool openProtocolDescription(QString filename);

    uint32_t getRequestLength(uint8_t requestID);
    void decodeRequest(uint8_t* requestID, uint16_t length);

    QStringList getReport();
    QList<RPCRuntimeFunction> getFunctionList();


private:
    QList<RPCRuntimeFunction> functionList;
};

#endif // RPCRUNTIMEINTERPRETER_H
