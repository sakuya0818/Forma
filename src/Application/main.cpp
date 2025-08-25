#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "MainWindow.h"
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
	// 设置不同 Qt 版本的高DPI支持
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

	// 设置OpenGL版本
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
		// 桌面 OpenGL（Core Profile）
		qDebug("Requesting 3.3 core context");
		fmt.setVersion(3, 3);
		fmt.setProfile(QSurfaceFormat::CoreProfile);
		fmt.setOption(QSurfaceFormat::DeprecatedFunctions, false); // 禁用旧函数
	}
	else {
		// OpenGL ES
		qDebug("Requesting 3.0 context");
		fmt.setVersion(3, 0);
		fmt.setRenderableType(QSurfaceFormat::OpenGLES); // 显式声明为 ES
	}
	QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.show();

    return a.exec();
}
