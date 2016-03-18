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
    QByteArray decode(QByteArray inBuffer);
    QList<RPCRuntimeDecodedParam> subParams;
    RPCRuntimeParamterDescription getParamDescription() const;

    int64_t value;
    QString string;

private:
    RPCRuntimeParamterDescription paramDescription;
};

class RPCRuntimeDecoder
{
public:
    RPCRuntimeDecoder(RPCRunTimeProtocolDescription protocolDescription);
    ~RPCRuntimeDecoder();
    bool isNull();
    bool isReply();
    void setIsReply(bool reply);
    int getTransferLength(uint8_t ID);
    RPCRunTimeProtocolDescription protocolDescription;
    QByteArray RPCDecodeRPCData(QByteArray inBuffer);
    void RPCDecodeChannelCodedData(QByteArray inBuffer);
    QByteArray encodeToChannelCodedData(QByteArray inBuffer);

    QStringList getPrintableReport();

    void setChannelCodecOutput(QByteArray codecOutput);

    void clear();
    bool getErrorCRCHappened() const;
    void setErrorCRCHappened(bool value);

    bool getErrorChannelCodecHappened() const;
    void setErrorChannelCodecHappened(bool value);

    QString name;
    QString declaration;
    QList<RPCRuntimeDecodedParam> decodedParams;
    RPCRuntimeTransfer transfer;
private:

    QByteArray decodeParams(QByteArray inBuffer, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams);
    QStringList printsubType(int tabDepth, QList<RPCRuntimeDecodedParam> decodedParamList , bool isArrayField);
    QByteArray codecOutput;
    RPCRuntimeFunction getFunctionByID(uint8_t ID);
    bool isReplyByID(uint8_t ID, RPCRuntimeFunction fun);
    RPCRuntimeTransfer getTransfer(RPCRuntimeFunction fun, bool isReply);

    bool reply;
    bool errorCRCHappened;
    bool errorChannelCodecHappened;
};

#endif // RPCRUNTIMEDECODERESULT_H
