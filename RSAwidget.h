#ifndef RSAWIDGET_H
#define RSAWIDGET_H

#include "mainwindow.h"
#include "RSA.h"

using namespace std;

namespace Ui { class RSAwidget; }

class RSAwidget : public QWidget
{
    Q_OBJECT

public:
    explicit RSAwidget(QWidget *parent = 0);
    ~RSAwidget();

private slots:

    void on_genpushButton_clicked();

    void on_filepushButton_clicked();

    void on_pubkeypushButton_clicked();

    void on_prikeypushButton_clicked();

    void on_encryptpushButton_clicked();

    void on_decryptpushButton_clicked();

    void on_backpushButton_clicked();

signals:
    void sendsignal();

private:
    string infile_path;
    string publickey_path;
    string privatekey_path;

    RSA rsa;
    Ui::RSAwidget *ui;
};

#endif // RSAWIDGET_H

