#include "rpcruntimefunction.h"

RPCRuntimeFunction::RPCRuntimeFunction()
{

}

RPCRuntimeFunction::~RPCRuntimeFunction()
{

}

RPCRuntimeFunctionParamter::RPCRuntimeFunctionParamter()
{

}

RPCRuntimeFunctionParamter::~RPCRuntimeFunctionParamter()
{

}

bool RPCRuntimeFunctionParamter::setTypeByString(QString typeName)
{
    if (typeName == "integer"){
        rpcParamType = RPCParamType_t::param_int;
        return true;
    }else if(typeName == "enum"){
        rpcParamType = RPCParamType_t::param_enum;
        return true;
    }else{
        return false;
    }

}



