#ifndef RPCWATCHPOINT_H
#define RPCWATCHPOINT_H

#include <QDateTime>
#include <QPair>
#include <QString>
#include <functional>

//TODO: What is a watchpoint, what does it do?

class RPCWatchPoint {
	public:
	using WatchCallBack = std::function<void(QString, QString, QPair<int, int>, QDateTime, int64_t)>;
	RPCWatchPoint() = default;
	RPCWatchPoint(QString FieldID, QString humanReadableName, QPair<int, int> plotIndex, WatchCallBack callback);

	QString FieldID;
	QString humanReadableName;
	QPair<int, int> plotIndex;
	WatchCallBack callback;

	void call(QDateTime timeStamp, int64_t val);

	private:
	bool valid = false;
};

#endif //RPCWATCHPOINT_H
