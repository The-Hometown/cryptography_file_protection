#ifndef ZUCWIDGET_H
#define ZUCWIDGET_H

#include "mainwindow.h"
#include "ZUC.h"

using namespace std;

namespace Ui { class ZUCwidget; }

class ZUCwidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZUCwidget(QWidget *parent = 0);
    ~ZUCwidget();

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

    ZUC zuc;
    Ui::ZUCwidget *ui;
};

#endif // ZUCWIDGET_H
