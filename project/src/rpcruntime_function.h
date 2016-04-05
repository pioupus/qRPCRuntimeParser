#ifndef RPCRUNTIMEFUNCTION_H
#define RPCRUNTIMEFUNCTION_H

#include <QtXml>
#include <memory>
#include <QString>
#include <QList>
#include <QPair>


enum class  RPCParamType_t {param_none,param_int,param_enum,param_character,param_array,param_struct};

class RPCRuntimeParamterDescription
{
public:
    RPCRuntimeParamterDescription();
    ~RPCRuntimeParamterDescription();

    bool loadFromXML(QDomElement xmlParams);
    bool setTypeByString(QString typeName);

    QString name;
    QString typeName;
    QList<QPair<int,QString>> enumValues;
    QList<RPCRuntimeParamterDescription> subParameters;

    RPCRuntimeParamterDescription getParameterDescriptionByFieldIDToken(QStringList &IDToken, int index);

    int elementBitLength;
    int elementCount;
    int indexPosition;


    RPCParamType_t rpcParamType;
    bool isSigned;
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
    bool isReply();
    void setReply(bool r);
    QString getName();
    void setName(QString n);
private:
    bool isPackageLenghCalced; //inited false
    bool empty;
    bool reply;
    QString name;
};

class RPCRuntimeFunction
{
public:
    RPCRuntimeFunction();
    ~RPCRuntimeFunction();

    QString name;
    QString declaration;

    bool isNull();

    RPCRuntimeTransfer request;
    RPCRuntimeTransfer reply;

};






#endif // RPCRUNTIMEFUNCTION_H
