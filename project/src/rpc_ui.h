#ifndef RPCUI_H
#define RPCUI_H

#include "rpcruntime_decoded_function_call.h"

#include <QList>
#include <QTreeWidgetItem>
#include <QStringList>

#include <memory>

std::unique_ptr<QTreeWidgetItem> getTreeWidgetReport(const RPCRuntimeDecodedFunctionCall &function);

QStringList get_report(const RPCRuntimeDecodedFunctionCall &function);

#endif //RPCUI_H
