#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//---------------------------------------------------------
#include "common.h"
#include "nclass.h"
//---------------------------------------------------------
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//---------------------------------------------------------
    void logWrite(QString text);
//---------------------------------------------------------

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
//---------------------------------------------------------

//---------------------------------------------------------
};

#endif // MAINWINDOW_H
