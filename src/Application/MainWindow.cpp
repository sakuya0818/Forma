#include "MainWindow.h"

#include <QDebug>
#include <QHBoxLayout>

#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaEventBus.h"
#include "ElaLog.h"
#include "ElaMenu.h"
#include "ElaMenuBar.h"
#include "ElaProgressBar.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"

MainWindow::MainWindow(QWidget *parent)
	: ElaWindow(parent)
{
	//初始化窗口
    initWindow();
    //额外布局
    initEdgeLayout();

    // 拦截默认关闭事件
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
        });
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
        });

    //移动到中心
    moveToCenter();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWindow()
{
	// 设置标题栏图标、标题，并初始化窗口大小
    setWindowIcon(QIcon(":/Resource/Image/Forma.png"));
	setWindowTitle("Forma");
    resize(1200, 740);
	// 设置信息卡图标、标题和副标题
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Forma.png"));
    setUserInfoCardTitle("Forma");
    setUserInfoCardSubTitle("A cross-platform 3D graphics framework based on QT.");
}

void MainWindow::initEdgeLayout()
{
    //菜单栏
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    // this->setMenuBar(menuBar);
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);
    this->setCustomWidgetMaximumWidth(500);

    menuBar->addElaIconAction(ElaIconType::AtomSimple, "动作菜单");
    ElaMenu* iconMenu = menuBar->addMenu(ElaIconType::Aperture, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::SelectAll);
    iconMenu->addElaIconAction(ElaIconType::Copy, "复制");
    iconMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addSeparator();
    ElaMenu* shortCutMenu = new ElaMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Find);
    shortCutMenu->addElaIconAction(ElaIconType::Copy, "复制");
    shortCutMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    shortCutMenu->addSeparator();
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addMenu(shortCutMenu);

    menuBar->addMenu("样例菜单(&B)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&C)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&E)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&F)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
	menuBar->addMenu("样例菜单(&G)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");

	//工具栏
	ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
	toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	toolBar->setToolBarSpacing(3);
	toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolBar->setIconSize(QSize(25, 25));
	// toolBar->setFloatable(false);
	// toolBar->setMovable(false);
	ElaToolButton* toolButton1 = new ElaToolButton(this);
	toolButton1->setElaIcon(ElaIconType::BadgeCheck);
	toolBar->addWidget(toolButton1);
	ElaToolButton* toolButton2 = new ElaToolButton(this);
	toolButton2->setElaIcon(ElaIconType::ChartUser);
	toolBar->addWidget(toolButton2);
	toolBar->addSeparator();
	ElaToolButton* toolButton3 = new ElaToolButton(this);
	toolButton3->setElaIcon(ElaIconType::Bluetooth);
	toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolButton3->setText("Bluetooth");
	toolBar->addWidget(toolButton3);
	ElaToolButton* toolButton4 = new ElaToolButton(this);
	toolButton4->setElaIcon(ElaIconType::BringFront);
	toolBar->addWidget(toolButton4);
	toolBar->addSeparator();
	ElaToolButton* toolButton5 = new ElaToolButton(this);
	toolButton5->setElaIcon(ElaIconType::ChartSimple);
	toolBar->addWidget(toolButton5);
	ElaToolButton* toolButton6 = new ElaToolButton(this);
	toolButton6->setElaIcon(ElaIconType::FaceClouds);
	toolBar->addWidget(toolButton6);
	ElaToolButton* toolButton8 = new ElaToolButton(this);
	toolButton8->setElaIcon(ElaIconType::Aperture);
	toolBar->addWidget(toolButton8);
	ElaToolButton* toolButton9 = new ElaToolButton(this);
	toolButton9->setElaIcon(ElaIconType::ChartMixed);
	toolBar->addWidget(toolButton9);
	ElaToolButton* toolButton10 = new ElaToolButton(this);
	toolButton10->setElaIcon(ElaIconType::Coins);
	toolBar->addWidget(toolButton10);
	ElaToolButton* toolButton11 = new ElaToolButton(this);
	toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolButton11->setElaIcon(ElaIconType::AlarmPlus);
	toolButton11->setText("AlarmPlus");
	toolBar->addWidget(toolButton11);
	ElaToolButton* toolButton12 = new ElaToolButton(this);
	toolButton12->setElaIcon(ElaIconType::Crown);
	toolBar->addWidget(toolButton12);
	QAction* test = new QAction(this);
	test->setMenu(new QMenu(this));

	ElaProgressBar* progressBar = new ElaProgressBar(this);
	progressBar->setMinimum(0);
	progressBar->setMaximum(0);
	progressBar->setFixedWidth(350);
	toolBar->addWidget(progressBar);

	this->addToolBar(Qt::TopToolBarArea, toolBar);

	////停靠窗口
    //ElaDockWidget* logDockWidget = new ElaDockWidget("日志信息", this);
    //logDockWidget->setWidget(new T_LogWidget(this));
    //this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    //resizeDocks({ logDockWidget }, { 200 }, Qt::Horizontal);

	//状态栏
	ElaStatusBar* statusBar = new ElaStatusBar(this);
	ElaText* statusText = new ElaText("初始化成功！", this);
	statusText->setTextPixelSize(14);
	statusBar->addWidget(statusText);
	this->setStatusBar(statusBar);
}
