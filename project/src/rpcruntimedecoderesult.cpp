#include "rpcruntimedecoderesult.h"

RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription protocolDescription)
{
    reply = false;
    this->protocolDescription = protocolDescription;
}

RPCRuntimeDecoder::~RPCRuntimeDecoder()
{

}

bool RPCRuntimeDecoder::isNull()
{
    return transfer.isNull();
}

bool RPCRuntimeDecoder::isReply()
{
    return reply;
}

void RPCRuntimeDecoder::setIsReply(bool reply)
{
    this->reply = reply;
}

QByteArray RPCRuntimeDecoder::decode(QByteArray inBuffer)
{
    QByteArray result = inBuffer;

    QList<RPCRuntimeFunction> functionList = protocolDescription.getFunctionList();

    uint8_t ID = inBuffer[0];

    for(int i=0; i < functionList.count();i++){
        RPCRuntimeFunction fun = functionList[i];
        if (fun.reply.ID == ID){
            name = fun.name;
            declaration = fun.declaration;
            transfer = fun.reply;
            setIsReply(true);
            result = inBuffer.mid(1);
            break;
        }else if(fun.request.ID == ID){
            name = fun.name;
            transfer = fun.request;
            declaration = fun.declaration;
            setIsReply(false);
            result = inBuffer.mid(1);
            break;
        }
    }

    if (!isNull()){
        result = decodeParams(result,transfer.paramList,decodedParams);
    }
    return result;
}

QStringList RPCRuntimeDecoder::printsubType(int tabDepth, QList<RPCRuntimeDecodedParam> decodedParamList, bool isArrayField ){
    QString line;
    QStringList result;
    QString tabPrefix = QString(tabDepth,'\t');

    bool standardView = true;//when array and element

    if ((isArrayField) && (decodedParamList.count() > 0)){
        if (decodedParamList[0].subParams.count() == 0){
            standardView = false;
        }
    }

    for (int i=0;i<decodedParamList.count();i++){
        RPCRuntimeDecodedParam decodedParam = decodedParamList[i];
        RPCRuntimeParamterDescription paramDesc = decodedParam.getParamDescription();
        QStringList subList;


        if (standardView){
            line = "";
        }
        if (isArrayField && standardView){
            line += "["+QString::number(i)+"]:";
        }
        if (!isArrayField){
            line += paramDesc.name+"("+paramDesc.typeName+"):\t";
        }

        if ((paramDesc.rpcParamType == RPCParamType_t::param_int) || (paramDesc.rpcParamType == RPCParamType_t::param_enum)){
            line += decodedParam.string+" ";
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_character){
            line += decodedParam.string;
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_array){
            subList.append(printsubType(tabDepth+1, decodedParam.subParams,true));
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_struct){
            subList = printsubType(tabDepth+1, decodedParam.subParams,false);

        }
        if (standardView){
            result.append(tabPrefix+line);
            result.append(subList);
        }
    }
    if (standardView == false){
        result.append(tabPrefix+line);
    }
    return result;
}

QStringList RPCRuntimeDecoder::getPrintableReport()
{
    QStringList result;
    QString line;
    if (isReply()){
        line = "Reply of: "+name;
    }else{
        line = "Request: "+name;
    }
    result.append(line);
    result.append("Function: "+declaration);
    result.append("");
    result.append(printsubType(0, decodedParams, false));
    return result;
}

QByteArray RPCRuntimeDecoder::decodeParams(QByteArray inBuffer, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams)
{
    for (int i = 0; i<paramDescriptionList.count();i++){
        RPCRuntimeParamterDescription paramDesc = paramDescriptionList[i];
        RPCRuntimeDecodedParam decParam(paramDesc);
        if (paramDesc.subParameters.count()){
            if (paramDesc.rpcParamType == RPCParamType_t::param_array){
                for(int element=0;element<paramDesc.elementCount;element++){
                    inBuffer = decodeParams(inBuffer, paramDesc.subParameters, decParam.subParams);
                }
                if ((paramDesc.subParameters.count() == 1)&&(paramDesc.subParameters[0].elementBitLength == 8)&&
                        ((paramDesc.subParameters[0].rpcParamType == RPCParamType_t::param_character) ||
                                                    (paramDesc.subParameters[0].rpcParamType == RPCParamType_t::param_int))){
                    int textLength = decParam.subParams.count();
                    for(int n=0;n<textLength;n++){
                        char c = (char)decParam.subParams[n].value;
                        if (c == '\0'){
                            break;
                        }
                        decParam.string.append(c);
                    }

                }
            }else{
                inBuffer = decodeParams(inBuffer, paramDesc.subParameters, decParam.subParams);
            }
        }else{
            inBuffer = decParam.decode(inBuffer);
        }

        decodedParams.append(decParam);
    }
    return inBuffer;
}



RPCRuntimeDecodedParam::RPCRuntimeDecodedParam(RPCRuntimeParamterDescription paramDescription)
{
    this->paramDescription = paramDescription;
}

RPCRuntimeDecodedParam::~RPCRuntimeDecodedParam()
{

}

QByteArray RPCRuntimeDecodedParam::decode(QByteArray inBuffer)
{
    value = 0;
    uint64_t val=0;
    int lengthInByte = paramDescription.elementBitLength/8;

    if (paramDescription.rpcParamType == RPCParamType_t::param_struct){
        lengthInByte = 0;
    }else{


        for(int i=0;i<lengthInByte; i++){
            uint8_t byte = inBuffer[i];
            val += ((uint64_t)byte * (1 << (8*i)));

        }

        if (paramDescription.rpcParamType == RPCParamType_t::param_int){

            if (paramDescription.isSigned ==false){
                if (lengthInByte==1)
                    string = "0x"+QString("%1").arg(val, 2, 16, QChar('0')).toUpper();
                else
                    string = QString::number(val);
                value = val;
            }else{
                uint64_t v = 0;
                for(size_t i=(size_t)lengthInByte;i<sizeof(uint64_t); i++){
                    v += ((uint64_t)0xFF * ((uint64_t)1 << 8*i));
                }
                val += v;
                value = (int64_t)val;
                string = QString::number(value);
            }
        }else if(paramDescription.rpcParamType == RPCParamType_t::param_character){
            value = val;
        }else if(paramDescription.rpcParamType == RPCParamType_t::param_enum){
            value = val;
            QList<QPair<int,QString>> enumValues = paramDescription.enumValues;
            string = "illegal enum value";
            for(int i = 0; i<enumValues.count();i++){
                QPair<int,QString> item = enumValues[i];
                if (item.first == value){
                    string = item.second;
                    break;
                }
            }
        }


    }
    return inBuffer.mid(lengthInByte);
}

RPCRuntimeParamterDescription RPCRuntimeDecodedParam::getParamDescription() const
{
    return paramDescription;
}



