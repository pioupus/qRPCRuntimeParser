#ifndef RPCRUNTIMEDECODEDPARAM_H
#define RPCRUNTIMEDECODEDPARAM_H

#include <QByteArray>
#include "rpcruntime_paramter_description.h"

/*
 * TODO: What is a RPCRuntimeDecodedParam?
 */

class RPCRuntimeDecodedParam{
public:
	RPCRuntimeDecodedParam(RPCRuntimeParameterDescription paramDescription);
	RPCRuntimeDecodedParam();
	QByteArray decode(QByteArray inBuffer);
	QList<RPCRuntimeDecodedParam> subParams;
	RPCRuntimeParameterDescription getParamDescription() const;

	int64_t value;
	QString string;
	QString FieldID;
	bool isNull();

private:
	bool null;
	RPCRuntimeParameterDescription paramDescription;
};

#endif //RPCRUNTIMEDECODEDPARAM_H
