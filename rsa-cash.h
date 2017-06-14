#ifndef RSACASH_H
#define RSACASH_H

class human;

class bank
{
private:
    QMap <const human*, N> storage;
public:
    bank(){};
    void register_client(human *client);
    bool put (human *client, const N sum);
    bool take(human *client, const N sum);
    N balance(const human *client) const;
};

class human
{
    private:
        friend bank;
        QString name;
        bank *banking;
    public:
        human();
        human(const QString label, bank &banking_service);
        N balance();
        bool putmoney(const N sum);
        bool takemoney(const N sum);
};


#endif // RSACASH_H
//requestList buildRequestList(N sum, nominalList nom)
//rawBlindRequestList buildRawBlindRequestList(requestList A,
