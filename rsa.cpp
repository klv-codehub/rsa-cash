#include "common.h"
#include "nclass.h"
#include "rsa.h"

N rsa_encrypt(const N& num, const public_key& key)
{
    return num.powmod(key.e, key.n);
}

N rsa_decrypt(const N& num, const private_key& key)
{
    return num.powmod(key.d, key.n);
}

N rsa_signify(const N& num, const private_key& key)
{
    return num.powmod(key.d, key.n);
}

N rsa_verify(const N& num, const public_key& key)
{
    return num.powmod(key.e, key.n);
}

N rsa_blind(const N& num, const public_key& key, N& r)
{
    return num * r.powmod(key.e, key.n) % key.n;
}

N rsa_unblind(const N& num, N& r)
{
    return num / r;
}
