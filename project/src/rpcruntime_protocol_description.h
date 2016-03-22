#ifndef RPCRUNTIMEPROTOCOLDESCRIPTION_H
#define RPCRUNTIMEPROTOCOLDESCRIPTION_H

#include <QString>
#include <QStringList>
#include "rpcruntime_function.h"
#include <functional>   // std::bind

typedef std::function<void(QString, QString, QPair<int,int>, QDateTime, int64_t)> watchCallBack_t;

class RPCWatchPoint{
public:
    RPCWatchPoint();
    RPCWatchPoint(QString FieldID,QString humanReadableName,QPair<int,int> plotIndex, watchCallBack_t callback);
    ~RPCWatchPoint();

    QString FieldID;
    QString humanReadableName;
    QPair<int,int> plotIndex;
    watchCallBack_t callback;

    void call(QDateTime timeStamp, int64_t val);


private:
    bool valid;
};

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

    QString getFileName();

    void addWatchPoint(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, watchCallBack_t callback);
    void removeWatchPoint(QString FieldID);
    void clearWatchPoint();

    QList<RPCWatchPoint> getWatchPointList();


private:
    QList<RPCRuntimeFunction> functionList;
    QString fileName;
    QList<RPCWatchPoint> watchpointList;

};

#endif // RPCRUNTIMEINTERPRETER_H
