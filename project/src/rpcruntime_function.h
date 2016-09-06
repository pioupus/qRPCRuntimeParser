#ifndef RPCRUNTIMEFUNCTION_H
#define RPCRUNTIMEFUNCTION_H

#include "rpcruntime_paramter_description.h"

#include <memory>
#include <QString>
#include <QList>
#include <QPair>

class RPCRuntimeTransfer
{
public:
	RPCRuntimeTransfer();

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
