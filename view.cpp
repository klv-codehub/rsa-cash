#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::refreshAliceWalletPrice()
{
    ui->label_AliceWalletPrice->setText(Alice.getWalletPrice().to_str() + " ₽");
}
void MainWindow::refreshAliceTradeWalletPrice()
{
    ui->label_AliceTradeWalletPrice->setText(Alice.getTradeWalletPrice().to_str() + " ₽");
}
void MainWindow::refreshAliceBagPrice()
{
    ui->label_AliceBagPrice->setText(Alice.getBagPrice().to_str() + " ₽");
}
void MainWindow::refreshBobBagPrice()
{
    ui->label_BobBagPrice->setText(Bob.getBagPrice().to_str() + " ₽");
}
void MainWindow::refreshBobTradeBagPrice()
{
    ui->label_BobTradeBagPrice->setText(Bob.getTradeBagPrice().to_str() + " ₽");
}
void MainWindow::refreshAliceBalance()
{
    ui->label_AliceValue->setText( Alice.balance().to_str() + " ₽" );
}
void MainWindow::refreshBobBalance()
{
    ui->label_BobValue->setText( Bob.balance().to_str() + " ₽" );
}


void MainWindow::refreshAliceWallet()
{
    qListWidget_pos('s', ui->listWidget_AliceWallet);
    ui->listWidget_AliceWallet->clear();
    QList<banknote> banknotesList = Alice.getWallet().values();
    foreach(banknote i, banknotesList)
        ui->listWidget_AliceWallet->addItem(i.nom.to_str() + "₽ SN:" + i.serial.to_str() + " &" + i.sign.to_str());
    qListWidget_pos('l', ui->listWidget_AliceWallet);
    refreshAliceWalletPrice();
}

void MainWindow::refreshAliceTradeWallet()
{
    qListWidget_pos('s', ui->listWidget_AliceTradeWallet);
    ui->listWidget_AliceTradeWallet->clear();
    QList<banknote> TradeBanknotesList = Alice.getTradeWallet().values();
    foreach(banknote i, TradeBanknotesList)
        ui->listWidget_AliceTradeWallet->addItem(i.nom.to_str() + "₽ SN:" + i.serial.to_str() + " &" + i.sign.to_str());
    refreshAliceTradeWalletPrice();
    qListWidget_pos('l', ui->listWidget_AliceTradeWallet);
}

void MainWindow::refreshAliceBag()
{
    qListWidget_pos('s', ui->listWidget_AliceBag);
    ui->listWidget_AliceBag->clear();
    itemMap AliceBag = Alice.getBag();
    QList<item> AliceBagItems = AliceBag.keys();
    foreach(item i, AliceBagItems)
        ui->listWidget_AliceBag->addItem(AliceBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)");
    qListWidget_pos('l', ui->listWidget_AliceBag);
    refreshAliceBagPrice();
}

void MainWindow::refreshBobBag()
{
    qListWidget_pos('s', ui->listWidget_BobBag);
    ui->listWidget_BobBag->clear();
    itemMap BobBag = Bob.getBag();
    QList<item> BobBagItems = BobBag.keys();
    foreach(item i, BobBagItems)
        ui->listWidget_BobBag->addItem(BobBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)");
    qListWidget_pos('l', ui->listWidget_BobBag);
    refreshBobBagPrice();
}

void MainWindow::refreshBobTradeBag()
{
    qListWidget_pos('s', ui->listWidget_BobTradeBag);
    ui->listWidget_BobTradeBag->clear();
    itemMap BobTradeBag = Bob.getTradeBag();
    QList<item> BobTradeBagItems = BobTradeBag.keys();
    foreach(item i, BobTradeBagItems)
        ui->listWidget_BobTradeBag->addItem(BobTradeBag[i].to_str() + "x " + i.name + " (" + i.price.to_str() + " ₽)");
    qListWidget_pos('l', ui->listWidget_BobTradeBag);
    refreshBobTradeBagPrice();
}

void MainWindow::refreshEmited()
{
    qListWidget_pos('s', ui->listWidget_bank_emited);
    ui->listWidget_bank_emited->clear();
    QList<N> emitedList = sber.getEmitedSignsList();
    foreach(N i, emitedList)
        ui->listWidget_bank_emited->addItem(i.to_str());
    qListWidget_pos('l', ui->listWidget_bank_emited);
}

void MainWindow::refreshSpended()
{
    qListWidget_pos('s', ui->listWidget_bank_spended);
    ui->listWidget_bank_spended->clear();
    QList<N> spendedList = sber.getSpendedSerialsList();
    foreach(N i, spendedList)
        ui->listWidget_bank_spended->addItem(i.to_str());
    qListWidget_pos('l', ui->listWidget_bank_spended);
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
    qListWidget_pos('s', ui->listWidget_bank_nom);
    ui->listWidget_bank_nom->clear();
    keyNameMap nameMap = sber.getKeyNameMap();
    QList<QString> nameList = nameMap.keys();
    foreach(QString i, nameList)
        ui->listWidget_bank_nom->addItem(nameMap[i].to_str() + " " + i);
    qListWidget_pos('l', ui->listWidget_bank_nom);
}
