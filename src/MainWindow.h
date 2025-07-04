#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"

#include <QMainWindow>

class ElaContentDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void initWindow();
	void initEdgeLayout();

private:
    Ui::MainWindow *ui;

    ElaContentDialog* _closeDialog{ nullptr };
};
#endif // MAINWINDOW_H
