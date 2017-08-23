#include "print.h"
#include "integers.h"
#include "rsa.h"
#include "actors.h"

banknotesMap   human::getWallet()        {return wallet;}
banknotesMap   human::getTradeWallet()   {return trade_wallet;}
itemMap        human::getBag()           {return bag;}
itemMap        human::getTradeBag()      {return trade_bag;}

void human::make_deal(human& buyer, human& seller, bank& banking)
{
    if( !banking.getCurrencyMap().contains(1) )
    {
        say("Продавец: Я не хочу торговать, пока банк не выпустит номинал в 1₽.\n", seller.color);
        say("Продавец: Вдруг я не смогу отдать сдачу?\n", seller.color);
        return;
    }

    N money_incomed, money_cashed, money_failed;

    banknotesMap buyerWallet = buyer.getTradeWallet();

    foreach (banknote i, buyerWallet) {
        money_incomed = money_incomed + i.nom;
        if(banking.depositBanknote(&seller, i))
            money_cashed = money_cashed + i.nom;
    }

    money_failed = money_incomed - money_cashed;

    N money_price = seller.getTradeBagPrice();


    say("Продавец: Принятая сумма: " + money_incomed.to_str() + "₽\n", seller.color);
    say("Продавец: Не успешно: " + money_failed.to_str() + "₽\n", seller.color);
    say("Продавец: Успешно: " + money_cashed.to_str() + "₽\n", seller.color);
    say("Продавец: Цена товаров: " + money_price.to_str() + "₽\n", seller.color);

    N money_change;
    if(money_incomed < money_price)
    {
        money_change = money_incomed;
        say("Продавец: Мне дали недостаточно денег. Я отказываюсь от сделки и возвращаю полученные деньги!\n", seller.color);

    }
    else
    {
        money_change = money_cashed - money_price;
        say("Продавец: Я согласен на сделку. ", seller.color);
        if(money_change > 0)
            say("Сдача: " + money_change.to_str() + "₽\n", seller.color);
        else
            say("Сдача не нужна.\n", seller.color);
        seller.sendTradeBagTo(buyer);
    }

    //Согласно документации Qt, гарантированно отсортирован по возрастанию
    QList <N> nomlist = banking.getCurrencyMap().keys();
    QList<N>::iterator i = nomlist.end();
    QVector<N> change_noms;

    while(money_change > 0)
    {

        i--;
        while( money_change > *i || money_change == *i)
        {
            change_noms.insert(0, *i);
            money_change = money_change - *i;
        }

    }

    foreach (N nom, change_noms) {
        seller.takeBanknoteToTradeWallet(seller.emitRandomBanknote(nom));
    }

    seller.sendTradeWalletTo(buyer);
    seller.trade_wallet.clear();
}

void human::sendTradeBagTo(human& reciever)
{
    QList<item> tradeBagItems = trade_bag.keys();
        foreach(item itm, tradeBagItems)
        {
            while(trade_bag.contains(itm))
            {
                reciever.addItem(itm);
                untradeItem(itm);
                removeItem(itm);
            }
        }
}

void human::takeBanknoteToWallet(const banknote& b)
{
    if(b.serial != 0)
    if(wallet.contains(b.serial))
        say("Беспредел! Мне суют банкноту с SN:" + b.serial.to_str() + ", которая у меня уже есть!\n", color);
    else
    {
       // b.color = ...
       wallet[b.serial] = b;
    }
}

void human::takeBanknoteToTradeWallet(const banknote& b)
{
    if(trade_wallet.contains(b.serial))
        say("Беспредел! Мне суют банкноту с SN:" + b.serial.to_str() + ", которая у меня уже есть!\n", color);
    else
    {
       // b.color = ...
        trade_wallet[b.serial] = b;
    }
}

void human::sendTradeWalletTo(human& reciever)
{
    foreach(banknote b, trade_wallet)
    {
        //b.color = ...
        reciever.takeBanknoteToTradeWallet(b);
    }
}

bool operator< (const item& A, const item& B)
{
    if(A.name < B.name) return true;
    else if(A.name == B.name)
        if(A.price < B.price) return true;
    return false;
   // return (A.name != B.name  A.price != B.price) ? true:false;//WTF is here
}

N   human::getWalletPrice() const
{
    N price = 0;
    foreach(banknote i, wallet)
        price = price + i.nom;
    return price;
}

