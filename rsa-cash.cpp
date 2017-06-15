#include "common.h"
#include "nclass.h"
#include "rsa.h"
#include "rsa-cash.h"

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
    dprint("Клиент: мой адрес: " + QString::number((int)this) + "P\n");
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
