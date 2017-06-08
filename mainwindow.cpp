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


void MainWindow::on_pushButton_keygen_clicked()
{
    N NOne("1");
    N NZero("0");

    N p (ui->lineEdit_p->text().toStdString().c_str());
    N q (ui->lineEdit_q->text().toStdString().c_str());
    N e (ui->lineEdit_q->text().toStdString().c_str());

    N n = p*q;
    N PmunusOne = p-NOne;  //FIXME
    N QmunusOne = q-NOne;  //FIXME
    N fiN = PmunusOne*QmunusOne;

    N d = e.revmod(fiN);

    ui->lineEdit_n->setText(QString::fromStdString(n.to_str()));
    ui->lineEdit_d->setText(QString::fromStdString(d.to_str()));
    if(d != NZero)
    {
        ui->tabWidget->setEnabled(true);
    } else
    {
        ui->lineEdit_d->setText("Обратного числа к выбранному e по модулю fi(N) не существует!");
        ui->tabWidget->setDisabled(true);
    }

}
