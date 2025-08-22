#include <QApplication>
#include <QScreen>
#include <spdlog/spdlog.h>
#include <fmt/chrono.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "MainWindow.h"
#include "ElaApplication.h"
//#include "utils/logger.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
	//根据实际屏幕缩放比例更改
	qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif
	QApplication a(argc, argv);
	eApp->init();

	spdlog::set_pattern("[%n] [%^%l%$] %v");
#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif
	//spdlog::set_default_logger(get_logger("Default"));

	std::tm now = fmt::localtime(std::time(nullptr));
	spdlog::info("Dandelion 3D, started at {:%Y-%m-%d %H:%M:%S%z}", now);

    MainWindow w;
    w.show();
    return a.exec();
}
