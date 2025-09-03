#include "MainObject.h"
#include "../MainWindow.h"
#include "SplashScreen.h"
#include <QDebug>


MainObject::MainObject(QObject* parent) :
	QObject(parent)
{
}

MainObject::~MainObject()
{
	delete m_mainWindow;
}

void MainObject::setInit()
{
	SplashScreen::getInstance()->setStagePercent(0, "初始化应用程序 ...");
	m_mainWindow = new MainWindow(nullptr);
	SplashScreen::getInstance()->setStart(m_mainWindow, "Forma", QString(":/Resource/Image/Forma.png"));
	SplashScreen::getInstance()->setStagePercent(40, "初始化主界面 ...");
	SplashScreen::getInstance()->setStagePercent(80, "加载界面 ...");
	SplashScreen::getInstance()->setStagePercent(100, "加载完毕！");
	SplashScreen::getInstance()->setFinish();

	m_mainWindow->show();
}