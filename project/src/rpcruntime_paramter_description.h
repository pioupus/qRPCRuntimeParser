#ifndef RPCRUNTIMEPARAMETERDESCRIPTION_H
#define RPCRUNTIMEPARAMETERDESCRIPTION_H

#include <QtXml>
#include <QList>
#include <QString>

enum class  RPCParamType_t {param_none,param_int,param_enum,param_character,param_array,param_struct};

class RPCRuntimeParameterDescription
{
public:
	RPCRuntimeParameterDescription();

	bool loadFromXML(QDomElement xmlParams);
	bool setTypeByString(QString typeName);

	QString name;
	QString typeName;
	QList<QPair<int,QString>> enumValues;
	QList<RPCRuntimeParameterDescription> subParameters;

	RPCRuntimeParameterDescription getParameterDescriptionByFieldIDToken(QStringList &IDToken, int index);

	int elementBitLength;
	int elementCount;
	int indexPosition;


	RPCParamType_t rpcParamType;
	bool isSigned;
public:

	void calcLength();
};

#endif // RPCRUNTIMEPARAMETERDESCRIPTION_H
