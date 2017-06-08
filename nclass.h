#ifndef NCLASS_H
#define NCLASS_H

class N
{
    private:
        byte_vector digit;
    public:
        N(){};
        N(const char* str);
        N(int a);
        string to_str();
        string to_binstr();
        byte_vector to_bytevector();

        N powmod(N pow, N mod);
        N revmod(N mod);

        void mul10k(int k);
        //void divN(N b);
        bool chk0();
        void mulK(int k);
        //void mulN(N b);

        friend bool operator > (N& a, N& b);
        friend bool operator < (N& a, N& b);
        friend bool operator == (N& a, N& b);
        friend bool operator != (N& a, N& b);


        friend N operator + (N& a, N& b);
        friend N operator - (N& a, N& b);
        friend N operator * (N& a, N& b);
        friend N operator / (N& a, N& b);
        friend N operator % (N& a, N& b);     
};

class Z
{
    private:
        N module;
        bool sign;
    public:
        Z(){};
        Z(const char* str);
        Z(N& a);
        string to_str();

        N abs();

        friend bool operator > (Z& a, Z& b);
        friend bool operator < (Z& a, Z& b);
        friend bool operator == (Z& a, Z& b);

        friend Z operator + (Z& a, Z& b);
        friend Z operator - (Z& a, Z& b);
        friend Z operator * (Z& a, Z& b);
};

#endif // NCLASS_H
