#ifndef RPCRUNTIMEFUNCTION_H
#define RPCRUNTIMEFUNCTION_H

#include "rpcruntime_transfer.h"

#include <QString>

class RPCRuntimeFunction
{
public:
	QString name;
	QString declaration;

	bool isNull();

	RPCRuntimeTransfer request;
	RPCRuntimeTransfer reply;
};

#endif // RPCRUNTIMEFUNCTION_H
