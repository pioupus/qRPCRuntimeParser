#ifndef RPCRUNTIMEPROTOCOLDESCRIPTION_H
#define RPCRUNTIMEPROTOCOLDESCRIPTION_H

#include <QString>
#include <QStringList>
#include "rpcruntime_function.h"
#include <functional>   // std::bind



class RPCRunTimeProtocolDescription
{
public:
    RPCRunTimeProtocolDescription();
    ~RPCRunTimeProtocolDescription();

    bool openProtocolDescription(QString filename);

    uint32_t getRequestLength(uint8_t requestID);

    QStringList getReport();
    QList<RPCRuntimeFunction> getFunctionList();

    RPCRuntimeTransfer getTransferByID(int ID);


    RPCRuntimeParamterDescription getParamDescriptionByFieldID(QString FieldID);

    QString getFileName();






private:
    QList<RPCRuntimeFunction> functionList;
    QString fileName;

    RPCRuntimeParamterDescription getParameterDescriptionByFieldIDToken(QList<RPCRuntimeParamterDescription> &paramList, QStringList &IDToken, int index, bool isArray, int arrayElementCount);
};

#endif // RPCRUNTIMEINTERPRETER_H
