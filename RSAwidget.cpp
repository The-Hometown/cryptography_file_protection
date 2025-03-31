#include "RSAwidget.h"
#include "ui_RSAwidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

RSAwidget::RSAwidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::RSAwidget)

{
    ui->setupUi(this);

    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")   // 解决中文乱码问题
    #endif

    ui->progressBar->setRange(0, 100); // 设置进度条的范围从0到100
    ui->progressBar->setValue(0);   // 初始化进度条的值为0

    //设置信号
    connect(ui->genpushButton, SIGNAL(clicked()), this, SLOT(genpushButton_click()));
    connect(ui->filepushButton, SIGNAL(clicked()), this, SLOT(filepushButton_click()));
    connect(ui->pubkeypushButton, SIGNAL(clicked()), this, SLOT(pubkeypushButton_click()));
    connect(ui->prikeypushButton, SIGNAL(clicked()), this, SLOT(prikeypushButton_click()));
    connect(ui->encryptpushButton, SIGNAL(clicked()), this, SLOT(encryptpushButton_click()));
    connect(ui->decryptpushButton, SIGNAL(clicked()), this, SLOT(decryptpushButton_click()));
    connect(ui->backpushButton, SIGNAL(clicked()), this, SLOT(backpushButton_click()));
}

RSAwidget::~RSAwidget()
{
    delete ui;
}

void RSAwidget::on_genpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString publickeyfileName = folderPath + "/public_key.txt";
        QString privatekeyfileName = folderPath + "/private_key.txt";
        QFile file(publickeyfileName);
        QFile file1(privatekeyfileName);
        std::string publickeyfilePathStr = publickeyfileName.toStdString();
        std::string privatekeyfilePathStr = privatekeyfileName.toStdString();
        rsa.genKeys(publickeyfilePathStr, privatekeyfilePathStr);

        // 更新进度条的值
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "密钥生成成功！");
        ui->textBrowser->append("生成的公钥存储在: " + publickeyfileName);
        ui->textBrowser->append("生成的私钥存储在: " + privatekeyfileName);
    }

}

void RSAwidget::on_filepushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        infile_path = filePath.toStdString();
        ui->filelineEdit->setText(filePath);
        ui->textBrowser->append("选择文件: " + filePath);
    }
}

void RSAwidget::on_pubkeypushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        publickey_path = filePath.toStdString();
        ui->pubkeylineEdit->setText((filePath));
        rsa.getPublicKey(publickey_path);
        ui->textBrowser->append(("公钥文件: " + filePath));
    }
}

void RSAwidget::on_prikeypushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        privatekey_path = filePath.toStdString();
        ui->keylineEdit->setText(filePath);
        rsa.getPrivateKey(privatekey_path);
        ui->textBrowser->append("私钥文件: " + filePath);
    }
}

void RSAwidget::on_encryptpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/encrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        rsa.encrypt_file(infile_path, outfile_path);
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件加密成功！");
        ui->textBrowser->append("加密后的文件存放路径: " + fileName);
    }
}

void RSAwidget::on_decryptpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/decrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        rsa.decrypt_file(infile_path, outfile_path);
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件解密成功！");
        ui->textBrowser->append("解密后的文件存放路径: " + fileName);
    }
}

void RSAwidget::on_backpushButton_clicked()
{
    emit sendsignal();
    this->close();
}

