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
    ui->textEdit->clear();
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
    N e (ui->lineEdit_e->text().toStdString().c_str());

    dprint(p.to_str() + '\n');
    dprint(q.to_str() + '\n');
    dprint(e.to_str() + '\n');
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
        cryptocalc_public_key.n = n;
        cryptocalc_public_key.e = e;
        cryptocalc_private_key.n = n;
        cryptocalc_private_key.d = d;
    } else
    {
        ui->lineEdit_d->setText("Обратного числа к выбранному e по модулю fi(N) не существует!");
        ui->tabWidget->setDisabled(true);
    }
}



void MainWindow::on_pushButton_encrypt_clicked()
{
    N data (ui->lineEdit_encrypt_data->text().toStdString().c_str());
    N result = rsa_encrypt(data, cryptocalc_public_key);
    ui->lineEdit_encrypt_result->setText(QString::fromStdString(result.to_str()));
}

void MainWindow::on_pushButton_decrypt_clicked()
{
    N data (ui->lineEdit_decrypt_data->text().toStdString().c_str());
    N result = rsa_decrypt(data, cryptocalc_private_key);
    ui->lineEdit_decrypt_result->setText(QString::fromStdString(result.to_str()));
}


void MainWindow::on_pushButton_crypt_copy_result_clicked()
{
    ui->lineEdit_decrypt_data->setText(ui->lineEdit_encrypt_result->text());
}

void MainWindow::on_pushButton_signify_clicked()
{
    N data (ui->lineEdit_signify_data->text().toStdString().c_str());
    N result = rsa_signify(data, cryptocalc_private_key);
    ui->lineEdit_signify_result->setText(QString::fromStdString(result.to_str()));
}

void MainWindow::on_pushButton_veryfy_clicked()
{
    N data (ui->lineEdit_verify_data->text().toStdString().c_str());
    N result = rsa_verify(data, cryptocalc_public_key);
    ui->lineEdit_verify_result->setText(QString::fromStdString(result.to_str()));
}

void MainWindow::on_pushButton_sign_copy_result_clicked()
{
    ui->lineEdit_verify_data->setText(ui->lineEdit_signify_result->text());
}
