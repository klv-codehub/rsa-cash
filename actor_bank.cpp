#include "print.h"
#include "integers.h"
#include "rsa.h"
#include "actors.h"

publicCurrencyMap   bank::getCurrencyMap()          {return pubMap;}
privateCurrencyMap  bank::getCurrencyPrivateMap()   {return privMap;}
keyNameMap          bank::getKeyNameMap()           {return nameMap;}
emitedSignsList     bank::getEmitedSignsList()      {return emitedList;}
spendedSerialsList  bank::getSpendedSerialsList()   {return spendedList;}

//Односторонняя немультипликативная функция
#include <QCryptographicHash>
N bank::hash(const N& X)
{   
    return X;//Пока что здесь стоит заглушка - функция Y=X
//    return N::from_bytearray(
//                QCryptographicHash::hash(
//                    X.to_bytearray(),
//                    QCryptographicHash::Md5));
}

//Возвращает подпись (F(k)*(r^e mod n))^d mod n
N bank::signBanknote(human *client, N nom, N blinded_hash)
{
    say("Банк: Клиент '" + client->name + "' запросил выпуск банкноты с номиналом "
           + nom.to_str() + "₽ и затемнёным хэшем '" + blinded_hash.to_str() + "'.\n", "orange");
    if( !(blinded_hash > 1) ){
        say("Банк: Я могу подписать только хэши больше еденицы. Запрос отклонён.'\n", "orange");
        return 0;
    }
    if(!pubMap.contains(nom)){
        say("Банк: Такой номинал мною не выпускается. Запрос отклонён.'\n", "orange");
        return 0;
    }
    if(storage[client] < nom){
        say("Банк: У клиента недостаточно средств на счету для выпуска банкноты такого номинала. Запрос отклонён.'\n", "orange");
        return 0;
    }

    N blind_hash_sign = rsa_signify(blinded_hash, privMap[nom]);

    if(emitedList.contains(blind_hash_sign)){
        say("Банк: Банкнота с получившейся подписью уже выпускалась. Запрос отклонён.\n", "orange");
        return 0;
    }
    //По идее эта проверка не обязательна, но клиент в принципе волен не использовать затемняющий множитель.
//    if(spendedMap.contains(blind_number)){
//        say("Банк: Банкнота с таким номером уже тратилась. Запрос отклонён.'\n");
//        return 0;
//    } CHECKME (R = 1)
    this->take(client, nom);
    emitedList.append(blind_hash_sign);
    say("Банк: Я согласен выпустить эту банкноту. Моя подпись: '"
           + blind_hash_sign.to_str() + "'. Номинал купюры списан со счёта клиента.\n", "orange");
    return blind_hash_sign;
}

bool bank::depositBanknote(human *client, banknote B)
{
    say("Банк: Клиент '" + client->name + "' хочет положить на счёт банкноту с номиналом '"
           + B.nom.to_str() + "', серийным номером '" + B.serial.to_str() + "' и подписью '" + B.sign.to_str() + "'.\n", "orange");
    if(spendedList.contains(B.serial)){
        say("Банк: Эта банкнота была потрачена ранее. Запрос отклонён.\n", "orange");
        return false;
    }
    if( !pubMap.contains(B.nom) ){
        say("Банк: Запрошенный номинал отсутствует в реестре. Запрос отклонён.\n", "orange");
        return false;
    }

    if( B.serial == rsa_verify(B.sign, pubMap[B.nom]) )
    {
    this->put(client, B.nom);
    spendedList.append(B.serial);
    say("Банк: Моя подпись валидна. Банкноту ещё не тратили. Её номинал был добавлен к счёту клиента,"
           "а серийный номер был занесён в список потраченых.\n", "orange");
    return true;
    }
    else
    {
        say("Банк: Подпись на банкноте неверна. Запрос отклонён.\n", "orange");
        return false;
    }

}

bool bank::addCurrency(N nom, keypair kp, QString keyname)
{
    if(pubMap.contains(nom))
    {
        say("Банк: Я не могу добавить номинал '" + nom.to_str() + "' - такой уже существует!\n", "orange");
        return false;
    }
    if(nameMap.contains(keyname))
    {
        say("Банк: Я могу использовать ключ '" + keyname + "' - он уже занят!\n", "orange");
        return false;
    }
    pubMap[nom].n = kp.n;
    pubMap[nom].e = kp.e;

    privMap[nom].n = kp.n;
    privMap[nom].d = kp.d;

    nameMap[keyname] = nom;

    say("Банк: Я выпустил новый номинал '" + nom.to_str() + "' по ключу '" + keyname +"'!\n", "orange");
    return true;
}

bool bank::removeCurrency(QString keyname)
{
    if(nameMap.contains(keyname))
    {
        N nom = nameMap[keyname];   
        say("Банк: Я больше не принимаю номинал '" + nom.to_str() + "' по ключу '" + keyname + "'.\n", "orange");

        nameMap.remove(keyname);
        pubMap.remove(nom);
        privMap.remove(nom);
        return true;
    }
    else return false;
}
bool bank::removeEmitedSignFromList(N sign)
{
    if(emitedList.removeOne(sign)) // fixme? why removeOne?
    {
        say("Банк: Подпись '" + sign.to_str() + "' удалена из реестра.\n", "orange");
        return true;
    }
    else return false;
}

bool bank::removeSpendedSerialFromList(N serial)
{
    if(spendedList.removeOne(serial))
    {
            say("Банк: Серийный номер '" + serial.to_str() + "' удалён из реестра.\n", "orange");
            return true;
    }
    else return false;
}

void bank::register_client(human *client)
{
    storage[client] = 0;
}

N bank::balance(const human *client) const
{
    return storage[client];
}
bool bank::put (human *client, const N sum)
{
    if(sum > 0)
    {
        storage[client] = storage[client] + sum;
        say("Банк: клиент " + client->name + " положил себе на счёт " + sum.to_str() + " ₽!\n", "orange");
    }
    return true;
}
bool bank::take(human *client, const N sum)
{
    if(sum > 0)
    {
        if(storage[client] < sum)
        {
            say("Банк: У клиента " + client->name + " на счету недостаточно денег, чтобы снять " + sum.to_str() + " ₽.\n", "orange");
            return false;
        }
        storage[client] = storage[client] - sum;
        say("Банк: клиент " + client->name + " снял со своего счёта " + sum.to_str() + " ₽!\n", "orange");
    }
    return true;
}


