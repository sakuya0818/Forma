#ifndef LOGGER_H
#define LOGGER_H

#include "Utils_global.h"

#include <QObject>
#include <QString>

/**************************************************************************
 * 模块名称：spdlog日志封装模块                                           *
 *                                                                        *
 * 功能描述：                                                             *
 *   - 基于 spdlog 的多线程模式，确保线程安全                             *
 *   - 同时支持两种日志输出形式：终端日志与每日日志文件                   *
 *                                                                        *
 * 使用说明：                                                             *
 *   - 调用 InitLogger() 初始化日志系统                                   *
 *   - 可根据日志类型输出不同颜色（终端）                                 *
 *   - 每日自动生成新日志文件，自动清理超过30份的旧日志                   *
 *   - 默认日志路径：执行文件目录下 /DailyLogs/daily.txt                  *
 *                                                                        *
 * 日志类型支持：                                                         *
 *   - debug、info、warn、error 等分类                                    *
 *   - 支持信号槽调用方式                                                 *
 **************************************************************************/

#define OS_TraceLogInfo(level, msg) \
    Logger::getInstance()->writeLog(__FUNCTION__, __FILE__, __LINE__, msg, level)

#define OS_DebugLog(msg) OS_TraceLogInfo(LogLevel::Debug, msg)
#define OS_InfoLog(msg)  OS_TraceLogInfo(LogLevel::Info, msg)
#define OS_WarnLog(msg)  OS_TraceLogInfo(LogLevel::Warn, msg)
#define OS_ErrorLog(msg) OS_TraceLogInfo(LogLevel::Error, msg)

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
	void writeLog(const QString& func, const QString& file, int line, const QString& message, LogLevel level);

signals:
	void sigShowLogInfo(LogLevel level, QString message);

private:
	explicit Logger(QObject* parent = nullptr);
	void init();
};

#endif