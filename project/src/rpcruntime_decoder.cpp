#include "rpcruntime_decoder.h"
#include "channel_codec/channel_codec.h"
#include "channel_codec/channel_codec_config.h"

#include <assert.h>
#include "rpc_transmission/client/generated_general/RPC_types.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_network.h"
#include "rpc_transmission/server/generated_general/RPC_TRANSMISSION_parser.h"
#include "errorlogger/generic_eeprom_errorlogger.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//RPCRuntimeDecoder* sendingClassInstance = NULL;

static RPC_SIZE_RESULT get_transmission_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){
    int transferSize=0;

    assert(instance->aux.RPCRuntimeDecoderInstance);
    RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;

    const unsigned char *current = (const unsigned char *)buffer;
    RPC_SIZE_RESULT returnvalue;

    if (size_bytes == 0){
        returnvalue.result = RPC_COMMAND_INCOMPLETE;
        returnvalue.size = 1;
        return returnvalue;
    }

    transferSize = sendingClass->getTransferLength(*current);

    if (transferSize > 0){
        returnvalue.size = transferSize;
    }else{
        returnvalue.size = 0;
        returnvalue.result = RPC_COMMAND_UNKNOWN;
        return returnvalue;
    }

    returnvalue.result = returnvalue.size > size_bytes ? RPC_COMMAND_INCOMPLETE : RPC_SUCCESS;

    return returnvalue;
}

static void parse_transmission(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){

    assert(instance->aux.RPCRuntimeDecoderInstance);
    RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;

    QByteArray inBuffer = QByteArray((char*)buffer,(int)size_bytes);
    sendingClass->RPCDecodeRPCData(inBuffer);
}

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t errNum){
    //assert(sendingClassInstance);
    assert(instance->aux.RPCRuntimeDecoderInstance);
    RPCRuntimeDecoder* sendingClass = instance->aux.RPCRuntimeDecoderInstance;
    if (errNum == errlog_W_CHCODEC_RX_CRC_fail){
        sendingClass->setErrorCRCHappened(true);
    }else{
        sendingClass->setErrorChannelCodecHappened(true);
    }
}

EXTERNC RPC_RESULT phyPushDataBuffer(channel_codec_instance_t *instance, const char *buffer, size_t length){
    assert(instance->aux.RPCRuntimeDecoderInstance);

    QByteArray codecOutput = QByteArray(buffer,(int)length);

    instance->aux.RPCRuntimeDecoderInstance->setChannelCodecOutput(codecOutput);
    RPC_RESULT result = RPC_SUCCESS;

    return result;
}

EXTERNC void RPC_CHANNEL_CODEC_parser_init(channel_codec_instance_t *instance){
    (void)instance;

}

EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_answer_length(channel_codec_instance_t *instance, const void *buffer, size_t size){
    return  get_transmission_size(instance,buffer,  size);
}


