#ifndef RPCRUNTIMEINTERPRETER_H
#define RPCRUNTIMEINTERPRETER_H

#include <QString>
#include <QStringList>
#include "rpcruntimefunction.h"
#include "rpcruntimedecoderesult.h"

class RPCRunTimeInterpreter
{
public:
    RPCRunTimeInterpreter();
    ~RPCRunTimeInterpreter();

    bool openProtocolDescription(QString filename);

    uint32_t getRequestLength(uint8_t requestID);
    void decodeRequest(uint8_t* requestID, uint16_t length);

    QStringList getReport();
    QList<RPCRuntimeFunction> getFunctionList();

    RPCRuntimeDecodeResult decode(QByteArray inData);

private:
    QList<RPCRuntimeFunction> functionList;
    RPCRuntimeDecodeResult getFunctionByID(uint8_t ID);
};

#endif // RPCRUNTIMEINTERPRETER_H
