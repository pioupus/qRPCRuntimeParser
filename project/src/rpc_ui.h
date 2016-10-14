#ifndef RPCUI_H
#define RPCUI_H

class RPCRuntimeDecodedFunctionCall;
class RPCRuntimeEncodedFunctionCall;

#include <QList>
#include <QStringList>
#include <QTreeWidgetItem>

#include <memory>

std::unique_ptr<QTreeWidgetItem> getTreeWidgetReport(const RPCRuntimeDecodedFunctionCall &function);

std::unique_ptr<QTreeWidgetItem> getTreeWidgetReport(const RPCRuntimeEncodedFunctionCall &function);

QStringList get_report(const RPCRuntimeDecodedFunctionCall &function);

#endif //RPCUI_H
