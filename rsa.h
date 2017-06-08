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

struct banknote{
    N nominal;			//Номинал банкноты - рубль, сотка, касарь итд
    public_key blinder; //Открытая часть ключа, которым банк будет подписывать серийник банкноты
};

typedef 	list	<banknote> 			banknote_set;

N rsa_encrypt(const N& num, const public_key& key);
N rsa_decrypt(const N& num, const private_key& key)	;
N rsa_signify(const N& num, const private_key& key)	;
N rsa_verify(const N& num, const public_key& key);
N rsa_blind(const N& num, const public_key& key, N& r);

#endif // RSA_Hd
