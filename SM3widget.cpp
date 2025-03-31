#include "SM3widget.h"
#include "ui_SM3widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

SM3widget::SM3widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SM3widget)

{
    ui->setupUi(this);

    #if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")   // 解决中文乱码问题
    #endif

    ui->progressBar->setRange(0, 100); // 设置进度条的范围从0到100
    ui->progressBar->setValue(0);   // 初始化进度条的值为0

    //设置信号
    connect(ui->filepushButton, SIGNAL(clicked()), this, SLOT(filepushButton_click()));
    connect(ui->hashpushButton, SIGNAL(clicked()), this, SLOT(hashpushButton_click()));
    connect(ui->backpushButton, SIGNAL(clicked()), this, SLOT(backpushButton_click()));
}

SM3widget::~SM3widget()
{
    delete ui;
}

void SM3widget::on_filepushButton_clicked()
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

void SM3widget::on_hashpushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", QDir::homePath());
    if (!folderPath.isEmpty())
    {
        QString fileName = folderPath + "/hash_value.txt";
        QFile file(fileName);
        std::string outfile_path = fileName.toStdString();
        sm3.hashCalculateFile(infile_path, outfile_path);
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Success", "文件hash值计算成功！");
        ui->textBrowser->append("hash值文件存放路径: " + fileName);
    }
}

void SM3widget::on_backpushButton_clicked()
{
    emit sendsignal();
    this->close();
}
