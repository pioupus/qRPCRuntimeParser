#ifndef RPCRUNTIMEPROTOCOLDESCRIPTION_H
#define RPCRUNTIMEPROTOCOLDESCRIPTION_H

#include <QString>
#include <QStringList>
#include "rpcruntime_function.h"

/*
 * A RPCRunTimeProtocolDescription describes the Protocol created by the RPC-Generator loaded from an XML-File
*/

class RPCRunTimeProtocolDescription
{
public:
	bool openProtocolDescription(QString filename);

	uint32_t getRequestLength(uint8_t requestID) const; //TODO: what if we have variable length data?

	QList<RPCRuntimeFunction> getFunctionList() const;

	RPCRuntimeTransfer getTransferByID(int ID) const;

	RPCRuntimeParameterDescription getParamDescriptionByFieldID(QString FieldID);

	QString getFileName() const;

private:
    QList<RPCRuntimeFunction> functionList;
    QString fileName;

	RPCRuntimeParameterDescription getParameterDescriptionByFieldIDToken(QList<RPCRuntimeParameterDescription> &paramList, QStringList &IDToken, int index, bool isArray, int arrayElementCount);
};

#endif // RPCRUNTIMEINTERPRETER_H
