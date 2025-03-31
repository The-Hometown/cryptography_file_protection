#ifndef AESWIDGET_H
#define AESWIDGET_H

#include "mainwindow.h"
#include "AES.h"

using namespace std;

namespace Ui { class AESwidget; }

class AESwidget : public QWidget
{
    Q_OBJECT

public:
    explicit AESwidget(QWidget *parent = 0);
    ~AESwidget();

private slots:

    void on_genpushButton_clicked();

    void on_filepushButton_clicked();

    void on_keypushButton_clicked();

    void on_encryptpushButton_clicked();

    void on_decryptpushButton_clicked();

    void on_backpushButton_clicked();

signals:
    void sendsignal();

private:
    string infile_path;
    string key_path;

    AES aes;
    Ui::AESwidget *ui;
};

#endif // AESWIDGET_H
