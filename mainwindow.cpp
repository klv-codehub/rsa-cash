#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showMaximized();

    extern QTextEdit *DebugPrintQTE;
    DebugPrintQTE = ui->textEdit;
    extern QTextEdit *ProtocolPrintQTE;
    ProtocolPrintQTE = ui->textEdit_protocol;

    Alice = {"Alice", "green", sber};
    Bob = {"Bob", "blue", sber};
    sber.register_client(&Alice);   //Из-за странного бага с this в конструкторе
    sber.register_client(&Bob);     //Регистрацию в банке проводим здесь
    refreshAliceBalance();
    refreshBobBalance();

    keypair t;
    t.n = 143;
    t.e = 7;
    t.d = 103;
    keyStorage["key_1"] = t;
    t.n = 123;
    t.e = 432;
    t.d = 3487;
    keyStorage["key_2"] = t;
    t.n = 23;
    t.e = 43122;
    t.d = 3484567;
    keyStorage["key_3"] = t;

    refreshKeys();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshAliceWallet()
{
    ui->listWidget_AliceWallet->clear();
    QList<banknote> banknotesList = Alice.getWallet().values();
    foreach(banknote i, banknotesList)
        ui->listWidget_AliceWallet->addItem(i.nom.to_str() + "₽ №" + i.serial.to_str() + " &" + i.sign.to_str() + "\n");
}

void MainWindow::refreshAliceTradeWallet()
{
    ui->listWidget_AliceTradeWallet->clear();
    QList<banknote> TradeBanknotesList = Alice.getTradeWallet().values();
    foreach(banknote i, TradeBanknotesList)
        ui->listWidget_AliceTradeWallet->addItem(i.nom.to_str() + "₽ №" + i.serial.to_str() + " &" + i.sign.to_str() + "\n");
}

void MainWindow::refreshAliceBag()
{
    ui->listWidget_AliceBag->clear();
    itemMap AliceBag = Alice.getBag();
    QList<item> AliceBagItems = AliceBag.keys();
    foreach(item i, AliceBagItems)
        ui->listWidget_AliceBag->addItem(AliceBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)\n");
}

void MainWindow::refreshBobBag()
{
    ui->listWidget_BobBag->clear();
    itemMap BobBag = Bob.getBag();
    QList<item> BobBagItems = BobBag.keys();
    foreach(item i, BobBagItems)
        ui->listWidget_BobBag->addItem(BobBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)\n");
}

void MainWindow::refreshBobTradeBag()
{
    ui->listWidget_BobTradeBag->clear();
    itemMap BobTradeBag = Bob.getTradeBag();
    QList<item> BobTradeBagItems = BobTradeBag.keys();
    foreach(item i, BobTradeBagItems)
        ui->listWidget_BobTradeBag->addItem(BobTradeBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)\n");
}

void MainWindow::refreshAliceBalance()
{
    ui->label_AliceValue->setText( Alice.balance().to_str() + " ₽" );
}

void MainWindow::refreshBobBalance()
{
    ui->label_BobValue->setText( Bob.balance().to_str() + " ₽" );
}



void MainWindow::on_pushButton_banknote_sign_clicked()
{
    Alice.emitBanknote(ui->lineEdit_banknote_nom->text(),
                       ui->lineEdit_banknote_serial->text(),
                       ui->lineEdit_banknote_R->text());
    refreshAliceWallet();
    refreshAliceBalance();
    refreshEmited();
}



void MainWindow::on_pushButton_AliceDeposit_clicked()
{
    if(!ui->listWidget_AliceWallet->selectedItems().isEmpty())
    {
        N serial = ui->listWidget_AliceWallet->currentItem()->text();
        dprint("LOL: " + serial.to_str() + "\n");
        //currencyKeyName.remove(0, currencyKeyName.indexOf(' ') + 1);
        //N nom = sber.getKeyNameMap()[currencyKeyName];

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

void MainWindow::refreshEmited()
{
    ui->listWidget_bank_emited->clear();
    QList<N> emitedList = sber.getEmitedSignsList();
    foreach(N i, emitedList)
        ui->listWidget_bank_emited->addItem(i.to_str());
}

void MainWindow::refreshSpended()
{
    ui->listWidget_bank_spended->clear();
    QList<N> spendedList = sber.getSpendedSerialsList();
    foreach(N i, spendedList)
        ui->listWidget_bank_spended->addItem(i.to_str());
}

void MainWindow::refreshKeys()
{
    ui->comboBox_keypair_choose->clear();
    QList<QString> keyList = keyStorage.keys();
    foreach(QString i, keyList)
        ui->comboBox_keypair_choose->addItem(i);
}

void MainWindow::refreshNominals()
{
    ui->listWidget_bank_nom->clear();
    keyNameMap nameMap = sber.getKeyNameMap();
    QList<QString> nameList = nameMap.keys();
    foreach(QString i, nameList)
        ui->listWidget_bank_nom->addItem(nameMap[i].to_str() + " " + i);
}

void MainWindow::on_pushButton_clicked()
{
    DebugPrintQTE->clear();
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

void MainWindow::on_comboBox_keypair_choose_currentIndexChanged(int index)
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
