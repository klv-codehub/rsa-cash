#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//---------------------------------------------------------
#include "common.h"
#include "nclass.h"
#include "rsa.h"
#include "rsa-cash.h"
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
    void logWrite(QString text);

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

    void on_comboBox_keypair_choose_currentIndexChanged(int index);

    void on_pushButton_bank_nom_forget_clicked();

    void on_pushButton_AliceDeposit_clicked();

    void on_pushButton_bank_emited_forget_clicked();

    void on_pushButton_spended_forget_clicked();

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

#endif // MAINWINDOW_H
