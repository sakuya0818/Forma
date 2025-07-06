#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"

#include <QMainWindow>

class ElaContentDialog;

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void initWindow();
	void initEdgeLayout();

private:

    ElaContentDialog* _closeDialog{ nullptr };
};
#endif // MAINWINDOW_H
