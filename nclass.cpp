#include "common.h"
#include "nclass.h"
#include <iostream>
#include <algorithm>

N::N()
{
    //digit.push_back(0);
}

N::N(const char* str)
{
    for(int i = strlen(str) - 1; i >=0 ; i--)
        if('0' <= str[i] && str[i] <= '9')
            digit.push_back(str[i] - '0');
}

string N::to_str()
{
    string t;
    for(int i = digit.size()-1; i >=0; i--)
        t+= digit[i] + '0';
    return t;
}
//=======================================================================================
N operator + (N& a, N& b)
{
   //N c("123");
    //printf("\nnew!! a size = %d, b size = %d", a.digit.size(), b.digit.size());

    N bigger = (a.digit.size() < b.digit.size())?b:a;
    N smaller = (a.digit.size() < b.digit.size())?a:b;

   int sum = 0;
   N result;

   //printf(" \nbigger size = %d, smaller size = %d\n", bigger.digit.size(), smaller.digit.size());

   for (int i = 0; i < bigger.digit.size(); i++) {
       if (i < smaller.digit.size()) {
           sum += bigger.digit[i] + smaller.digit[i];
       } else {
           sum += bigger.digit[i];
       }
       if (sum <= 9) {
           result.digit.push_back(sum);
           sum = 0;
       } else {
           result.digit.push_back(sum - 10);
           sum = 1;
           if (i == bigger.digit.size() - 1) {
               result.digit.push_back(sum);
           }
       }
   }
   return result;
}

bool operator == (N& a, N& b) {
    if (a.digit.size() != b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] != b.digit[i]) return false;
    }
    return true;
}

bool operator > (N& a, N& b) {
    if (a.digit.size() > b.digit.size()) return true;
    if (a.digit.size() < b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] < b.digit[i]) return false;
        else if (a.digit[i] > b.digit[i]) return true;
    }
    return false;
}

bool operator < (N& a, N& b) {
    if (a.digit.size() < b.digit.size()) return true;
    if (a.digit.size() > b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        //printf("a [i] = %d\nb[i] = %d\n",a.digit[i],b.digit[i]);
        if (a.digit[i] > b.digit[i]) return false;
        else if (a.digit[i] < b.digit[i]) return true;
    }
    return false;
}

N operator - (N& a, N& b) {
    N result;
    //по умолчанию а > b, иначе возвращает пустое N
    if (a < b) return a;
    int dif = 0;

    int i;
    for (i = 0; i < b.digit.size(); i++) {
        a.digit[i] += dif;
        if (a.digit[i] < b.digit[i] || a.digit[i] == 255) {
            a.digit[i] += 10;
            dif = -1;
        } else {
            dif = 0;
        }
        result.digit.push_back(a.digit[i] - b.digit[i]);
    }

    for (; i < a.digit.size(); i++) {
        a.digit[i] += dif;
        if (a.digit[i] == 255) {
            a.digit[i] += 10;
            dif = -1;
        } else {
            dif = 0;
        }
        result.digit.push_back(a.digit[i]);
    }


    /*
    for (int i = 0; i < a.digit.size(); i++) {
        a.digit[i] += dif;
        if (i < b.digit.size()) {
            printf("%d\n", i);
            //-1???
            if (a.digit[i] < b.digit[i] || a.digit[i] == 255) {
                a.digit[i] += 10;
                dif = -1;
            } else {
                dif = 0;
            }
            result.digit.push_back(a.digit[i] - b.digit[i]);
        } else {
            //-1??
            if (a.digit[i] == 255) {
                a.digit[i] += 10;
                dif = -1;
            } else {
                dif = 0;
            }
            result.digit.push_back(a.digit[i]);
            }
        }

*/

    for (int i = result.digit.size() - 1; i > 0; i--) {
        if (result.digit[i] != 0) return result;
        else result.digit.erase(result.digit.end() - 1);
    }
    /*
    for (int i = 0; i < result.digit.size() ; i++) {
        printf("res[%d] = %d\n", i, result.digit[i]);
    }
*/
    return result;
}

