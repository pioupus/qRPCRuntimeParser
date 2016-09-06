#ifndef RPCRUNTIMEPARAMTERDESCRIPTION_H
#define RPCRUNTIMEPARAMTERDESCRIPTION_H

#include <QtXml>
#include <QList>
#include <QString>

enum class  RPCParamType_t {param_none,param_int,param_enum,param_character,param_array,param_struct};

class RPCRuntimeParamterDescription
{
public:
	RPCRuntimeParamterDescription();

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

#endif // RPCRUNTIMEPARAMTERDESCRIPTION_H
