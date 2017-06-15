#include "common.h"
#include "nclass.h"
#include "rsa.h"
#include "rsa-cash.h"

//Односторонняя немультипликативная функция
N bank::hash(const N& X)
{
    return X;//Пока что здесь стоит заглушка - функция Y=X
}

//Возвращает подпись (F(k)*(r^e mod n))^d mod n
N bank::signBanknote(human *client, N nom, N blinded_hash)
{
    dprint("Банк: Клиент '" + client->name + "' запросил выпуск банкноты с номиналом '"
           + nom.to_str() + "' и затемнёным хэшем '" + blinded_hash.to_str() + "'.\n");
    if( !(blinded_hash > 1) ){
        dprint("Банк: Я могу подписать только хэши больше еденицы. Запрос отклонён.'\n");
        return 0;
    }
    if(!pubMap.contains(nom)){
        dprint("Банк: Такой номинал мною не выпускается. Запрос отклонён.'\n");
        return 0;
    }
    if(storage[client] < nom){
        dprint("Банк: У клиента недостаточно средств на счету для выпуска банкноты такого номинала. Запрос отклонён.'\n");
        return 0;
    }

    N blind_hash_sign = rsa_signify(blinded_hash, privMap[nom]);

    if(emitedList.contains(blind_hash_sign)){
        dprint("Банк: Банкнота с получившейся подписью уже выпускалась. Запрос отклонён.\n");
        return 0;
    }
    //По идее эта проверка не обязательна, но клиент в принципе волен не использовать затемняющий множитель.
//    if(spendedMap.contains(blind_number)){
//        dprint("Банк: Банкнота с таким номером уже тратилась. Запрос отклонён.'\n");
//        return 0;
//    } CHECKME (R = 1)
    this->take(client, nom);
    emitedList.append(blind_hash_sign);
    dprint("Банк: Я согласен выпустить эту банкноту. Моя подпись: '"
           + blind_hash_sign.to_str() + "'. Номинал купюры списан со счёта клиента.\n");
    return blind_hash_sign;
}

bool bank::depositBanknote(human *client, banknote B)
{
    dprint("Банк: Клиент '" + client->name + "' хочет положить на счёт банкноту с номиналом '"
           + B.nom.to_str() + "', серийным номером '" + B.serial.to_str() + "' и подписью '" + B.sign.to_str() + "'.\n");
    if(spendedList.contains(B.serial)){
        dprint("Банк: Эта банкнота была потрачена ранее. Запрос отклонён.\n");
        return false;
    }
    this->put(client, B.nom);
    spendedList.append(B.serial);
    dprint("Банк: Номинал банкноты был добавлен к счёту клиента, а её серийный номер был занесён в список потраченых.\n");
    return true;
}

bool human::emitBanknote(N nom, N serial, N R)
{
    dprint(this->name + ": Хочу выпустить банкноту с номиналом '" + nom.to_str()
           + "' и серийным номером '" + serial.to_str() + "'. Возьму затемняющий множитель R = " + R.to_str() + ".\n");
    if(banking->getCurrencyMap().contains(nom))
    {
        N blinded_hash = rsa_blind(banking->hash(serial), banking->getCurrencyMap()[nom], R);
        dprint(this->name + ": Вычисляю f(serial) * R^e mod n = '" + blinded_hash.to_str() + "' и посылаю это число банку.\n");
        banknote B;
        B.sign = banking->signBanknote(this, nom, blinded_hash);
        if(B.sign != 0){
            B.nom = nom;
            B.serial = serial;
            B.is_spended = 0;
            dprint(this->name + ": Банк прислал подпись '" + B.sign.to_str() +
                   "'. Умножу её на 1/R mod n, чтобы снять затемняющий можножитель.\n");
            B.sign = rsa_unblind(B.sign, banking->getCurrencyMap()[nom], R);
            dprint(this->name + ": У меня получилась подпись '" + B.sign.to_str() +
                   "'. Теперь я могу пользоваться выпущенной банкнотой.\n");
            wallet[B.serial] = B;
            //dprint("AHA: " + wallet[B.nom].nom.to_str() + " " + wallet[B.nom].serial.to_str() + " " + wallet[B.nom].sign.to_str() + "\n" );
            return true;
        }
        else dprint(this->name + ": К сожалению, банк отказался подписывать мою банкноту.\n");
    }
    else dprint(this->name + ": К сожалению, банк не выпускает банкноты с таким номиналом.\n");
    return false;
}

bool human::depositBanknote(N serial)
{
    dprint(this->name + ": Хочу положить на свой счёт банкноту с серийным номером '" + serial.to_str() + "'.\n");
    //Нельзя вызывать эту функцию для несуществующих серийных номеров из-за строчки ниже
    if (banking->depositBanknote(this, wallet[serial]) )
    {
        dprint(this->name + ": Банк принял мою банкноту и добавил её номинал на мой счёт.\n");
        return true;
    }
    else
    {
        dprint(this->name + ": Банк не принял мою банкноту.\n");
        return false;
    }
}


bool bank::addCurrency(N nom, keypair kp, QString keyname)
{
    if(pubMap.contains(nom))
    {
        dprint("Банк: Я не могу добавить номинал '" + nom.to_str() + "' - такой уже существует!\n");
        return false;
    }
    if(nameMap.contains(keyname))
    {
        dprint("Банк: Я могу использовать ключ '" + keyname + "' - он уже занят!\n");
        return false;
    }
    pubMap[nom].n = kp.n;
    pubMap[nom].e = kp.e;

    privMap[nom].n = kp.n;
    privMap[nom].d = kp.d;

    nameMap[keyname] = nom;

    dprint("Банк: Я выпустил новый номинал '" + nom.to_str() + "' по ключу '" + keyname +"'!\n");
    return true;
}

bool bank::removeCurrency(QString keyname)
{
    if(nameMap.contains(keyname))
    {
        N nom = nameMap[keyname];
        nameMap.remove(keyname);
        pubMap.remove(nom);
        privMap.remove(nom);
        return true;
    }
    else return false;
}
bool bank::removeEmitedSignFromList(N sign)
{
    return emitedList.removeOne(sign);
}

bool bank::removeSpendedSerialFromList(N serial)
{
    return spendedList.removeOne(serial);
}







void bank::register_client(human *client)
{
    storage[client] = 0;
}

N bank::balance(const human *client) const
{
    dprint("Банк: у клиента " + client->name + " на счету " + storage[client].to_str() + "$!\n");
    return storage[client];
}
bool bank::put (human *client, const N sum)
{
    storage[client] = storage[client] + sum;
    dprint("Банк: клиент " + client->name + " положил себе на счёт " + sum.to_str() + "$!\n");
    return true;
}
bool bank::take(human *client, const N sum)
{
    storage[client] = storage[client] - sum;
    dprint("Банк: клиент " + client->name + " снял со своего счёта " + sum.to_str() + "$!\n");
    return true;
}



human::human()
{
    name = "unknown";
    banking = NULL;
}

human::human(const QString label, bank& banking_service)
{
    name = label;
    banking = &banking_service;
    //banking->register_client(this); //FIXME
}

void human::sayname()
{
   // dprint("Клиент: мой адрес: " + QString::number((int)this) + "P\n");
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
//void human::showmoney() const
//{
//    dprint(name + ": i have " + money.to_str() + "$!\n");
//}
