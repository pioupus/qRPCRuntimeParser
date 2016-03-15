#ifndef RPCRUNTIMEDECODERESULT_H
#define RPCRUNTIMEDECODERESULT_H
#include "rpcruntimedecoderesult.h"
#include "rpcruntimefunction.h"

class RPCRuntimeDecodeResult
{
public:
    RPCRuntimeDecodeResult();
    ~RPCRuntimeDecodeResult();
    bool isNull();

    RPCRuntimeTransfer transfer;

    QString name;
    bool isReply();
    void setIsReply(bool reply);


private:
    bool reply;

};

#endif // RPCRUNTIMEDECODERESULT_H
