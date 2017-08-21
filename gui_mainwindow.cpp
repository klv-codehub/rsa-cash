#include "gui_mainwindow.h"
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
//say("{" + new_item.name + "}" + "{" + new_item.price.to_str() + "}\n", "green");//DEBUG
void qListWidget_pos(char action, QListWidget *listWidget)
{
    static int currRow, currItems;
    if(action == 's')
    {
        currRow = listWidget->currentRow();
        currItems = listWidget->count();
    }
    if(action == 'l')
    {
        if(listWidget->count() < currItems && currRow == currItems - 1)
            listWidget->setCurrentRow(currRow - 1);
        else listWidget->setCurrentRow(currRow);
    }
}
void MainWindow::on_pushButton_AliceTradeBanknote_clicked()
{
    if(!ui->listWidget_AliceWallet->selectedItems().isEmpty())
    {
        QString serial = ui->listWidget_AliceWallet->currentItem()->text();
        serial.remove(0, serial.indexOf("SN:")+3);
        serial.remove(serial.indexOf(" &"), serial.length());

        Alice.tradeBanknote(serial);
        refreshAliceWallet();
        refreshAliceTradeWallet();
    }
}

void MainWindow::on_pushButton_AliceUntradeBanknote_clicked()
{
    if(!ui->listWidget_AliceTradeWallet->selectedItems().isEmpty())
    {
        QString serial = ui->listWidget_AliceTradeWallet->currentItem()->text();
        serial.remove(0, serial.indexOf("SN:")+3);
        serial.remove(serial.indexOf(" &"), serial.length());

        Alice.untradeBanknote(serial);
        refreshAliceWallet();
        refreshAliceTradeWallet();
    }
}

void MainWindow::on_pushButton_BobAddItem_clicked()
{
    item new_item;
    new_item.name = ui->lineEdit_item_name->text();
    new_item.price = ui->lineEdit_item_price->text();
    Bob.addItem(new_item);
    refreshBobBag();
    refreshBobBagPrice();
}

void MainWindow::on_pushButton_BobRemoveItem_clicked()
{
    if(!ui->listWidget_BobBag->selectedItems().isEmpty())
    {
        item selected;
        QString itemText = ui->listWidget_BobBag->currentItem()->text();

        selected.name = itemText;
        selected.name.remove(0, selected.name.indexOf('x') + 2);
        selected.name.remove(selected.name.lastIndexOf('(') - 1, selected.name.length());

        itemText.remove(0, itemText.lastIndexOf('(') + 1);
        itemText.remove(itemText.indexOf(' '), itemText.length());
        selected.price = itemText;

        Bob.removeItem(selected);
        refreshBobBag();
        refreshBobBagPrice();
    }
}

void MainWindow::on_pushButton_BobTradeItem_clicked()
{
    if(!ui->listWidget_BobBag->selectedItems().isEmpty())
    {
        item selected;
        QString itemText = ui->listWidget_BobBag->currentItem()->text();

        selected.name = itemText;
        selected.name.remove(0, selected.name.indexOf('x') + 2);
        selected.name.remove(selected.name.lastIndexOf('(') - 1, selected.name.length());

        itemText.remove(0, itemText.lastIndexOf('(') + 1);
        itemText.remove(itemText.indexOf(' '), itemText.length());
        selected.price = itemText;

        Bob.tradeItem(selected);
        refreshBobBag();
        refreshBobTradeBag();
        refreshBobBagPrice();
        refreshBobTradeBagPrice();
    }
}

void MainWindow::on_pushButton_BobUntradeItem_clicked()
{
    if(!ui->listWidget_BobTradeBag->selectedItems().isEmpty())
    {
        item selected;
        QString itemText = ui->listWidget_BobTradeBag->currentItem()->text();

        selected.name = itemText;
        selected.name.remove(0, selected.name.indexOf('x') + 2);
        selected.name.remove(selected.name.lastIndexOf('(') - 1, selected.name.length());

        itemText.remove(0, itemText.lastIndexOf('(') + 1);
        itemText.remove(itemText.indexOf(' '), itemText.length());
        selected.price = itemText;

        Bob.untradeItem(selected);
        refreshBobBag();
        refreshBobTradeBag();
        refreshBobBagPrice();
        refreshBobTradeBagPrice();
    }
}

void MainWindow::on_pushButton_exchange_clicked()
{
    human::make_deal(Alice, Bob, sber);

    refreshBobBalance();
    refreshBobTradeBag();
    refreshBobTradeBagPrice();
    refreshAliceBag();
    refreshAliceBagPrice();
}

void MainWindow::on_pushButton_AliceUseItem_clicked()
{
    if(!ui->listWidget_AliceBag->selectedItems().isEmpty())
    {
        item selected;
        QString itemText = ui->listWidget_AliceBag->currentItem()->text();

        selected.name = itemText;
        selected.name.remove(0, selected.name.indexOf('x') + 2);
        selected.name.remove(selected.name.lastIndexOf('(') - 1, selected.name.length());

        itemText.remove(0, itemText.lastIndexOf('(') + 1);
        itemText.remove(itemText.indexOf(' '), itemText.length());
        selected.price = itemText;

        Alice.removeItem(selected);
        refreshAliceBag();
        refreshAliceBagPrice();
    }
}

void MainWindow::on_pushButton_AliceDepositAllBanknotes_clicked()
{
    foreach(banknote i, Alice.getWallet())
        Alice.depositBanknote(i.serial);
    refreshAliceBalance();
    refreshAliceWallet(); //fixme?
}
