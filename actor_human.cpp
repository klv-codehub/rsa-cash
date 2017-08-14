#include "print.h"
#include "integers.h"
#include "rsa.h"
#include "actors.h"

banknotesMap   human::getWallet()        {return wallet;}
banknotesMap   human::getTradeWallet()   {return trade_wallet;}
itemMap        human::getBag()           {return bag;}
itemMap        human::getTradeBag()      {return trade_bag;}

bool operator< (const item& A, const item& B)
{
    if(A.name < B.name) return true;
    else if(A.name == B.name)
        if(A.price < B.price) return true;
    return false;
   // return (A.name != B.name  A.price != B.price) ? true:false;//WTF is here
}

N   human::getWalletPrice()
{
    N price = 0;
    foreach(banknote i, wallet)
        price = price + i.nom;
    return price;
}

N   human::getTradeWalletPrice()
{
    N price = 0;
    foreach(banknote i, trade_wallet)
        price = price + i.nom;
    return price;
}

N   human::getBagPrice()
{
    N price = 0;
    foreach(item i, bag.keys())
        price = price + i.price * bag[i];
    return price;
}

N   human::getTradeBagPrice()
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

bool human::emitBanknote(N nom, N serial, N R)
{
    say(this->name + ": Хочу выпустить банкноту с номиналом '" + nom.to_str()
           + "' и серийным номером '" + serial.to_str() + "'. Возьму затемняющий множитель R = " + R.to_str() + ".\n", this->color);
    if( !banking->getCurrencyMap().contains(nom) )
    {
        say(this->name + ": К сожалению, банк не выпускает банкноты с таким номиналом.\n", this->color);
            return false;
    }
    if( this->wallet.contains(serial))
    {
        say(this->name + ": Но у меня уже есть банкнота с таким номером. Я могу запутаться.\n", this->color);
            return false;
    }
    if( serial > banking->getCurrencyMap()[nom].n)
    {
        say(this->name + ": Но серийный номер банкноты должен быть меньше модуля открытого ключа банка.\n", this->color);
            return false;
    }

        N blinded_hash = rsa_blind(banking->hash(serial), banking->getCurrencyMap()[nom], R);
        say(this->name + ": Вычисляю f(serial) * R^e mod n = '" + blinded_hash.to_str() + "' и посылаю это число банку.\n", this->color);
        banknote B;
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
            wallet[B.serial] = B;
            return true;
        }
        else say(this->name + ": К сожалению, банк отказался подписывать мою банкноту.\n", this->color);
    return false;
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