N   human::getTradeWalletPrice() const
{
    N price = 0;
    foreach(banknote i, trade_wallet)
        price = price + i.nom;
    return price;
}

N   human::getBagPrice() const
{
    N price = 0;
    foreach(item i, bag.keys())
        price = price + i.price * bag[i];
    return price;
}

N   human::getTradeBagPrice() const
{
    N price = 0;
    foreach(item i, trade_bag.keys())
        price = price + i.price * trade_bag[i];
    return price;
}

void human::tradeBanknote(N serial)
{
    if(wallet.contains(serial))
    {
        if(trade_wallet.contains(serial))
            say(name + ": Эта банкнота уже выбрана для оплаты. Я не могу использовать две одинаковых банкноты в одном платеже.\n", color);
        else
            trade_wallet[serial] = wallet[serial];
    } //Это сообщение не должно вылезать при  нормальной работе программы
    else say(name + ": [ERROR] Я не могу платить не существующей банкнотой.\n", "red");
}

void human::untradeBanknote(N serial)
{
    if(trade_wallet.contains(serial))
    {
        if( !wallet.contains(serial) )                  //Если мы удалили банкноту из кошелька
            wallet[serial] = trade_wallet[serial];      //Скопируем её обратно
        trade_wallet.remove(serial);                    //Уберём банкноту из списка обмена
    //Это сообщение не должно вылезать при  нормальной работе программы
    }
    else say(name + ": [ERROR] Я не могу убрать несуществующую банкноту.\n", "red");
}

void human::addItem(item new_item)
{
    if( new_item.name.isEmpty() )
    {
            say(name + ": Мне нужно дать название предмету, чтобы отличать его от других в сумке.\n", color);
            return;
    }
    if(bag.contains(new_item))
        bag[new_item] = bag[new_item] + 1;
    else
        bag[new_item] = 1;
    say(name + ": Я кладу к себе в сумку предмет '" + new_item.name + "', который стоит " + new_item.price.to_str() + "₽\n", color);
}

void human::removeItem(item old_item)
{
    if(bag.contains(old_item))
    {
        if (bag[old_item] == 1)
            bag.remove(old_item);
        else bag[old_item] = bag[old_item] - 1;
        say(name + ": Я выкидываю из сумки предмет '" + old_item.name + "', который стоит " + old_item.price.to_str() + "₽\n", color);
    }
    //Это сообщение не должно вылезать при  нормальной работе программы
    else say(name + ": [ERROR] Я не могу выбросить не существующий предмет.\n", "red");
}

void human::tradeItem(item new_item)
{   //Если такой предмет есть в сумке
    if(bag.contains(new_item))
    {
        //Убираем предмет из сумки
        if (bag[new_item] == 1)
            bag.remove(new_item);
        else bag[new_item] = bag[new_item] - 1;
        //Кладём предмет на прилавок
        if(trade_bag.contains(new_item))
            trade_bag[new_item] = trade_bag[new_item] + 1;
        else
            trade_bag[new_item] = 1;
        say(name + ": Я хочу продать предмет '" + new_item.name + "', который стоит " + new_item.price.to_str() + "₽\n", color);
    } //Это сообщение не должно вылезать при  нормальной работе программы
    else say(name + ": [ERROR] Я не могу продавать не существующий предмет.\n", "red");
}

void human::untradeItem(item old_item)
{   //Если такой предмет есть на прилавке
    if(trade_bag.contains(old_item))
    {
        //Убираем предмет с прилавка
        if (trade_bag[old_item] == 1)
            trade_bag.remove(old_item);
        else trade_bag[old_item] = trade_bag[old_item] - 1;
        //Кладём предмет в сумку
        if(bag.contains(old_item))
            bag[old_item] = bag[old_item] + 1;
        else
            bag[old_item] = 1;
        say(name + ": Я больше не хочу продавать предмет '" + old_item.name + "', который стоит " + old_item.price.to_str() + "₽\n", color);
    } //Это сообщение не должно вылезать при  нормальной работе программы
    else say(name + ": [ERROR] Я не могу перестать продавать не продающийся предмет.\n", "red");
}

human::human()
{
    name = "unknown";
    banking = NULL;
}

human::human(const QString label, const QString _color, bank& banking_service)
{
    name = label;
    color = _color;
    banking = &banking_service;
    //banking->register_client(this); //FIXME
}

