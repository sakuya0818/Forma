#include "Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <QCoreApplication>
#include <QDir>

Logger* Logger::instance() {
	static Logger logger;
	return &logger;
}

Logger::Logger(QObject* parent) : QObject(parent) {
	init();
}

void Logger::init() {
	QString logDir = QCoreApplication::applicationDirPath() + "/logs";
	QDir().mkpath(logDir);
	QString logPath = logDir + "/app.log";

	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(logPath.toStdString(), 0, 0);

	std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
	auto logger = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());

	logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
	logger->set_level(spdlog::level::debug);
	spdlog::register_logger(logger);
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

	emit logWritten(level, message);
}

void Logger::writeDetailed(const QString& func, const QString& file, int line, const QString& message, LogLevel level) {
	QString fileName = QFileInfo(file).fileName();
	QString fullMsg = QString("%1 %2(%3) | %4").arg(func).arg(fileName).arg(line).arg(message);
	write(fullMsg, level);
}
