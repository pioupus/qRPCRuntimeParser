#include "rpcruntime_protocol_description.h"

#include <QtXml>
#include <Qfile>
#include <QDebug>
#include <QString>
#include <functional>

RPCRunTimeProtocolDescription::RPCRunTimeProtocolDescription()
{

}

RPCRunTimeProtocolDescription::~RPCRunTimeProtocolDescription()
{

}

bool RPCRunTimeProtocolDescription::openProtocolDescription(QString filename)
{
    QDomDocument xmlBOM;
    this->fileName = filename;
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly ))
    {
        qCritical() << "Error while loading file";
        return false;
    }

    xmlBOM.setContent(&f);
    f.close();
    QDomElement root=xmlBOM.documentElement();
    QString Type=root.tagName();
    if (root.tagName() != "RPC"){
        qCritical() << "not a RPC description file";
        return false;
    }

    QDomElement function=root.firstChild().toElement();
    functionList.clear();
    while(!function.isNull()){
        if (function.tagName()=="function"){
            bool ok;
            RPCRuntimeFunction runtimefunction;
            runtimefunction.name = function.attribute("name","");
            if (runtimefunction.name == ""){
                qCritical() << "found RPC Function without name";
                return false;
            }
            QDomElement declaration=function.firstChildElement("declaration");
            QDomElement request=function.firstChildElement("request");
            QDomElement reply = function.firstChildElement("reply");

            if (declaration.isNull()){
                qCritical() << "didnt found declaration in function: " << runtimefunction.name;
                return false;
            }

            if (request.isNull()){
                qCritical() << "didnt found request in function: " << runtimefunction.name;
                return false;
            }
            runtimefunction.request.ID = request.attribute("ID","").toInt(&ok);
            if (!ok){
                qCritical() << "didnt found ID in function: " << runtimefunction.name;
                return false;
            }

            runtimefunction.declaration = declaration.text();
            runtimefunction.request.setIsNull(request.isNull());
            runtimefunction.request.setReply(false);
            runtimefunction.request.setName(runtimefunction.name);
            runtimefunction.reply.setIsNull(reply.isNull());
            runtimefunction.reply.ID = reply.attribute("ID","").toInt(&ok);
            runtimefunction.reply.setReply(true);
            runtimefunction.reply.setName(runtimefunction.name);

            if (!runtimefunction.request.loadParamListFromXML(request.firstChild().toElement())){
                return false;
            }

            if (!runtimefunction.reply.loadParamListFromXML(reply.firstChild().toElement())){
                return false;
            }

            functionList.append(runtimefunction);
        }
        function = function.nextSibling().toElement();
    }
    return true;
}

QList<RPCRuntimeFunction> RPCRunTimeProtocolDescription::getFunctionList()
{
    return functionList;
}

QString RPCRunTimeProtocolDescription::getFileName()
{
    return fileName;
}

void RPCRunTimeProtocolDescription::addWatchPoint(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, watchCallBack_t callback)
{

    RPCWatchPoint wp(FieldID,humanReadableName,plotIndex,callback);
    watchpointList.append(wp);
}

void RPCRunTimeProtocolDescription::removeWatchPoint(QString FieldID)
{
    int i = 0;
    while(i < watchpointList.count()){
        auto wp = watchpointList[i];
        if (wp.FieldID == FieldID){

            watchpointList.removeAt(i);
        }else{
            i++;
        }
    }
    qDebug()<< "WPl count" << watchpointList.count() <<  this;

}

void RPCRunTimeProtocolDescription::clearWatchPoint()
{
    watchpointList.clear();
}

QList<RPCWatchPoint> RPCRunTimeProtocolDescription::getWatchPointList()
{
    qDebug()<< "WPl dsfsdf count" << watchpointList.count() << this;
    return watchpointList;
}




RPCWatchPoint::RPCWatchPoint()
{
    valid = false;
    this->callback = NULL;
}

RPCWatchPoint::RPCWatchPoint(QString FieldID, QString humanReadableName, QPair<int, int> plotIndex, watchCallBack_t callback)
{
    this->callback = callback;
    this->plotIndex = plotIndex;
    this->humanReadableName = humanReadableName;
    this->FieldID = FieldID;
    valid = true;
}

RPCWatchPoint::~RPCWatchPoint()
{

}

void RPCWatchPoint::call(QDateTime timeStamp, int64_t val)
{
    callback(FieldID,humanReadableName,plotIndex,timeStamp,val);
}
