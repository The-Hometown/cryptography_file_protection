#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->softAESpushButton, SIGNAL(clicked()), this, SLOT(softAESpushButton_click()));
    connect(ui->softDESpushButton, SIGNAL(clicked()), this, SLOT(softDESpushButton_click()));
    connect(ui->soft3DESpushButton, SIGNAL(clicked()), this, SLOT(soft3DESpushButton_click()));
    connect(ui->softRSApushButton, SIGNAL(clicked()), this, SLOT(softRSApushButton_click()));
    connect(ui->softRC4pushButton, SIGNAL(clicked()), this, SLOT(softRC4pushButton_click()));
    connect(ui->softZUCpushButton, SIGNAL(clicked()), this, SLOT(softZUCpushButton_click()));
    //connect(ui->softSM2pushButton, SIGNAL(clicked()), this, SLOT(softSM2pushButton_click()));
    connect(ui->softSM3pushButton, SIGNAL(clicked()), this, SLOT(softSM3pushButton_click()));
    connect(ui->softSM4pushButton, SIGNAL(clicked()), this, SLOT(softSM4pushButton_click()));
    connect(ui->hardAESpushButton, SIGNAL(clicked()), this, SLOT(hardAESpushButton_click()));
    connect(ui->hardDESpushButton, SIGNAL(clicked()), this, SLOT(hardDESpushButton_click()));
    connect(ui->hard3DESpushButton, SIGNAL(clicked()), this, SLOT(hard3DESpushButton_click()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::softAESpushButton_click()
{
    this->hide();
    AESwidget *aeswidget = new AESwidget();
    connect(aeswidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    aeswidget->show();
}

void MainWindow::softDESpushButton_click()
{
    this->hide();
    DESwidget *deswidget = new DESwidget();
    connect(deswidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    deswidget->show();
}

void MainWindow::soft3DESpushButton_click()
{
    this->hide();
    TDEAwidget *tdeawidget = new TDEAwidget();
    connect(tdeawidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    tdeawidget->show();
}

void MainWindow::softRSApushButton_click()
{
    this->hide();
    RSAwidget *rsawidget = new RSAwidget();
    connect(rsawidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    rsawidget->show();
}

void MainWindow::softRC4pushButton_click()
{
    this->hide();
    RC4widget *rc4widget=new RC4widget();
    connect(rc4widget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    rc4widget->show();
}

void MainWindow::softZUCpushButton_click()
{
    this->hide();
    ZUCwidget *zucwidget=new ZUCwidget();
    connect(zucwidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    zucwidget->show();
}
/*
void MainWindow::softSM2pushButton_click()
{
    this->hide();
    SM2widget *sm2widget=new SM2widget();
    connect(sm2widget,SIGNAL(sendsignal()),this,SLOT(reshow()));
    sm2widget->show();

}*/

void MainWindow::softSM3pushButton_click()
{
    this->hide();
    SM3widget *sm3widget=new SM3widget();
    connect(sm3widget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    sm3widget->show();
}

void MainWindow::softSM4pushButton_click()
{
    this->hide();
    SM4widget *sm4widget=new SM4widget();
    connect(sm4widget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    sm4widget->show();
}

void MainWindow::hardAESpushButton_click()
{
    this->hide();
    hardAESwidget *hardaeswidget=new hardAESwidget();
    connect(hardaeswidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    hardaeswidget->show();
}

void MainWindow::hardDESpushButton_click()
{
    this->hide();
    hardDESwidget *harddeswidget=new hardDESwidget();
    connect(harddeswidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    harddeswidget->show();
}

void MainWindow::hard3DESpushButton_click()
{
    this->hide();
    hardTDEAwidget *hardtdeawidget=new hardTDEAwidget();
    connect(hardtdeawidget, SIGNAL(sendsignal()), this, SLOT(reshow()));
    hardtdeawidget->show();
}

void MainWindow::reshow()
{
    this->show();
}
