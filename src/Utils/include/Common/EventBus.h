#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>

class EventBus : public QObject
{
	Q_OBJECT
public:
	static EventBus& instance() {
		static EventBus bus;
		return bus;
	}

signals:
	void messagePosted(const QString& msg); // 事件：发布消息

private:
	EventBus() {}
	Q_DISABLE_COPY(EventBus) // 禁止拷贝
};

#endif // EVENTBUS_H
