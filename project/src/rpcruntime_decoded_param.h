#ifndef RPCRUNTIMEDECODEDPARAM_H
#define RPCRUNTIMEDECODEDPARAM_H

#include <QByteArray>
#include "rpcruntime_paramter_description.h"

/*
 * TODO: What is a RPCRuntimeDecodedParam?
 */

class RPCRuntimeDecodedParam{
public:
	RPCRuntimeDecodedParam(RPCRuntimeParamterDescription paramDescription);
	RPCRuntimeDecodedParam();
	QByteArray decode(QByteArray inBuffer);
	QList<RPCRuntimeDecodedParam> subParams;
	RPCRuntimeParamterDescription getParamDescription() const;

	int64_t value;
	QString string;
	QString FieldID;
	bool isNull();

private:
	bool null;
	RPCRuntimeParamterDescription paramDescription;
};

#endif //RPCRUNTIMEDECODEDPARAM_H
