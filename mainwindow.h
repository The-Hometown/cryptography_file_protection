#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AESwidget.h"
#include "DESwidget.h"
#include "TDEAwidget.h"
#include "RSAwidget.h"
#include "RC4widget.h"
#include "ZUCwidget.h"
//#include "SM2widget.h"
#include "SM3widget.h"
#include "SM4widget.h"
#include "hardAESwidget.h"
#include "hardDESwidget.h"
#include "hardTDEAwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void softAESpushButton_click();
    void softDESpushButton_click();
    void soft3DESpushButton_click();
    void softRSApushButton_click();
    void softRC4pushButton_click();
    void softZUCpushButton_click();
    //void softSM2pushButton_click();
    void softSM3pushButton_click();
    void softSM4pushButton_click();
    void hardAESpushButton_click();
    void hardDESpushButton_click();
    void hard3DESpushButton_click();

    void reshow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