bool N::chk0() {
    N tmp("0");
    //for (int i = 0; i < this->digit.size() ; i++) {
      //  printf("res[%d] = %d\n", i,this->digit[i]);
    //}
    if (*this == tmp) return true;
    else return false;
}

void N::mul10k (int k)
{
    if (this->chk0()) {
        return;
    }
    for (int i = 0; i < k; i++) {
        this->digit.emplace(this->digit.begin(), 0);
    }
}


void N::divN(N b)
{
    N tmpNull("0");
    N a = *this, result;
    if (a < b) {
        *this = tmpNull;
        return;
    }
    if (b.chk0()) {
        puts("Деление на 0: операция не определена. Возврат исходного значения . . .");
        return;
    }
    int k = a.digit.size() - b.digit.size();
    for (int j = 0; j < k + 1; j++){
        result.digit.push_back(0);
    }

    for (int i = result.digit.size() - 1; a > b || a == b; ) {
        N tmp = b;
        tmp.mul10k(k);
        while (a < tmp) {
            tmp = b;
            tmp.mul10k(--k);
            i--;
        }
        a = a - tmp;
        result.digit[i] += 1;
    }

    for (int i = result.digit.size() - 1; i > 0; i--) {
        if (result.digit[i] != 0) break;
        else result.digit.erase(result.digit.end() - 1);
    }

    *this = result;
}

N operator % (N& a, N& b)
{

    N tmp = a;
    //for (int i = 0; i < tmp.digit.size() ; i++) {
      //  printf("tmp[%d] = %d\n", i,tmp.digit[i]);
    //}
    tmp.divN(b);
    //or (int i = 0; i < tmp.digit.size() ; i++) {
      //  printf("tmp[%d] = %d\n", i,tmp.digit[i]);
    //}
    tmp.mulN(b);
    //tmp = a - tmp;

    //a = a - tmp;

    return a - tmp;
}

string N::to_binstr()
{
    string t, nstr;
    N odd("2"), tmp = *this, ntmp;
    while (tmp > odd || tmp == odd) {
        ntmp = (tmp % odd);
        t += ntmp.digit[0] + '0';
        tmp.divN(odd);
    }
    t += tmp.digit[0] + '0';
    for (int i = t.length() - 1; i >= 0; i--) {
        nstr += t[i];
    }
    return nstr;
}

void N::powmod(N pow, N mod)
{
    N odd("2"), tmpOne("1"), tmpThis = *this;

    if (pow == tmpOne) {
        (*this) = (*this) % mod;
        return;
    }
    string powStr = pow.to_binstr();
    for (int i = 1; i < powStr.length(); i++) {
        this->mulN(*this);
        if (powStr[i] == '1') {
            this->mulN(tmpThis);
        }
        (*this) = (*this) % mod;
    }
}


void N::mulK(int k)
{
    N tmpNull("0");
    if (k == 0) {
        *this = tmpNull;
        return;
    }
    int dif = 0;
     for (int i = 0; i < this->digit.size(); i++) {
         this->digit[i] *= k;
         this->digit[i] += dif;
         dif = 0;
         if (this->digit[i] >= 10) {
             dif = this->digit[i] / 10;
             this->digit[i] = this->digit[i] % 10;
         }
     }
     if (dif != 0) {
         this->digit.push_back(dif);
     }
}

void N::mulN(N b)
{
    N result;
    N bigger = (this->digit.size() < b.digit.size())?b:(*this);
    N smaller = (this->digit.size() < b.digit.size())?(*this):b;
    N tmp = bigger;
    for (int i = 0; i < smaller.digit.size(); i++) {
        tmp.mulK(smaller.digit[i]);
        tmp.mul10k(i);
        result = result + tmp;
        tmp = bigger;
    }
    *this = result;
}


//=======================================================================================
//string N::to_binstr();
//byte_vector N::to_bytevector();

//N N::powmod(N pow, N mod);

