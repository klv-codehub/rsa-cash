#ifndef RSACASH_H
#define RSACASH_H

struct keypair{
    N n;	// Модуль
    N e;	// Открытая экспонента
    N d;	// Закрытая экспонента
};

typedef QMap <N, public_key> publicCurrencyMap;
typedef QMap <N, private_key> privateCurrencyMap;
typedef QMap <N, N> emitedCurrencyMap;  //Номер и номинал
typedef QMap <N, N> spendedCurrencyMap; //Номер и номинал
typedef QMap <QString, keypair> keyMap; //Имя ключевой пары и она сама
typedef QMap <QString, N> keyNameMap;   //Номинал и имя ключа

class human;

class bank
{
private:
    QMap <const human*, N> storage;
    publicCurrencyMap pubMap;
    privateCurrencyMap privMap;
    keyNameMap nameMap;
    emitedCurrencyMap emitedMap;
    spendedCurrencyMap spendedMap;
public:
    bank(){};
    void register_client(human *client);
    N balance(const human *client) const;
    bool put (human *client, const N sum);
    bool take(human *client, const N sum);

    bool addCurrency(N nom, keypair kp, QString keyname);
    bool removeCurrency(QString keyname);
    publicCurrencyMap getCurrencyMap()          {return pubMap;}
    privateCurrencyMap getCurrencyPrivateMap()   {return privMap;}
    keyNameMap getKeyNameMap()                  {return nameMap;}
    emitedCurrencyMap getEmitedNumbersMap()     {return emitedMap;}
    spendedCurrencyMap getSpendedNumbersMap()   {return spendedMap;}
};

class human
{
    private:
        friend bank;
        QString name;
        bank *banking;
    public:
        void sayname();
        human();
        human(const QString label, bank &banking_service);
        N balance() const;
        bool putmoney(const N sum);
        bool takemoney(const N sum);
};


#endif // RSACASH_H
//requestList buildRequestList(N sum, nominalList nom)
//rawBlindRequestList buildRawBlindRequestList(requestList A,