EXTERNC RPC_SIZE_RESULT RPC_CHANNEL_CODEC_get_request_size(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes){
    return  get_transmission_size(instance,buffer,  size_bytes);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_answer(channel_codec_instance_t *instance, const void *buffer, size_t size){
    parse_transmission(instance,buffer,size);
}

EXTERNC void RPC_CHANNEL_CODEC_parse_request(channel_codec_instance_t *instance, const void *buffer, size_t size_bytes ){
    parse_transmission(instance,buffer,size_bytes);
}


RPCRuntimeDecoder::RPCRuntimeDecoder(RPCRunTimeProtocolDescription protocolDescription)
{
    this->protocolDescription = protocolDescription;
    clear();
}

RPCRuntimeDecoder::RPCRuntimeDecoder()
{
    clear();
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

int RPCRuntimeDecoder::getTransferLength(uint8_t ID)
{
    RPCRuntimeFunction fun = getFunctionByID(ID);
    RPCRuntimeTransfer transfer = getTransfer(fun,reply);
    if (transfer.isNull()){
        return -1;
    }else{
        return transfer.getTotalLength();
    }
}

void RPCRuntimeDecoder::setTimeStamp(QDateTime timeStamp)
{
    this->timeStamp = timeStamp;
}

RPCRuntimeTransfer RPCRuntimeDecoder::getTransfer(RPCRuntimeFunction fun, bool isReply){
    if (isReply){
        return fun.reply;
    }else {
        return fun.request;
    }
}
bool RPCRuntimeDecoder::getErrorChannelCodecHappened() const
{
    return errorChannelCodecHappened;
}

void RPCRuntimeDecoder::setErrorChannelCodecHappened(bool value)
{
    errorChannelCodecHappened = value;
}

bool RPCRuntimeDecoder::fieldExists(QString FieldID)
{
    return !getDecodedParamByFieldID(FieldID).isNull();
}

RPCRuntimeTransfer RPCRuntimeDecoder::getDecodedTransferByFieldID(QString FieldID){
    RPCRuntimeTransfer result;
        QStringList IDToken = FieldID.split("?");
    if (IDToken.count() < 2){
        return result;
    }
    if (IDToken[0] != protocolDescription.getFileName()){
        return result;
    }
    if(IDToken[1].toInt() != transfer.ID){
        return result;
    }
    return transfer;
}

RPCRuntimeParamterDescription RPCRuntimeDecoder::getParamDescriptionByFieldID(QString FieldID)
{
    return protocolDescription.getParamDescriptionByFieldID(FieldID);
}

RPCRuntimeDecodedParam RPCRuntimeDecoder::getDecodedParamByFieldID(QString FieldID){
    RPCRuntimeDecodedParam result;
    RPCRuntimeDecodedParam result_none;
    QStringList IDToken = FieldID.split("?");
    if (IDToken.count() < 3){
        return result_none;
    }
    if (IDToken[0] != protocolDescription.getFileName()){
        return result_none;
    }
    if(IDToken[1].toInt() != transfer.ID){
        return result_none;
    }

    QList<RPCRuntimeDecodedParam> subParams=decodedParams;
    for(int i=2;i<IDToken.count();i++){
        bool ok;
        int FieldID_token = IDToken[i].toInt(&ok);
        if (!ok){
            return result_none;
        }
        if (FieldID_token >= subParams.count()){
            return result_none;
        }
        if (i == IDToken.count()-1){
            return subParams[FieldID_token];
        }else{
            subParams = subParams[FieldID_token].subParams;
        }

    }
    return result;
}

bool RPCRuntimeDecoder::getErrorCRCHappened() const
{
    return errorCRCHappened;
}

void RPCRuntimeDecoder::setErrorCRCHappened(bool value)
{
    errorCRCHappened = value;
}


bool RPCRuntimeDecoder::isReplyByID(uint8_t ID, RPCRuntimeFunction fun ){
    if (fun.reply.ID==ID){
        return true;
    }else {
        return false;
    }
}

RPCRuntimeFunction RPCRuntimeDecoder::getFunctionByID(uint8_t ID ){
    QList<RPCRuntimeFunction> functionList = protocolDescription.getFunctionList();
    RPCRuntimeFunction fun;
    for(int i=0; i < functionList.count();i++){
        fun = functionList[i];
        if (fun.reply.ID == ID){
            return fun;
        }else if(fun.request.ID == ID){
            return fun;
        }
    }
    return fun;
}

void RPCRuntimeDecoder::clear(){
    RPCRuntimeTransfer emptyTransfer;
    decodedParams.clear();
    transfer = emptyTransfer;
    name = "";
    declaration = "";
    reply = false;
    errorChannelCodecHappened = false;
    errorCRCHappened = false;

}

QByteArray RPCRuntimeDecoder::RPCDecodeRPCData(QByteArray inBuffer)
{

    QByteArray result = inBuffer;

    uint8_t ID = inBuffer[0];
    clear();

    QString FieldID = protocolDescription.getFileName()+"?"+QString::number(ID)+"?";

    RPCRuntimeFunction fun = getFunctionByID(ID);

    if (!fun.isNull()){
        reply = isReplyByID(ID,fun);
        transfer = getTransfer(fun,reply);
        //qDebug() << "transfer null:" << transfer.isNull();
        name = fun.name;
        declaration = fun.declaration;
        result = inBuffer.mid(1);
    }

    if (!transfer.isNull()){
        result = decodeParams(result,FieldID,"", transfer.paramList,decodedParams);
    }
    QList<RPCWatchPoint> watchPointList = getWatchPointList();
    for(auto wp : watchPointList){
        auto dp = getDecodedParamByFieldID(wp.FieldID);
        if(!dp.isNull()){
            wp.call(timeStamp,dp.value);
        }else{
            auto tr = getDecodedTransferByFieldID(wp.FieldID);
            if(!tr.isNull()){
                wp.call(timeStamp,0);
            }
        }
    }

    return result;
}

QByteArray RPCRuntimeDecoder::decodeParams(QByteArray inBuffer, QString FieldID, QString OverwriteID, QList<RPCRuntimeParamterDescription> paramDescriptionList, QList<RPCRuntimeDecodedParam> &decodedParams)
{
    for (int i = 0; i<paramDescriptionList.count();i++){
        RPCRuntimeParamterDescription paramDesc = paramDescriptionList[i];
        RPCRuntimeDecodedParam decParam(paramDesc);
        if (paramDesc.subParameters.count()){
            if (paramDesc.rpcParamType == RPCParamType_t::param_array){
                for(int element=0;element<paramDesc.elementCount;element++){
                    inBuffer = decodeParams(inBuffer,FieldID+QString::number(i)+"?",QString::number(element),  paramDesc.subParameters, decParam.subParams);
                }
                if ((paramDesc.subParameters.count() == 1)&&(paramDesc.subParameters[0].elementBitLength == 8)&&
                        ((paramDesc.subParameters[0].rpcParamType == RPCParamType_t::param_character) ||
                                                    (paramDesc.subParameters[0].rpcParamType == RPCParamType_t::param_int))){
                    int textLength = decParam.subParams.count();
                    for(int n=0;n<textLength;n++){
                        char c = (char)decParam.subParams[n].value;
                        if (paramDesc.subParameters[0].rpcParamType == RPCParamType_t::param_character){
                            if (c == '\0'){
                                break;
                            }
                            decParam.string.append(c);
                        }else{
                            decParam.string.append("0x"+QString("%1 ").arg(c, 2, 16, QChar('0')));
                        }
                    }

                    decParam.string = decParam.string.trimmed();
                }
            }else{

                inBuffer = decodeParams(inBuffer, FieldID+QString::number(i)+"?","", paramDesc.subParameters, decParam.subParams);
            }
        }else{
            inBuffer = decParam.decode(inBuffer);
        }
        if (OverwriteID == ""){
            decParam.FieldID = FieldID+QString::number(i);
        }else{
            decParam.FieldID = FieldID+OverwriteID;
        }
        decodedParams.append(decParam);
    }
    return inBuffer;
}

void RPCRuntimeDecoder::RPCDecodeChannelCodedData(QByteArray inBuffer)
{
    //sendingClassInstance = this;

    channel_codec_instance[channel_codec_comport_uart].aux.portindex = channel_codec_comport_uart;
    channel_codec_instance[channel_codec_comport_uart].aux.RPCRuntimeDecoderInstance = this;

    channel_init_instance(&channel_codec_instance[channel_codec_comport_uart],
                                     channel_codec_rxbuffer[channel_codec_comport_uart],CHANNEL_CODEC_RX_BUFFER_SIZE,
                                     channel_codec_txbuffer[channel_codec_comport_uart],CHANNEL_CODEC_TX_BUFFER_SIZE);

    for(int i=0;i<inBuffer.count();i++){
        channel_push_byte_to_RPC(&channel_codec_instance[channel_codec_comport_uart],inBuffer[i]);
    }

   // sendingClassInstance = NULL;

}

QByteArray RPCRuntimeDecoder::encodeToChannelCodedData(QByteArray inBuffer)
{
   // sendingClassInstance = this;
    channel_codec_instance[channel_codec_comport_uart].aux.portindex = channel_codec_comport_uart;
    channel_codec_instance[channel_codec_comport_uart].aux.RPCRuntimeDecoderInstance = this;

    channel_init_instance(&channel_codec_instance[channel_codec_comport_uart],
                                     channel_codec_rxbuffer[channel_codec_comport_uart],CHANNEL_CODEC_RX_BUFFER_SIZE,
                                     channel_codec_txbuffer[channel_codec_comport_uart],CHANNEL_CODEC_TX_BUFFER_SIZE);

    channel_start_message_from_RPC(&channel_codec_instance[channel_codec_comport_uart],inBuffer.length());
    for(int i = 0; i< inBuffer.count();i++){
        channel_push_byte_from_RPC(&channel_codec_instance[channel_codec_comport_uart],inBuffer[i]);
    }
    channel_commit_from_RPC(&channel_codec_instance[channel_codec_comport_uart]);


    //sendingClassInstance = NULL;
    return codecOutput;
}



QStringList RPCRuntimeDecoder::getPrintableReport()
{
    QStringList result;

    QString line ;
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

QList<QTreeWidgetItem *> RPCRuntimeDecoder::getTreeWidgetReport_recursive(QTreeWidgetItem * parent,QList<RPCRuntimeDecodedParam> decodedParamList, bool isArrayField)
{
    QString strName;
    QString strVal;
    bool standardView = true;//when array and element
    if ((isArrayField) && (decodedParamList.count() > 0)){
        if (decodedParamList[0].subParams.count() == 0){
            standardView = false;
        }
    }


    QStringList nameValList;
    nameValList.append("");
    nameValList.append("");

    QTreeWidgetItem* treeItem = parent;
    QList<QTreeWidgetItem *> result;

    for (int i=0;i<decodedParamList.count();i++){


        RPCRuntimeDecodedParam decodedParam = decodedParamList[i];
        RPCRuntimeParamterDescription paramDesc = decodedParam.getParamDescription();

        QList<QTreeWidgetItem *> subList;


        if (standardView){
            strName = "";
            strVal = "";
        }
        if (isArrayField && standardView){
            strName += "["+QString::number(i)+"]";
        }
        if (!isArrayField){
            strName += paramDesc.name+"("+paramDesc.typeName+")";
        }

        if ((paramDesc.rpcParamType == RPCParamType_t::param_int) || (paramDesc.rpcParamType == RPCParamType_t::param_enum)){
            strVal += decodedParam.string+" ";
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_character){
            strVal += decodedParam.string;
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_array){
            treeItem = new  QTreeWidgetItem(parent,nameValList);
            //treeItem = new  QTreeWidgetItem((QTreeWidgetItem*)NULL,nameValList);
            treeItem->setText(0,strName);
            treeItem->setData(0,Qt::UserRole,decodedParam.FieldID);
            subList.append(getTreeWidgetReport_recursive(treeItem,decodedParam.subParams, true));
        }else if (paramDesc.rpcParamType == RPCParamType_t::param_struct){
            treeItem = new  QTreeWidgetItem(parent,nameValList);
            //treeItem = new  QTreeWidgetItem((QTreeWidgetItem*)NULL,nameValList);
            treeItem->setText(0,strName);
            treeItem->setData(0,Qt::UserRole,decodedParam.FieldID);
            subList.append(getTreeWidgetReport_recursive(treeItem,decodedParam.subParams, false));
        }
        if (standardView){
            if (subList.count()){
                treeItem->addChildren(subList);
            }else{
                treeItem = new  QTreeWidgetItem(parent,nameValList);
                //treeItem = new  QTreeWidgetItem((QTreeWidgetItem*)NULL,nameValList);
                treeItem->setText(0,strName);
                treeItem->setText(1,strVal.trimmed());
                treeItem->setData(0,Qt::UserRole,decodedParam.FieldID);
            }
            result.append(treeItem);
        }
    }
    if (standardView == false){
        treeItem->setText(1,strVal.trimmed());
        result.append(treeItem);
    }
    return result;
}

QList<QTreeWidgetItem *> RPCRuntimeDecoder::getTreeWidgetReport(QTreeWidgetItem * parent=0)
{
    QList<QTreeWidgetItem *> items;
    QStringList nameValList;
    nameValList.append("");
    nameValList.append("");
    QTreeWidgetItem *treeItem1 = new  QTreeWidgetItem(parent,nameValList);

    treeItem1->setText(0,name);
    treeItem1->setText(1,declaration);

    QTreeWidgetItem *treeItem2 = new  QTreeWidgetItem(treeItem1,nameValList);

    if (isReply()){
        treeItem2->setText(0,"Reply");
    }else{
        treeItem2->setText(0,"Request");
    }


    getTreeWidgetReport_recursive(treeItem2,decodedParams, false);
    items.append(treeItem1);
    treeItem1->setExpanded(true);
    return items;
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


void RPCRuntimeDecoder::setChannelCodecOutput(QByteArray codecOutput)
{
    this->codecOutput = codecOutput;
}



void RPCRuntimeDecoder::addWatchPoint(QString FieldID, QString humanReadableName, QPair<int,int> plotIndex, RPCWatchPoint::WatchCallBack callback)
{

    RPCWatchPoint wp(FieldID,humanReadableName,plotIndex,callback);
    if (getParamDescriptionByFieldID(FieldID).rpcParamType == RPCParamType_t::param_int){
        watchpointList.append(wp);
    }
}

void RPCRuntimeDecoder::removeWatchPoint(QString FieldID)
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
}

void RPCRuntimeDecoder::clearWatchPoint()
{
    watchpointList.clear();
}

QList<RPCWatchPoint> RPCRuntimeDecoder::getWatchPointList()
{
    return watchpointList;
}
