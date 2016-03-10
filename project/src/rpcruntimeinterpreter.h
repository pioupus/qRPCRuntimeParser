#ifndef RPCRUNTIMEINTERPRETER_H
#define RPCRUNTIMEINTERPRETER_H

#include <QString>
#include <QStringList>
#include "rpcruntimefunction.h"

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

private:
    QList<RPCRuntimeFunction> functionList;
};

#endif // RPCRUNTIMEINTERPRETER_H
