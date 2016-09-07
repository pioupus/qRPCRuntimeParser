#include "rpcruntime_transfer.h"

RPCRuntimeTransfer::RPCRuntimeTransfer(){
	empty = true;
	isPackageLenghCalced = false;
	ID = 0;
	reply = false;
}

bool RPCRuntimeTransfer::isNull() const
{
	return empty;
}

void RPCRuntimeTransfer::setIsNull(bool value)
{
	empty = value;
}

int RPCRuntimeTransfer::getTotalLength()
{
	int result = 1;
	for(int i=0;i<paramList.count();i++){
		RPCRuntimeParameterDescription item = paramList[i];
		result += (item.elementBitLength/8);
	}
	return result;
}

bool RPCRuntimeTransfer::isReply()
{
	return reply;
}

void RPCRuntimeTransfer::setReply(bool r)
{
	reply = r;
}

QString RPCRuntimeTransfer::getName()
{
	return name;
}

void RPCRuntimeTransfer::setName(QString n)
{
	name = n;
}




bool RPCRuntimeTransfer::loadParamListFromXML(QDomElement xmlParams)
{
	while (!xmlParams.isNull()){

		if (xmlParams.tagName()=="parameter"){
			RPCRuntimeParameterDescription param;
			if (!param.loadFromXML(xmlParams)){
				return false;
			}
			paramList.append(param);
		}
		xmlParams = xmlParams.nextSibling().toElement();
	}
	return true;
}
