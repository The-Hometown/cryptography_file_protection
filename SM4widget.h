#ifndef SM4WIDGET_H
#define SM4WIDGET_H

#include "mainwindow.h"
#include "SM4.h"

using namespace std;

namespace Ui { class SM4widget; }

class SM4widget : public QWidget
{
    Q_OBJECT

public:
    explicit SM4widget(QWidget *parent = 0);
    ~SM4widget();

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

    SM4 sm4;
    Ui::SM4widget *ui;
};

#endif // SM4WIDGET_H
