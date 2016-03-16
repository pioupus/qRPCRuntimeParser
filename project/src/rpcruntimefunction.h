#ifndef RPCRUNTIMEFUNCTION_H
#define RPCRUNTIMEFUNCTION_H

#include <QtXml>
#include <memory>
#include <QString>
#include <QList>
#include <QPair>


enum class RPCParamType_t {param_none,param_int,param_enum,param_character,param_array,param_struct};

class RPCRuntimeParamterDescription
{
public:
    RPCRuntimeParamterDescription();
    ~RPCRuntimeParamterDescription();

    int elementBitLength;
    int elementCount;
    int indexPosition;

    QString name;
    QString typeName;
    RPCParamType_t rpcParamType;
    bool isSigned;

    QList<QPair<int,QString>> enumValues;

   // QList<std::shared_ptr<RPCRuntimeParamter>> subParamters;
    QList<RPCRuntimeParamterDescription> subParameters;
    bool loadFromXML(QDomElement xmlParams);

    bool setTypeByString(QString typeName);

public:

    void calcLength();
};

class RPCRuntimeTransfer
{
public:
    RPCRuntimeTransfer();
    ~RPCRuntimeTransfer();


    QList<RPCRuntimeParamterDescription> paramList;
    int ID;

    int getPackageLength();
    void calcPackagelength();
    bool loadParamListFromXML(QDomElement xmlParams);
    bool isNull() const;
    void setIsNull(bool value);

    int getTotalLength(void);

private:
    bool isPackageLenghCalced; //inited false
    bool empty;
};

class RPCRuntimeFunction
{
public:
    RPCRuntimeFunction();
    ~RPCRuntimeFunction();

    QString name;

    bool isNull();

    RPCRuntimeTransfer request;
    RPCRuntimeTransfer reply;

};






#endif // RPCRUNTIMEFUNCTION_H
