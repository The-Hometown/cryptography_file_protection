#ifndef TDEAWIDGET_H
#define TDEAWIDGET_H

#include "mainwindow.h"
#include "TDEA.h"

using namespace std;

namespace Ui { class TDEAwidget; }

class TDEAwidget : public QWidget
{
    Q_OBJECT

public:
    explicit TDEAwidget(QWidget *parent = 0);
    ~TDEAwidget();

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
    TDEA tdea;
    Ui::TDEAwidget *ui;
};

#endif // TDEAWIDGET_H
