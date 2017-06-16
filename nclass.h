#ifndef NCLASS_H
#define NCLASS_H

class N
{
    private:
        byte_vector digit;
    public:
        N();
        N(int);
        N(const QString);

        QString to_str() const;
        QString to_binstr() const;
        byte_vector to_bytevector();

        N powmod(const N pow, const N mod) const;
        N revmod(const N mod) const;

        void mul10k(int k);
        void mulK(int k);

        friend bool operator > (const N& a, const N& b);
        friend bool operator < (const N& a, const N& b);
        friend bool operator == (const N& a, const N& b);
        friend bool operator != (const N& a, const N& b);

        friend N operator + (const N& a, const N& b);
        friend N operator - (const N& a, const N& b);
        friend N operator * (const N& a, const N& b);
        friend N operator / (const N& a, const N& b);
        friend N operator % (const N& a, const N& b);
};

class Z
{
    private:
        N module;
        bool sign;
    public:
        Z(){};
        Z(int);
        Z(const N&);
        Z(const QString);
        QString to_str() const;

        N abs();

        friend bool operator > (const Z& a, const Z& b);
        friend bool operator < (const Z& a, const Z& b);
        friend bool operator == (const Z& a, const Z& b);

        friend Z operator + (const Z& a, const Z& b);
        friend Z operator - (const Z& a, const Z& b);
        friend Z operator * (const Z& a, const Z& b);
};

#endif // NCLASS_H
