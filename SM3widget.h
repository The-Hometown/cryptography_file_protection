#ifndef SM3WIDGET_H
#define SM3WIDGET_H

#include "mainwindow.h"
#include "SM3.h"

using namespace std;

namespace Ui { class SM3widget; }

class SM3widget : public QWidget
{
    Q_OBJECT

public:
    explicit SM3widget(QWidget *parent = 0);
    ~SM3widget();

private slots:

    void on_filepushButton_clicked();

    void on_hashpushButton_clicked();

    void on_backpushButton_clicked();

signals:
    void sendsignal();

private:
    string infile_path;

    SM3 sm3;
    Ui::SM3widget *ui;
};

#endif // SM3WIDGET_H
