#ifndef NCLASS_H
#define NCLASS_H

class N
{
    private:
        byte_vector digit;
    public:
        N();
        N(const char* str);
        string to_str();
        string to_binstr();
        byte_vector to_bytevector();

        void powmod(N pow, N mod);

        void mul10k(int k);
        void divN(N b);
        bool chk0();
        void mulK(int k);
        void mulN(N b);

        friend N operator + (N& a, N& b);

        friend bool operator < (N& a, N& b);
        friend bool operator > (N& a, N& b);
        friend bool operator == (N& a, N& b);

        friend N operator - (N& a, N& b);
        friend N operator / (N& a, N& b);
        friend N operator % (N& a, N& b);
};

#endif // NCLASS_H