void human::sayname()
{
   // say("Клиент: мой адрес: " + QString::number((int)this) + "P\n");
}


N human::balance() const
{
    return banking->balance(this);
}
bool human::putmoney(const N sum)
{
    banking->put(this, sum);
    return true;
}

bool human::takemoney(const N sum)
{
    banking->take(this, sum);
    return true;
}

banknote human::emitBanknote(N nom, N serial, N R)
{
    banknote B;
    B.serial = 0;
    say(this->name + ": Хочу выпустить банкноту с номиналом '" + nom.to_str()
           + "' и серийным номером '" + serial.to_str() + "'. Возьму затемняющий множитель R = " + R.to_str() + ".\n", this->color);
    if( !banking->getCurrencyMap().contains(nom) )
    {
        say(this->name + ": К сожалению, банк не выпускает банкноты с таким номиналом.\n", this->color);
            return B;
    }
    if( this->wallet.contains(serial))
    {
        say(this->name + ": Но у меня уже есть банкнота с таким номером. Я могу запутаться.\n", this->color);
            return B;
    }
    if( serial > banking->getCurrencyMap()[nom].n)
    {
        say(this->name + ": Но серийный номер банкноты должен быть меньше модуля открытого ключа банка.\n", this->color);
            return B;
    }
        N blinded_hash = rsa_blind(banking->hash(serial), banking->getCurrencyMap()[nom], R);
        say(this->name + ": Вычисляю f(serial) * R^e mod n = '" + blinded_hash.to_str() + "' и посылаю это число банку.\n", this->color);

        B.sign = banking->signBanknote(this, nom, blinded_hash);
        if(B.sign != 0){
            B.nom = nom;
            B.serial = serial;
            say(this->name + ": Банк прислал подпись '" + B.sign.to_str() +
                   "'. Умножу её на 1/R mod n, чтобы снять затемняющий можножитель.\n", this->color);
            B.sign = rsa_unblind(B.sign, banking->getCurrencyMap()[nom], R);
            if(B.sign != 0)
            {
                say(this->name + ": У меня получилась подпись '" + B.sign.to_str() +
                       "'. Теперь я могу пользоваться выпущенной банкнотой.\n", this->color);
            }
            else
            {
                say(name + ": Опа! Обратного затемняющему множителю R числа по модулю ключей подписи не существует.\n", color);
                say(name + ": Деньги потеряны, банкнота вышла с нулевой подписью, но зато теперь я знаю R - число, кратное делителю модуля ключевой пары!\n", color);
                say(name + ": Посчитаю-ка я НОД R и N...\n", color);
            }//Из-за этого нюанса надо добавить Бобу проверку на нулевую подпись банкноты...
        }
        else say(this->name + ": К сожалению, банк отказался подписывать мою банкноту.\n", this->color);
    return B;
}

banknote human::emitRandomBanknote(N nom)
{
    N nom_public_key_module = banking->getCurrencyMap()[nom].n;
    N serial = N::getRandomNumberLessThan(nom_public_key_module);
    N R = N::getRandomNumberLessThan(nom_public_key_module);
    return emitBanknote(nom, serial, R);
}

bool human::depositBanknote(N serial)
{
    say(this->name + ": Хочу положить на свой счёт банкноту с серийным номером '" + serial.to_str() + "'.\n", this->color);

    if(wallet.contains(serial))
        if (banking->depositBanknote(this, wallet[serial]) )
        {
            say(this->name + ": Банк принял мою банкноту и добавил её номинал на мой счёт.\n", this->color);
            return true;
        }
        else
        {
            say(this->name + ": Банк не принял мою банкноту.\n", this->color);
            return false;
        }
    //Это сообщение не должно вылезать при  нормальной работе программы
    else
    {
        say(this->name + ": [ERROR] Но у меня в кошельке нету банкноты с таким номером!\n", "red");
        return false;
    }
}

bool human::removeBanknote(N serial)
{
    say(this->name + ": Стираю из кошелька банкноту с серийным номером '" + serial.to_str() + "'.\n", this->color);

    if(wallet.contains(serial))
    {
        wallet.remove(serial);
        return true;
    }
    //Это сообщение не должно вылезать при  нормальной работе программы
    else
    {
        say(this->name + ": [ERROR] Но у меня в кошельке нету банкноты с таким номером!\n", "red");
        return false;
    }
}
