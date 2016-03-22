#include <QDebug>
#include "rpcruntime_function.h"



RPCRuntimeFunction::RPCRuntimeFunction()
{

}

RPCRuntimeFunction::~RPCRuntimeFunction()
{

}

bool RPCRuntimeFunction::isNull()
{
   return ((request.isNull()) && (reply.isNull()));
}

RPCRuntimeParamterDescription::RPCRuntimeParamterDescription()
{
    elementBitLength = 0;
    elementCount = 1;
    indexPosition = 0;
    rpcParamType = RPCParamType_t::param_none ;
    isSigned = true;

}

RPCRuntimeParamterDescription::~RPCRuntimeParamterDescription()
{

}

RPCRuntimeTransfer::RPCRuntimeTransfer(){
    empty = true;
    isPackageLenghCalced = false;
    ID = 0;
    reply = false;
}

RPCRuntimeTransfer::~RPCRuntimeTransfer(){

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
        RPCRuntimeParamterDescription item = paramList[i];
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
            RPCRuntimeParamterDescription param;
            if (!param.loadFromXML(xmlParams)){
                return false;
            }
            paramList.append(param);
        }
        xmlParams = xmlParams.nextSibling().toElement();
    }
    return true;
}



bool RPCRuntimeParamterDescription::loadFromXML(QDomElement xmlParams){
    bool ok = true;
    bool ok_gesamt = true;
    bool isArrayType = false;

    if (xmlParams.tagName() == "array"){
        isArrayType = true;
    }
    typeName = xmlParams.attribute("ctype","");
    elementBitLength = xmlParams.attribute("bits","").toInt(&ok);
    ok_gesamt &= ok;
    elementCount = 1; //will be overwritten by array type
    ok_gesamt &= ok;
    if(isArrayType == false){
        name = xmlParams.attribute("name","");
        indexPosition = xmlParams.attribute("position","").toInt(&ok);
        if(!ok){
            indexPosition = xmlParams.attribute("memberpos","").toInt(&ok);
        }
        ok_gesamt |= ok;
    }

    if (!setTypeByString(xmlParams.attribute("type",""))){
        //  qCritical() << "unknown type in function: " << parent->name << "and parameter" << param.name ;
        return false;
    }

    if (rpcParamType == RPCParamType_t::param_array){
        QDomElement xmlSubParams=xmlParams.firstChild().toElement();
        while(!xmlSubParams.isNull()){
            RPCRuntimeParamterDescription subparam;
            if (!subparam.loadFromXML(xmlSubParams)){
                return false;
            }

            subParameters.append(subparam);
            elementCount = elementBitLength / subparam.elementBitLength;
            xmlSubParams = xmlSubParams.nextSibling().toElement();
        }

    }

    if (rpcParamType == RPCParamType_t::param_struct){
        elementBitLength = 0;
        QDomElement xmlSubParams=xmlParams.firstChild().toElement();
        while(!xmlSubParams.isNull()){
            RPCRuntimeParamterDescription subparam;
            if (!subparam.loadFromXML(xmlSubParams)){
                return false;
            }
            elementBitLength += subparam.elementBitLength;
            subParameters.append(subparam);
            xmlSubParams = xmlSubParams.nextSibling().toElement();
        }

    }
    QDomElement paramSigned = xmlParams.firstChildElement("integer");

    if (!paramSigned.isNull()){
        isSigned = paramSigned.attribute("signed","").toLower() == QString("true");
    }else{
        if (rpcParamType == RPCParamType_t::param_int) {
           // qCritical() << "didnt found whether int is signed or not in function: " << parent->name << "and parameter" << param.name ;
            return false;
        }
    }

    QDomElement paramEnum = xmlParams.firstChildElement("enum");

    if (!paramEnum.isNull()){
        while(!paramEnum.isNull()){
            if (paramEnum.tagName()=="enum"){
                QPair<int,QString> enumItem;
                bool ok;
                enumItem.first = paramEnum.attribute("value","").toInt(&ok);
                enumItem.second = paramEnum.attribute("name","");
                if(enumItem.second == ""){
                  //  qCritical() << "enum name not valid in function: " << parent->name << "and parameter" << param.name ;
                    return false;
                }
                if(ok){
                    enumValues.append(enumItem);
                }
            }
            paramEnum = paramEnum.nextSibling().toElement();
        }
    }else{
        if (rpcParamType == RPCParamType_t::param_enum) {
        //    qCritical() << "didnt found whether int is signed or not in function: " << parent->name << "and parameter" << param.name ;
            return false;
        }
    }

    if ((name=="")&&(isArrayType==false) ){
       // qCritical() << "didnt found param name in function: " << parent->name;
        return false;
    }
    if ((typeName=="")&&(isArrayType==false)){
       // qCritical() << "didnt found type name in function: " << parent->name;
        return false;
    }
    return true;
}

bool RPCRuntimeParamterDescription::setTypeByString(QString typeName)
{
    if (typeName == "integer"){
        rpcParamType = RPCParamType_t::param_int;
        return true;
    }else if(typeName == "enum"){
        rpcParamType = RPCParamType_t::param_enum;
        return true;
    }else if(typeName == "array"){
        rpcParamType = RPCParamType_t::param_array;
        return true;
    }else if(typeName == "struct"){
        rpcParamType = RPCParamType_t::param_struct;
        return true;
    }else if(typeName == "character"){
        rpcParamType = RPCParamType_t::param_character;
        return true;
    }else{
        return false;
    }

}



