#include "RC4widget.h"
#include "ui_RC4widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

RC4widget::RC4widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::RC4widget)

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
    connect(ui->keypushButton, SIGNAL(clicked()), this, SLOT(keypushButton_click()));
    connect(ui->encryptpushButton, SIGNAL(clicked()), this, SLOT(encryptpushButton_click()));
    connect(ui->decryptpushButton, SIGNAL(clicked()), this, SLOT(decryptpushButton_click()));
    connect(ui->backpushButton, SIGNAL(clicked()), this, SLOT(backpushButton_click()));
}

RC4widget::~RC4widget()
{
    delete ui;
}

void RC4widget::on_genpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/key.txt";
        QFile file(fileName);
        std::string filePathStr = fileName.toStdString();
        rc4.genKey(filePathStr);

        // 更新进度条的值
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "密钥生成成功！");
        ui->textBrowser->append("生成的密钥存储在: " + fileName);
    }

}

void RC4widget::on_filepushButton_clicked()
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

void RC4widget::on_keypushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "Text files (*.txt)");
    if (!filePath.isEmpty())
    {
        ui->progressBar->setValue(0);
        key_path = filePath.toStdString();
        ui->keylineEdit->setText(filePath);
        rc4.getKey(key_path);
        ui->textBrowser->append("密钥文件: " + filePath);
    }
}

void RC4widget::on_encryptpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/encrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        rc4.enc_dec_file(infile_path, outfile_path, 0);
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件加密成功！");
        ui->textBrowser->append("加密后的文件存放路径: " + fileName);
    }
}

void RC4widget::on_decryptpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/decrypted_file.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        rc4.enc_dec_file(infile_path, outfile_path, 1);
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件解密成功！");
        ui->textBrowser->append("解密后的文件存放路径: " + fileName);
    }
}

void RC4widget::on_backpushButton_clicked()
{
    emit sendsignal();
    this->close();
}
