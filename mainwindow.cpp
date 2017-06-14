#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    extern QTextEdit *DebugPrintQTE;
    DebugPrintQTE = ui->textEdit;

    Alice = {"Alice", sber};
    Bob = {"Bob", sber};
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //human John("John Doe");
//    John.showmoney();
//    John.spendmoney(500);
//    John.showmoney();
//    John.earnmoney(300);
//    John.showmoney();
}

void MainWindow::logWrite(QString text)
{
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(text);
    ui->textEdit->moveCursor(QTextCursor::End);
}


void MainWindow::on_pushButton_keygen_clicked()
{
    N p = ui->lineEdit_p->text();
    N q = ui->lineEdit_q->text();
    N e = ui->lineEdit_e->text();

    N n = p*q;
    N fiN = (p-1)*(q-1);
    N d = e.revmod(fiN);

    ui->lineEdit_n->setText(n.to_str());
    ui->lineEdit_d->setText(d.to_str());
    if(d != 0)
    {
        ui->tabWidget->setEnabled(true);
        cryptocalc_public_key.n = n;
        cryptocalc_public_key.e = e;
        cryptocalc_private_key.n = n;
        cryptocalc_private_key.d = d;
    }
    else
    {
        ui->lineEdit_d->setText("Обратного числа к выбранному e по модулю fi(N) не существует!");
        ui->tabWidget->setDisabled(true);
    }
}

void MainWindow::on_pushButton_encrypt_clicked()
{
    N data = ui->lineEdit_encrypt_data->text();
    N result = rsa_encrypt(data, cryptocalc_public_key);
    ui->lineEdit_encrypt_result->setText(result.to_str());
}

void MainWindow::on_pushButton_decrypt_clicked()
{
    N data = ui->lineEdit_decrypt_data->text();
    N result = rsa_decrypt(data, cryptocalc_private_key);
    ui->lineEdit_decrypt_result->setText(result.to_str());
}

void MainWindow::on_pushButton_crypt_copy_result_clicked()
{
    ui->lineEdit_decrypt_data->setText(ui->lineEdit_encrypt_result->text());
}

void MainWindow::on_pushButton_signify_clicked()
{
    N data = ui->lineEdit_signify_data->text();
    N result = rsa_signify(data, cryptocalc_private_key);
    ui->lineEdit_signify_result->setText(result.to_str());
}

void MainWindow::on_pushButton_veryfy_clicked()
{
    N data = ui->lineEdit_verify_data->text();
    N result = rsa_verify(data, cryptocalc_public_key);
    ui->lineEdit_verify_result->setText(result.to_str());
}

void MainWindow::on_pushButton_sign_copy_result_clicked()
{
    ui->lineEdit_verify_data->setText(ui->lineEdit_signify_result->text());
}

void MainWindow::on_pushButton_Alice_putmoney_clicked()
{
    Alice.putmoney(ui->lineEdit_Alice_money->text());
    ui->label_AliceValue->setText( Alice.balance().to_str() );
}

void MainWindow::on_pushButton_Alice_takemoney_clicked()
{
    Alice.takemoney(ui->lineEdit_Alice_money->text());
    ui->label_AliceValue->setText( Alice.balance().to_str() );
}

void MainWindow::on_pushButton_Bob_putmoney_clicked()
{
    Bob.putmoney(ui->lineEdit_Bob_money->text());
    ui->label_BobValue->setText( Bob.balance().to_str() );
}

void MainWindow::on_pushButton_Bob_takemoney_clicked()
{
    Bob.takemoney(ui->lineEdit_Bob_money->text());
    ui->label_BobValue->setText( Bob.balance().to_str() );
}
