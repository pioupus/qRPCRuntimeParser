#ifndef RPCRUNTIMEDECODERESULT_H
#define RPCRUNTIMEDECODERESULT_H
#include "rpcruntimedecoderesult.h"
#include "rpcruntimefunction.h"
#include "rpcruntimeinterpreter.h"
#include <QList>

class RPCRuntimeDecodedParam{
public:
    RPCRuntimeDecodedParam(RPCRuntimeParamterDescription paramDescription);
    ~RPCRuntimeDecodedParam();

    int64_t value;
    QString string;

    QByteArray decode(QByteArray inBuffer);

    QList<RPCRuntimeDecodedParam> subParams;

    RPCRuntimeParamterDescription getParamDescription() const;

private:
    RPCRuntimeParamterDescription paramDescription;
};

class RPCRuntimeDecoder
{
public:
    RPCRuntimeDecoder(RPCRunTimeProtocolDescription protocolDescription);
    ~RPCRuntimeDecoder();
    bool isNull();

    RPCRuntimeTransfer transfer;

    QString name;
    bool isReply();
    void setIsReply(bool reply);

    QList<RPCRuntimeDecodedParam> decodedParams;

    RPCRunTimeProtocolDescription protocolDescription;
    QByteArray decode(QByteArray inBuffer);

    QStringList getPrintableReport();

private:
    bool reply;
    QByteArray decodeParams(QByteArray inBuffer, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams);
    QStringList printsubType(int tabDepth, QList<RPCRuntimeDecodedParam> decodedParamList , bool isArrayField);
};

#endif // RPCRUNTIMEDECODERESULT_H
