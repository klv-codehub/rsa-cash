#include "gui_mainwindow.h"
#include "ui_mainwindow.h"

#include "print.h"

void MainWindow::on_pushButton_banknote_sign_clicked()
{
    Alice.takeBanknoteToWallet(
    Alice.emitBanknote(ui->lineEdit_banknote_nom->text(),
                       ui->lineEdit_banknote_serial->text(),
                       ui->lineEdit_banknote_R->text()));
    refreshAliceWallet();
    refreshAliceBalance();
    refreshEmited();
}

void MainWindow::on_pushButton_AliceRemoveBanknote_clicked()
{
    if(!ui->listWidget_AliceWallet->selectedItems().isEmpty())
    {
        QString serial = ui->listWidget_AliceWallet->currentItem()->text();
        serial.remove(0, serial.indexOf("SN:")+3);
        serial.remove(serial.indexOf(" &"), serial.length());

        Alice.removeBanknote(serial);

        refreshAliceWallet();
        refreshAliceBalance(); //fixme?
        refreshSpended(); //fixme?
    }
}

void MainWindow::on_pushButton_AliceDepositBanknote_clicked()
{
    if(!ui->listWidget_AliceWallet->selectedItems().isEmpty())
    {
        QString serial = ui->listWidget_AliceWallet->currentItem()->text();
        serial.remove(0, serial.indexOf("SN:")+3);
        serial.remove(serial.indexOf(" &"), serial.length());//Второй параметр сделан "ошибочным" для упрощения кода

        //serial.remove(serial.indexOf('&') - 1, serial.length() ); REMEMBER ME
        //serial.remove(0, serial.indexOf("N")+1);//QT multichar bug with '№'

        say("{" + serial + "}\n", "blue");
        Alice.depositBanknote(serial);

        refreshAliceWallet();
        refreshAliceBalance();
        refreshSpended();
    }
}

void MainWindow::on_pushButton_bank_nom_forget_clicked()
{
    if(!ui->listWidget_bank_nom->selectedItems().isEmpty())
    {
        QString currencyKeyName = ui->listWidget_bank_nom->currentItem()->text();
        currencyKeyName.remove(0, currencyKeyName.indexOf(' ') + 1);
        N nom = sber.getKeyNameMap()[currencyKeyName];

        sber.removeCurrency(currencyKeyName);
        refreshNominals();
    }
}

void MainWindow::on_pushButton_Alice_putmoney_clicked()
{
    Alice.putmoney(ui->lineEdit_Alice_money->text());
    refreshAliceBalance();
}

void MainWindow::on_pushButton_Alice_takemoney_clicked()
{
    Alice.takemoney(ui->lineEdit_Alice_money->text());
    refreshAliceBalance();
}

void MainWindow::on_pushButton_Bob_putmoney_clicked()
{
    Bob.putmoney(ui->lineEdit_Bob_money->text());
    refreshBobBalance();
}

void MainWindow::on_pushButton_Bob_takemoney_clicked()
{
    Bob.takemoney(ui->lineEdit_Bob_money->text());
    refreshBobBalance();
}

void MainWindow::on_pushButton_bank_addcurrency_clicked()
{
    sber.addCurrency(ui->lineEdit_bank_nom->text(),
                     keyStorage[ui->comboBox_keypair_choose->currentText()],
                        ui->comboBox_keypair_choose->currentText());
    refreshNominals();
}

void MainWindow::on_comboBox_keypair_choose_currentIndexChanged(int)
{
    QString currentKeyName = ui->comboBox_keypair_choose->currentText();
    ui->lineEdit_bank_N->setText(keyStorage[currentKeyName].n.to_str());
    ui->lineEdit_bank_d->setText(keyStorage[currentKeyName].d.to_str());
    ui->lineEdit_bank_e->setText(keyStorage[currentKeyName].e.to_str());
}

void MainWindow::on_pushButton_bank_emited_forget_clicked()
{
    if(!ui->listWidget_bank_emited->selectedItems().isEmpty())
    {
        N sign = ui->listWidget_bank_emited->currentItem()->text();
        sber.removeEmitedSignFromList(sign);
        refreshEmited();
    }
}

void MainWindow::on_pushButton_spended_forget_clicked()
{
    if(!ui->listWidget_bank_spended->selectedItems().isEmpty())
    {
        N serial = ui->listWidget_bank_spended->currentItem()->text();
        sber.removeSpendedSerialFromList(serial);
        refreshSpended();
    }
}

//КРИПТОКАЛЬКУЛЯТОР
void MainWindow::on_pushButton_clicked()
{
    DebugPrintQTE->clear();
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
