#ifndef RPCRUNTIMEFUNCTION_H
#define RPCRUNTIMEFUNCTION_H


#include<QString>
#include<QList>
#include<QPair>

enum class RPCParamType_t {param_int,param_enum,param_character};

class RPCRuntimeFunctionParamter
{
public:
    RPCRuntimeFunctionParamter();
    ~RPCRuntimeFunctionParamter();

    QString name;
    int elementBitLength;
    int bitPosition;
    int indexPosition;
    bool isSigned;
    QString typeName;
    QList<QPair<int,QString>> enumValues;

    bool setTypeByString(QString typeName);

    RPCParamType_t rpcParamType;
};

class RPCRuntimeFunction
{
public:
    RPCRuntimeFunction();
    ~RPCRuntimeFunction();

    QString name;
    QList<RPCRuntimeFunctionParamter> paramList;
    int ID;

};




#endif // RPCRUNTIMEFUNCTION_H
