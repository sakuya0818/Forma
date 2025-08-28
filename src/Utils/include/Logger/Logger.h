#ifndef LOGGER_H
#define LOGGER_H

#include "Utils_global.h"

#include <QObject>
#include <QString>

/**************************************************************************
 * ģ�����ƣ�spdlog��־��װģ��                                           *
 *                                                                        *
 * ����������                                                             *
 *   - ���� spdlog �Ķ��߳�ģʽ��ȷ���̰߳�ȫ                             *
 *   - ͬʱ֧��������־�����ʽ���ն���־��ÿ����־�ļ�                   *
 *                                                                        *
 * ʹ��˵����                                                             *
 *   - ���� InitLogger() ��ʼ����־ϵͳ                                   *
 *   - �ɸ�����־���������ͬ��ɫ���նˣ�                                 *
 *   - ÿ���Զ���������־�ļ����Զ�������30�ݵľ���־                   *
 *   - Ĭ����־·����ִ���ļ�Ŀ¼�� /DailyLogs/daily.txt                  *
 *                                                                        *
 * ��־����֧�֣�                                                         *
 *   - debug��info��warn��error �ȷ���                                    *
 *   - ֧���źŲ۵��÷�ʽ                                                 *
 **************************************************************************/

#define LOG_TRACE(level, msg) \
    Logger::getInstance()->writeDetailed(__FUNCTION__, __FILE__, __LINE__, msg, level)

#define LOG_DEBUG(msg) LOG_TRACE(LogLevel::Debug, msg)
#define LOG_INFO(msg)  LOG_TRACE(LogLevel::Info, msg)
#define LOG_WARN(msg)  LOG_TRACE(LogLevel::Warn, msg)
#define LOG_ERROR(msg) LOG_TRACE(LogLevel::Error, msg)

enum class LogLevel {
	Debug = 0,
	Info,
	Warn,
	Error
};

class UTILS_EXPORT Logger : public QObject {
	Q_OBJECT
public:
	static Logger* getInstance();

	void write(const QString& message, LogLevel level = LogLevel::Info);
	void writeDetailed(const QString& func, const QString& file, int line, const QString& message, LogLevel level);

signals:
	void logWritten(LogLevel level, QString message);

private:
	explicit Logger(QObject* parent = nullptr);
	void init();
};

#endif