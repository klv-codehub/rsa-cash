#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    extern QTextEdit *DebugPrintQTE;
    DebugPrintQTE = ui->textEdit;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    logWrite("PushButton clicked!\n");
    N a("7893465879586353567895865376879475865376465234321567786475378958342635798346546789857465758358346587958635356789586537687947586537646523432156778647533");
    N b("789346587958635356789586537687947586537646337895834263579834654678985746575835837893465879586353567895865376879475865376463378958342635798346546789857465758358346587958635583465879586353567895865376879475865376465234321567786446523432156778647533");
    N c("546796543");
    N res = a.powmod(b, c);
    logWrite(QString::fromStdString(a.to_str() + "^" + b.to_str() + " mod " + c.to_str() + " = \n" + res.to_str() + "\n"));
    //N a("12");
    //N b("0");

    //logWrite(QString::fromStdString(c.to_str() + "!!!\n"));
}

void MainWindow::logWrite(QString text)
{
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(text);
    ui->textEdit->moveCursor(QTextCursor::End);
}
