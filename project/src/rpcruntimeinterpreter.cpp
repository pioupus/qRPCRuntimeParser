#include "rpcruntimeinterpreter.h"
#include <QtXml>
#include <Qfile>
#include <QDebug>

RPCRunTimeInterpreter::RPCRunTimeInterpreter()
{

}

RPCRunTimeInterpreter::~RPCRunTimeInterpreter()
{

}

bool RPCRunTimeInterpreter::openProtocolDescription(QString filename)
{
    QDomDocument xmlBOM;
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
            QDomElement request=function.firstChildElement("request");
            QDomElement reply = function.firstChildElement("reply");

            if (request.isNull()){
                qCritical() << "didnt found request in function: " << runtimefunction.name;
                return false;
            }
            runtimefunction.request.ID = request.attribute("ID","").toInt(&ok);
            if (!ok){
                qCritical() << "didnt found ID in function: " << runtimefunction.name;
                return false;
            }

            runtimefunction.reply.isNull = reply.isNull();
            runtimefunction.reply.ID = reply.attribute("ID","").toInt(&ok);

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

QList<RPCRuntimeFunction> RPCRunTimeInterpreter::getFunctionList()
{
    return functionList;
}

