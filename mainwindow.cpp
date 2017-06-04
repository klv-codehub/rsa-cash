#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    logWrite("PushButton clicked!\n");
    N a("1234567534214675432634211456578913249123449320741943085467236478456324146767342673657854321");
    N b("7895834263579834654678985746575835834658795863535678958653768794758653764652343215677864753");
    N c("546796543");
    N res = a.powmod(b, c);
    logWrite(QString::fromStdString(a.to_str() + "^" + b.to_str() + " mod " + c.to_str() + " = \n" + res.to_str() + "\n"));
}

void MainWindow::logWrite(QString text)
{
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(text);
    ui->textEdit->moveCursor(QTextCursor::End);
}
