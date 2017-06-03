//N N::rsa_encrypt(public_key& key)
//{
//    return powmod(key.e, key.n);
//}

//N N::rsa_decrypt(private_key& key)
//{
//    return powmod(key.d, key.n);
//}

//N N::rsa_signify(private_key& key)
//{
//    return powmod(key.d, key.n);
//}

//N N::rsa_verify(public_key& key)
//{
//    return powmod(key.e, key.n);
//}

//N N::rsa_blind(public_key& key, N& r)
//    return (this->N * r.powmod(key.e, key.n)) % key.n;
