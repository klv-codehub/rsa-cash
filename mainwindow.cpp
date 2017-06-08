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
    //N a("7893465879586353567895865376879475865376465234321567786475378958342635798346546789857465758358346587958635356789586537687947586537646523432156778647533");
    //N b("789346587958635356789586537687947586537646337895834263579834654678985746575835837893465879586353567895865376879475865376463378958342635798346546789857465758358346587958635583465879586353567895865376879475865376465234321567786446523432156778647533");
    //N c("546796543");
    //N res = a.powmod(b, c);
    //logWrite(QString::fromStdString(a.to_str() + "^" + b.to_str() + " mod " + c.to_str() + " = \n" + res.to_str() + "\n"));
    //N a("200");
    //N b("45");
    //N c = a - b;
    N a("2813");
    N b("8789");


    N c = a.revmod(b);
    logWrite(QString::fromStdString(a.to_str() + "-" + b.to_str() + "=" + c.to_str() + "\n"));
//    Z d = c + b;
//    logWrite(QString::fromStdString(c.to_str() + "+" + b.to_str() + "=" + d.to_str() + "\n"));
}

void MainWindow::logWrite(QString text)
{
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(text);
    ui->textEdit->moveCursor(QTextCursor::End);
}

void MainWindow::on_revmod_pushButton_clicked()
{
    N e(ui->e_lineEdit->text().toStdString().c_str());
    N fiN(ui->fiN_lineEdit->text().toStdString().c_str());
    N mod = e.revmod(fiN);
    ui->revmod_label->setText(QString::fromStdString(mod.to_str()));
}
