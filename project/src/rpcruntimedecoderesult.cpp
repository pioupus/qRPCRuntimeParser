#include "rpcruntimedecoderesult.h"

RPCRuntimeDecodeResult::RPCRuntimeDecodeResult()
{
    reply = false;
}

RPCRuntimeDecodeResult::~RPCRuntimeDecodeResult()
{

}

bool RPCRuntimeDecodeResult::isNull()
{
    return transfer.isNull();
}

bool RPCRuntimeDecodeResult::isReply()
{
    return reply;
}

void RPCRuntimeDecodeResult::setIsReply(bool reply)
{
    this->reply = reply;
}

