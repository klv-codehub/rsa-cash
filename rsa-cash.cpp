#include "common.h"
#include "nclass.h"
#include "rsa.h"
#include "rsa-cash.h"

human::human()
{
    name = "unknown";
    banking = NULL;
}

human::human(const QString label, bank& banking_service)
{
    name = label;
    banking = &banking_service;
    banking->register_client(this);
}

void bank::register_client(human *client)
{
    storage[client] = 0;
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

N bank::balance(const human *client) const
{
    dprint("Банк: у клиента " + client->name + " на счету " + storage[client].to_str() + "$!\n");
    return storage[client];
}




N human::balance()
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


//void client::sendmoney(client& A);
//void client::recvmoney(client& A);

