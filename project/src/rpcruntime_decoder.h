#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H
#include "rpcruntime_decoder.h"
#include "rpcruntime_function.h"
#include "rpcruntime_protocol_description.h"
#include <QList>

class RPCChannelCodec{
    public:
    RPCChannelCodec();
    ~RPCChannelCodec();
};

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
    QString declaration;
    bool isReply();
    void setIsReply(bool reply);

    int getTransferLength(uint8_t ID);

    QList<RPCRuntimeDecodedParam> decodedParams;

    RPCRunTimeProtocolDescription protocolDescription;
    QByteArray RPCDecodeRPCData(QByteArray inBuffer);
    void RPCDecodeChannelCodedData(QByteArray inBuffer);

    QByteArray encodeToChannelCodedData(QByteArray inBuffer);

    QStringList getPrintableReport();

    void setChannelCodecOutput(QByteArray codecOutput);

private:
    bool reply;
    QByteArray decodeParams(QByteArray inBuffer, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams);
    QStringList printsubType(int tabDepth, QList<RPCRuntimeDecodedParam> decodedParamList , bool isArrayField);
    QByteArray codecOutput;
};

#endif // RPCRUNTIMEDECODERESULT_H
