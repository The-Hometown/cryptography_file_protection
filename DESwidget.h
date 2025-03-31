#ifndef DESWIDGET_H
#define DESWIDGET_H

#include "mainwindow.h"
#include "DES.h"

using namespace std;

namespace Ui { class DESwidget; }

class DESwidget : public QWidget
{
    Q_OBJECT

public:
    explicit DESwidget(QWidget *parent = 0);
    ~DESwidget();

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
    DES des;
    Ui::DESwidget *ui;
};

#endif // DESWIDGET_H
