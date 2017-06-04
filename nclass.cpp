#include "common.h"
#include "nclass.h"
#include "mainwindow.h"

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

//Оператор "+"
N operator + (N& a, N& b)
{
    /*
     * Переменной bigger пирсваивается число с большим кол-вом цифр, smaller - с меньшим
     */
    N bigger = (a.digit.size() < b.digit.size())?b:a;
    N smaller = (a.digit.size() < b.digit.size())?a:b;

    /*
     * result - результирующее число
     * sum - переменная, хранящая сумму текущих цифр
     */
   N result;
   int sum = 0;
   for (int i = 0; i < bigger.digit.size(); i++) {
       /*
        * Если меньшее число не закончилось, цифры обоих чисел суммируются.
        */
       if (i < smaller.digit.size()) {
           sum += bigger.digit[i] + smaller.digit[i];
       } else {
           sum += bigger.digit[i];
       }
       /*
        * Если сумма цифр меньше или равна 9, то она записывается в разряд результирующего числа,
        * переменная sum обнуляется
        */
       if (sum <= 9) {
           result.digit.push_back(sum);
           sum = 0;
       }
       /*
        * Иначе в результирующее число записывается остаток по модулю 10,
        * переменная sum становится равной 1
        */
       else {
           result.digit.push_back(sum % 10);
           sum = 1;
           /*
            * Если это последний разряд числа наибольшего числа,
            * то в дополнительный разряд результирующего числа записывается единица
            */
           if (i == bigger.digit.size() - 1) {
               result.digit.push_back(sum);
           }
       }
   }
   /*
    * Возвращает результирующее число
    */
   return result;
}

//Оператор "=="
bool operator == (N& a, N& b)
{
    /*
     * Возвращает true, если числа равны,
     * иначе возвращает false
     */
    if (a.digit.size() != b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] != b.digit[i]) return false;
    }
    return true;
}

//Оператор ">"
bool operator > (N& a, N& b)
{
    /*
     * Возвращает true, если (a > b),
     * иначе возвращает false
     */
    if (a.digit.size() > b.digit.size()) return true;
    if (a.digit.size() < b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] < b.digit[i]) return false;
        else if (a.digit[i] > b.digit[i]) return true;
    }
    return false;
}

//Оператор "<"
bool operator < (N& a, N& b)
{
    /*
     * Возвращает true, если (a < b),
     * иначе возвращает false
     */
    if (a.digit.size() < b.digit.size()) return true;
    if (a.digit.size() > b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] > b.digit[i]) return false;
        else if (a.digit[i] < b.digit[i]) return true;
    }
    return false;
}

