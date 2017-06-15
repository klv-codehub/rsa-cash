#ifndef RSA_H
#define RSA_H

struct public_key{
    N n;	// Модуль
    N e;	// Открытая экспонента
};

struct private_key{
    N n;	// Модуль
    N d;	// Закрытая экспонента
};

N rsa_encrypt(const N& num, const public_key& key);
N rsa_decrypt(const N& num, const private_key& key)	;
N rsa_signify(const N& num, const private_key& key)	;
N rsa_verify(const N& num, const public_key& key);
N rsa_blind(const N& num, const public_key& key, N& r);
N rsa_unblind(const N& num, const public_key& key, N& r);
#endif // RSA_H
