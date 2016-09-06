#include "rpcruntime_protocol_description.h"

#include <QtXml>
#include <QFile>
#include <QDebug>
#include <QString>
#include <functional>

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

QList<RPCRuntimeFunction> RPCRunTimeProtocolDescription::getFunctionList() const
{
    return functionList;
}

RPCRuntimeTransfer RPCRunTimeProtocolDescription::getTransferByID(int ID) const
{
    RPCRuntimeTransfer result_none;
    for ( RPCRuntimeFunction func:functionList){
        if(func.reply.ID == ID){
            return func.reply;
        }else if(func.request.ID == ID){
            return func.request;
        }
    }
	return  result_none;
}

RPCRuntimeParamterDescription RPCRunTimeProtocolDescription::getParamDescriptionByFieldID(QString FieldID)
{
	RPCRuntimeParamterDescription result;
	RPCRuntimeParamterDescription result_none;


	QStringList IDToken = FieldID.split("?");
	if (IDToken.count() < 3){
		return result_none;
	}
	if (IDToken[0] != getFileName()){
		return result_none;
	}
	RPCRuntimeTransfer transfer = getTransferByID(IDToken[1].toInt());

	if (!transfer.isNull()){
		int tokenIndex=2;
		bool wasArray = false;
		int arrayElementCount = 0;
		QList<RPCRuntimeParamterDescription>  paramList =  transfer.paramList;
		while (tokenIndex < IDToken.count()){

			result = getParameterDescriptionByFieldIDToken(paramList,IDToken,tokenIndex,wasArray, arrayElementCount);
			if (result.rpcParamType == RPCParamType_t::param_array){
				wasArray = true;
			}else{
				wasArray = false;
			}

			if (result.rpcParamType == RPCParamType_t::param_none){
				return result_none;
			}
			arrayElementCount = result.elementCount;
			paramList = result.subParameters;
			tokenIndex++;
		}
	}
	return result;

}

QString RPCRunTimeProtocolDescription::getFileName() const
{
	return fileName;
}

RPCRuntimeParamterDescription RPCRunTimeProtocolDescription::getParameterDescriptionByFieldIDToken(QList<RPCRuntimeParamterDescription> &paramList, QStringList &IDToken, int index, bool isArray, int arrayElementCount)
{
	RPCRuntimeParamterDescription result_none;
	bool ok;
	int FieldID_token = IDToken[index].toInt(&ok);
	if (ok){
		if (paramList.count() > FieldID_token){
			return paramList.at(FieldID_token);
		}else if (isArray){
			if (FieldID_token < arrayElementCount){
				return paramList.at(0);
			}else{
				return result_none;
			}
		}
	}
	return result_none;
}



