#ifndef RPCRUNTIMEDECODER_H
#define RPCRUNTIMEDECODER_H

#include "rpcruntime_decoder.h"
#include "rpcruntime_function.h"
#include "rpcruntime_protocol_description.h"
#include "rpc_watch_point.h"
#include "rpcruntime_decoded_param.h"
#include <QList>
#include <QTreeWidgetItem>
#include "channel_codec/channel_codec_types.h"

#define CHANNEL_CODEC_TX_BUFFER_SIZE 64
#define CHANNEL_CODEC_RX_BUFFER_SIZE 64

struct RPCRuntimeDecoder
{
public:
    RPCRuntimeDecoder(RPCRunTimeProtocolDescription protocolDescription);
    RPCRuntimeDecoder();
    ~RPCRuntimeDecoder();
    bool isNull();
    bool isReply();
    void setIsReply(bool reply);
    int getTransferLength(uint8_t ID);
    RPCRunTimeProtocolDescription protocolDescription;
    void setTimeStamp(QDateTime timeStamp);
    QByteArray RPCDecodeRPCData(QByteArray inBuffer);
    void RPCDecodeChannelCodedData(QByteArray inBuffer);
    QByteArray encodeToChannelCodedData(QByteArray inBuffer);

    QStringList getPrintableReport();
    QList<QTreeWidgetItem *> getTreeWidgetReport(QTreeWidgetItem *parent);

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

    bool fieldExists(QString FieldID);
    RPCRuntimeDecodedParam getDecodedParamByFieldID(QString FieldID);

    RPCRuntimeTransfer getDecodedTransferByFieldID(QString FieldID);

    RPCRuntimeParamterDescription getParamDescriptionByFieldID(QString FieldID);

	void addWatchPoint(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, RPCWatchPoint::WatchCallBack callback);
    void removeWatchPoint(QString FieldID);
    void clearWatchPoint();

    QList<RPCWatchPoint> getWatchPointList();

private:

    QByteArray decodeParams(QByteArray inBuffer, QString FieldID, QString OverwriteID, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams);
    QStringList printsubType(int tabDepth, QList<RPCRuntimeDecodedParam> decodedParamList , bool isArrayField);
    QByteArray codecOutput;
    RPCRuntimeFunction getFunctionByID(uint8_t ID);
    bool isReplyByID(uint8_t ID, RPCRuntimeFunction fun);
    RPCRuntimeTransfer getTransfer(RPCRuntimeFunction fun, bool isReply);

    bool reply;
    bool errorCRCHappened;
    bool errorChannelCodecHappened;
    QDateTime timeStamp;
    QList<QTreeWidgetItem *> getTreeWidgetReport_recursive(QTreeWidgetItem *parent, QList<RPCRuntimeDecodedParam> decodedParamList, bool isArrayField);

    QList<RPCWatchPoint> watchpointList;

    char channel_codec_rxbuffer[channel_codec_comport_COUNT][CHANNEL_CODEC_RX_BUFFER_SIZE];
    char channel_codec_txbuffer[channel_codec_comport_COUNT][CHANNEL_CODEC_TX_BUFFER_SIZE];

    channel_codec_instance_t channel_codec_instance[channel_codec_comport_COUNT];

};

#endif // RPCRUNTIMEDECODERESULT_H
