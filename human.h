#ifndef HUMAN_H
#define HUMAN_H


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

        banknotesMap   getWallet()        {return wallet;}
        banknotesMap   getTradeWallet()   {return trade_wallet;}
        itemMap        getBag()           {return bag;}
        itemMap        getTradeBag()      {return trade_bag;}

        N   getWalletPrice();
        N   getTradeWalletPrice();
        N   getBagPrice();
        N   getTradeBagPrice();
};

#endif // HUMAN_H
