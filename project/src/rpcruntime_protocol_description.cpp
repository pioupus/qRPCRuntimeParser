#include "rpcruntime_protocol_description.h"

#include <QtXml>
#include <Qfile>
#include <QDebug>

#include "rpc_transmission/client/generated_general/RPC_TRANSMISSION_types.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC RPC_TRANSMISSION_RESULT phyPushDataBuffer(const char *buffer, size_t length){
    (void)buffer;
    (void)length;
    RPC_TRANSMISSION_RESULT result = RPC_TRANSMISSION_COMMAND_UNKNOWN;
    return result;
}

EXTERNC void RPC_TRANSMISSION_Parser_init(void){

}

EXTERNC RPC_TRANSMISSION_SIZE_RESULT RPC_TRANSMISSION_get_answer_length(const void *buffer, size_t size, void* sendingClassInstance){
    (void)buffer;
    (void)size;
    (void)sendingClassInstance;
    RPC_TRANSMISSION_SIZE_RESULT result;
    return result;
}

EXTERNC RPC_TRANSMISSION_SIZE_RESULT RPC_TRANSMISSION_get_request_size(const void *buffer, size_t size_bytes, void* sendingClassInstance){
    (void)buffer;
    (void)size_bytes;
    (void)sendingClassInstance;
    RPC_TRANSMISSION_SIZE_RESULT result;
    return result;
}

EXTERNC void RPC_TRANSMISSION_parse_answer(const void *buffer, size_t size, void* sendingClassInstance){
    (void)buffer;
    (void)size;
    (void)sendingClassInstance;

    RPCRunTimeProtocolDescription* testClass = (RPCRunTimeProtocolDescription*) sendingClassInstance;
    (void) testClass;

}

EXTERNC void RPC_TRANSMISSION_parse_request(const void *buffer, size_t size_bytes, void* sendingClassInstance){
    (void)buffer;
    (void)size_bytes;
    (void)sendingClassInstance;
}

RPCRunTimeProtocolDescription::RPCRunTimeProtocolDescription()
{

}

RPCRunTimeProtocolDescription::~RPCRunTimeProtocolDescription()
{

}

bool RPCRunTimeProtocolDescription::openProtocolDescription(QString filename)
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
            runtimefunction.reply.setIsNull(reply.isNull());
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

QList<RPCRuntimeFunction> RPCRunTimeProtocolDescription::getFunctionList()
{
    return functionList;
}