//Оператор "-"
N operator - (N& a, N& b)
{
    /*
     * Если уменьшаемое число меньше вычитаемого, то возвращается уменьшаемое
     */
    if (a < b) return a;
    /*
     * result - результирующее число
     * dif - переменная, хранящая разность текущих цифр
     */
    N result;
    int dif = 0;
    /*
     * Вычитание производится, пока не закончились цифры в меньшем числе
     */
    for (int i = 0; i < b.digit.size(); i++) {
        a.digit[i] += dif;
        if (a.digit[i] < b.digit[i] || a.digit[i] == 255) {
            a.digit[i] += 10;
            dif = -1;
        } else {
            dif = 0;
        }
        result.digit.push_back(a.digit[i] - b.digit[i]);
    }
    /*
     * Вычитание продолжается, пока не закончились цифры в большем числе
     */
    for (int i = b.digit.size(); i < a.digit.size(); i++) {
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
     * Удаление незначащих нулей в результирующем числе
     */
    for (int i = result.digit.size() - 1; i > 0; i--) {
        if (result.digit[i] != 0) return result;
        else result.digit.erase(result.digit.end() - 1);
    }
    /*
     * Возвращает результирующее число
     */
    return result;
}

//Метод класса N: умножение на 10 в степени k
void N::mul10k (int k)
{
    /*
     * Если исходное число равно 0, возвращает 0
     */
    N tmpNull("0");
    if (*this == tmpNull) {
        return;
    }
    /*
     * Добавляет разряды к исходному числу
     */
    for (int i = 0; i < k; i++) {
        this->digit.emplace(this->digit.begin(), 0);
    }
}

//Оператор "/"
N operator / (N& a, N& b)
{
    N tmpNull("0");
    if (a == tmpNull) return tmpNull;
    if (b == tmpNull) throw;
    int k = a.digit.size() - b.digit.size() + 1;
    N tmp, result;
    tmp.digit.push_back(a.digit[a.digit.size() - 1]);
    int i;
    for (i = a.digit.size() - 2; tmp < b && i >= 0; i--) {
        tmp.digit.insert(tmp.digit.begin(), a.digit[i]);
    }
    for (int j = 0; j < k; j++) {
        result.digit.insert(result.digit.begin(), 0);
        while (tmp > b || tmp == b) {
            tmp = tmp - b;
            result.digit[0] += 1;
        }
        if (i >= 0) {
            if (tmp == tmpNull) tmp.digit.erase(tmp.digit.begin());
            tmp.digit.insert(tmp.digit.begin(), a.digit[i--]);
        } else break;
    }
    return result;
}

//Оператор "%"
N operator % (N& a, N& b)
{
    /*
     * Находит целую часть от деления чисел, умножает результат на исходный делитель, получившееся число
     * вычитает из исходного делимого и возвращает
     */
    N tmp = a;
    tmp = tmp / b;
    tmp = tmp * b;
    return a - tmp;
}

//Метод класса N: перевод в двоичную строку
string N::to_binstr()
{
    dprint("BINSTR started\n");
    string t, nstr;
    N odd("2"), tmp = *this, ntmp;
    while (tmp > odd || tmp == odd) {
        ntmp = (tmp % odd);
        t += ntmp.digit[0] + '0';
        tmp = tmp / odd;
    }
    t += tmp.digit[0] + '0';
    /*
     * Символы в строке хранятся в обратном порядке, т.е. если число не 0, первый символ всегда '1'
     */
    for (int i = t.length() - 1; i >= 0; i--) {
        nstr += t[i];
    }
    dprint("BINSTR ended\n");
    return nstr;
}

//Метод класса N: возведение в степень по модулю
N N::powmod(N pow, N mod)
{
    dprint("POTOK VADIMA\n");
    N tmpOne("1"), res = *this;
    /*
     * Если степень равна 1, то производится деление по модулю и метод завершает своб работу
     */
    if (pow == tmpOne) {
        res = res % mod;
        return res;
    }
    /*
     * Для возведения числа в степень применяется алгоритм бинарного возведения.
     * Каждый шаг возведения сопровождается делением по модулю, для того
     * чтобы уменьшить длительность операции возведения
     */
    dprint("VTOROI POTOK VADIM!!!!\n");
    string powStr = pow.to_binstr();
    dprint("333 POTOK VADIM!!!!\n");
    for (int i = 1; i < powStr.length(); i++) {
        dprint(std::to_string(i) + "\n");
        //res.mulN(res);
        res = res * res;
        if (powStr[i] == '1') {
            res = res *(*this);
        }
        res = res % mod;
    }
    return res;
}

//Метод класса N: умножение на цифру
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

//Оператор "*"
N operator * (N& a, N& b)
{
    N result;
    N bigger = (a.digit.size() < b.digit.size())?b:a;
    N smaller = (a.digit.size() < b.digit.size())?a:b;
    N tmp = bigger;
    for (int i = 0; i < smaller.digit.size(); i++) {
        tmp.mulK(smaller.digit[i]);
        tmp.mul10k(i);
        result = result + tmp;
        tmp = bigger;
    }
    return result;
}


//=======================================================================================
//string N::to_binstr();
//byte_vector N::to_bytevector();

//N N::powmod(N pow, N mod);

