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
	void messagePosted(const QString& msg); // �¼���������Ϣ

private:
	EventBus() {}
	Q_DISABLE_COPY(EventBus) // ��ֹ����
};

#endif // EVENTBUS_H
