#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//---------------------------------------------------------
#include "integers.h"
#include "rsa.h"
#include "actors.h"
//---------------------------------------------------------
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//---------------------------------------------------------
    void refreshKeys();
    void refreshNominals();
    void refreshEmited();
    void refreshSpended();
    void refreshAliceWallet();
    void refreshAliceBalance();
    void refreshBobBalance();

    void refreshAliceBag();
    void refreshAliceTradeWallet();
    void refreshBobBag();
    void refreshBobTradeBag();

    void refreshAliceWalletPrice();
    void refreshAliceTradeWalletPrice();
    void refreshAliceBagPrice();

    void refreshBobBagPrice();
    void refreshBobTradeBagPrice();
//---------------------------------------------------------

private slots:
    void on_pushButton_clicked();

    void on_pushButton_keygen_clicked();

    void on_pushButton_encrypt_clicked();
    void on_pushButton_decrypt_clicked();
    void on_pushButton_crypt_copy_result_clicked();

    void on_pushButton_signify_clicked();
    void on_pushButton_veryfy_clicked();
    void on_pushButton_sign_copy_result_clicked();

    void on_pushButton_Alice_putmoney_clicked();
    void on_pushButton_Alice_takemoney_clicked();
    void on_pushButton_Bob_putmoney_clicked();
    void on_pushButton_Bob_takemoney_clicked();

    void on_pushButton_bank_addcurrency_clicked();

    void on_pushButton_banknote_sign_clicked();

    void on_comboBox_keypair_choose_currentIndexChanged(int);

    void on_pushButton_bank_nom_forget_clicked();
    void on_pushButton_bank_emited_forget_clicked();
    void on_pushButton_spended_forget_clicked();

    void on_pushButton_AliceTradeBanknote_clicked();
    void on_pushButton_AliceUntradeBanknote_clicked();
    void on_pushButton_AliceRemoveBanknote_clicked();
    void on_pushButton_AliceDepositBanknote_clicked();

    void on_pushButton_BobAddItem_clicked();
    void on_pushButton_BobRemoveItem_clicked();
    void on_pushButton_BobTradeItem_clicked();
    void on_pushButton_BobUntradeItem_clicked();

    void on_pushButton_exchange_clicked();

    void on_pushButton_AliceUseItem_clicked();

    void on_pushButton_AliceDepositAllBanknotes_clicked();

private:
    Ui::MainWindow *ui;
//---------------------------------------------------------
    public_key cryptocalc_public_key;
    private_key cryptocalc_private_key;

    human Alice;
    human Bob;
    bank sber;

    keyMap keyStorage;
//---------------------------------------------------------
};

#include <QListWidget>
void qListWidget_pos(char action, QListWidget *listWidget);

#endif // MAINWINDOW_H
