#include <QString>
#include <QPair>
#include <QDateTime>
#include <functional>

class RPCWatchPoint{
public:
	using WatchCallBack = std::function<void(QString, QString, QPair<int,int>, QDateTime, int64_t)>;
	RPCWatchPoint() = default;
	RPCWatchPoint(QString FieldID,QString humanReadableName,QPair<int,int> plotIndex, WatchCallBack callback);

	QString FieldID;
	QString humanReadableName;
	QPair<int,int> plotIndex;
	WatchCallBack callback;

	void call(QDateTime timeStamp, int64_t val);
private:
	bool valid = false;
};
