#include "Logger/Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

// 懒汉式单例模式
Logger& Logger::getInstance() {
	static Logger logger;
	return logger;
}

Logger::Logger(QObject* parent) : QObject(parent) {
	init();
}

void Logger::init() {
	try
	{
		QString logDir = QCoreApplication::applicationDirPath() + "/logs";
		QDir().mkpath(logDir);
		QString logPath = logDir + "/daily.log";

		// 创建控制台日志输出
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_color_mode(spdlog::color_mode::always);			// 强制启用颜色
		// %^ 表示颜色开始，% $ 表示颜色结束，中间的内容会根据日志等级自动设置颜色（info 是绿色，warn 是黄色，error 是红色）
		console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");

		// 创建每日文件日志输出，保留最近30天的日志
		auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(logPath.toStdString(), 0, 0);
		file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

		auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

		// 创建一个名为 "main" 的 logger，同时使用两个 sink：控制台 + 文件
		std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink, msvc_sink };
		auto logger = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());

		// 设置日志等级为 debug，并注册到 spdlog 的全局 logger 管理器中
		logger->set_level(spdlog::level::debug);
		spdlog::register_logger(logger);
	}
	catch (const spdlog::spdlog_ex& ex) {
		qDebug() << "SpdLoger init failed: " << ex.what();
	}
}

void Logger::write(const QString& message, LogLevel level) {
	auto logger = spdlog::get("main");
	if (!logger) return;

	std::string msg = message.toStdString();
	switch (level) {
	case LogLevel::Debug: logger->debug(msg); break;
	case LogLevel::Info:  logger->info(msg); break;
	case LogLevel::Warn:  logger->warn(msg); break;
	case LogLevel::Error: logger->error(msg); break;
	}

	emit sigShowLogInfo(level, message);
}

void Logger::writeLog(const QString& func, const QString& file, int line, const QString& message, LogLevel level) {
	QString fileName = QFileInfo(file).fileName();
	QString fullMsg = QString("%1 %2(%3) | %4").arg(func).arg(fileName).arg(line).arg(message);
	write(fullMsg, level);
}

void Logger::setLevel(LogLevel level)
{
	auto logger = spdlog::get("main");
	if (!logger) return;

	logger->set_level(static_cast<spdlog::level::level_enum>(level));
}
