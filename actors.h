#ifndef BANK_H
#define BANK_H

#include "integers.h"
#include <QMap>

struct keypair{ //Пара ключей
    N n;	// Модуль
    N e;	// Открытая экспонента
    N d;	// Закрытая экспонента
};

struct banknote{   //Электронная банкнота
    N serial;      //Номер
    N nom;         //Номинал
    N sign;        //Подпись банка
};

struct item{        //Вещь
    QString name;   //Название
    N price;        //Цена
};

//Оператор сравнения для однозначной сортировки предметов в ассоциативном массиве QMap
bool operator< (const item& A, const item& B);

//Хранилище ключей программы
typedef QMap <QString, keypair> keyMap; //Имя ключевой пары и она сама

//Банк
typedef QMap <QString, N> keyNameMap;   //Имя ключевой пары и её номинал
typedef QMap <N, public_key> publicCurrencyMap; //Номинал и открытый ключ
typedef QMap <N, private_key> privateCurrencyMap;//Номинал и закрытый ключ
typedef QList <N> emitedSignsList;      //Поставленные подписи
typedef QList <N> spendedSerialsList;   //Потраченные серийники

//Клиенты
typedef QMap <N, banknote> banknotesMap;      //Серийник - ключ
typedef QMap <item, N> itemMap;     //Предмет и количество

class human;

class bank
{
private:
    QMap <const human*, N> storage;
    publicCurrencyMap pubMap;
    privateCurrencyMap privMap;
    keyNameMap nameMap;
    emitedSignsList emitedList;
    spendedSerialsList spendedList;
public:
    bank(){};
    void register_client(human *client);
    N balance(const human *client) const;

    N hash(const N& X);

    bool put (human *client, const N sum);
    bool take(human *client, const N sum);

    bool addCurrency(N nom, keypair kp, QString keyname);
    bool removeCurrency(QString keyname);
    bool removeEmitedSignFromList(N sign);
    bool removeSpendedSerialFromList(N serial);

    N signBanknote(human *client, N nom, N blinded_hash);
    bool depositBanknote(human *client, banknote B);

    publicCurrencyMap   getCurrencyMap()          ;// {return pubMap;}
    privateCurrencyMap  getCurrencyPrivateMap()   ;// {return privMap;}
    keyNameMap          getKeyNameMap()           ;// {return nameMap;}
    emitedSignsList     getEmitedSignsList()      ;// {return emitedList;}
    spendedSerialsList  getSpendedSerialsList()   ;// {return spendedList;}
};

class human
{
    private:
        bank *banking;
        banknotesMap wallet;
        banknotesMap trade_wallet;
        itemMap     bag;
        itemMap     trade_bag;
    public:
        QString name;
        QString color;
        void sayname();
        human();
        human(const QString label, const QString _color, bank &banking_service);

        N balance() const;
        bool putmoney(const N sum);
        bool takemoney(const N sum);

        bool emitBanknote(N nom, N serial, N R);
        bool depositBanknote(N serial);
        bool removeBanknote(N serial);

        void tradeBanknote(N serial);
        void untradeBanknote(N serial);

        void addItem(item new_item);
        void removeItem(item old_item);
        void tradeItem(item new_item);
        void untradeItem(item old_item);

        void makeoffer(human *seller);
        void takeoffer(human  *buyer, banknotesMap trade_wallet);

        banknotesMap   getWallet()        ;// {return wallet;}
        banknotesMap   getTradeWallet()   ;// {return trade_wallet;}
        itemMap        getBag()           ;// {return bag;}
        itemMap        getTradeBag()      ;// {return trade_bag;}

        N   getWalletPrice();
        N   getTradeWalletPrice();
        N   getBagPrice();
        N   getTradeBagPrice();
};

#endif // BANK_H
