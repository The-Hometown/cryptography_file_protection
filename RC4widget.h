#ifndef RC4WIDGET_H
#define RC4WIDGET_H

#include "mainwindow.h"
#include "RC4.h"

using namespace std;

namespace Ui { class RC4widget; }

class RC4widget : public QWidget
{
    Q_OBJECT

public:
    explicit RC4widget(QWidget *parent = 0);
    ~RC4widget();

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

    RC4 rc4;
    Ui::RC4widget *ui;
};

#endif // RC4WIDGET_H
