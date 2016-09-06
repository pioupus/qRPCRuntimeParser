#include "rpc_watch_point.h"

RPCWatchPoint::RPCWatchPoint(QString FieldID, QString humanReadableName, QPair<int, int> plotIndex, RPCWatchPoint::WatchCallBack callback)
{
	this->callback = callback;
	this->plotIndex = plotIndex;
	this->humanReadableName = humanReadableName;
	this->FieldID = FieldID;
	valid = true;
}

void RPCWatchPoint::call(QDateTime timeStamp, int64_t val)
{
	callback(FieldID,humanReadableName,plotIndex,timeStamp,val);
}
