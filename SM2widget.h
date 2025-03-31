#ifndef SM2WIDGET_H
#define SM2WIDGET_H

#include "mainwindow.h"
#include "SM2.h"

using namespace std;

namespace Ui { class SM2widget; }

class SM2widget : public QWidget
{
    Q_OBJECT

public:
    explicit SM2widget(QWidget *parent = 0);
    ~SM2widget();

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

    SM2 sm2;
    Ui::SM2widget *ui;
};

#endif // SM2WIDGET_H
