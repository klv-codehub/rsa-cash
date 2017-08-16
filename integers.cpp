#include "integers.h"
#include "print.h"

N::N()
{
 //   digit.push_back(0);
}

N::N(int a)
{
    if(a<0) a*= -1;
    do
    {
         digit.push_back(a%10);
         a = a/10;
    }
    while(a > 0);
}
N::N(QString str)
{
    int i = 0;
    while(i < str.length())
    {
        if('0' <= str[i] && str[i] <= '9')
            if( !(digit.size() == 0 && str[i] == '0') ) //Не читаем ведущие нули
                digit.insert(digit.begin(), str[i].digitValue());// -'0'
        i++;
    }

    if (digit.size() == 0)
        digit.insert(digit.begin(), 0);
}
QString N::to_str() const
{
    QString t;
    for(int i = digit.size()-1; i >=0; i--)
        t+= digit[i] + '0';
    return t;
}

bool operator == (const N& a, const N& b)
{
    if (a.digit.size() != b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] != b.digit[i]) return false;
    }
    return true;
}
bool operator != (const N& a, const N& b)
{
    if (a.digit.size() != b.digit.size()) return true;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] != b.digit[i]) return true;
    }
    return false;
}
bool operator > (const N& a, const N& b)
{
    if (a.digit.size() > b.digit.size()) return true;
    if (a.digit.size() < b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] < b.digit[i]) return false;
        else if (a.digit[i] > b.digit[i]) return true;
    }
    return false;
}
bool operator < (const N& a, const N& b)
{
    if (a.digit.size() < b.digit.size()) return true;
    if (a.digit.size() > b.digit.size()) return false;
    for (int i = a.digit.size() - 1; i >= 0; i--) {
        if (a.digit[i] > b.digit[i]) return false;
        else if (a.digit[i] < b.digit[i]) return true;
    }
    return false;
}

N operator + (const N& a, const N& b)
{
   N bigger = (a.digit.size() < b.digit.size())?b:a;
   N smaller = (a.digit.size() < b.digit.size())?a:b;

   N result;
   int sum = 0;
   for (int i = 0; i < bigger.digit.size(); i++) {
       if (i < smaller.digit.size()) {
           sum += bigger.digit[i] + smaller.digit[i];
       } else {
           sum += bigger.digit[i];
       }
       if (sum <= 9) {
           result.digit.push_back(sum);
           sum = 0;
       }
       else {
           result.digit.push_back(sum % 10);
           sum = 1;
           if (i == bigger.digit.size() - 1) {
               result.digit.push_back(sum);
           }
       }
   }
   dprint(a.to_str() + " + " + b.to_str() + " = " + result.to_str() + '\n');
   return result;
}

N operator- (const N& a, const N& b)
{
    if (a < b) return a;

    N result = a;
    int dif = 0;

    for (int i = 0; i < b.digit.size(); i++) {
        result.digit[i] += dif;
        if (result.digit[i] < b.digit[i] || result.digit[i] == 255) {
            result.digit[i] += 10;
            dif = -1;
        } else {
            dif = 0;
        }
        result.digit[i] -= b.digit[i];
    }

    for (int i = b.digit.size(); i < a.digit.size(); i++) {
        result.digit[i] += dif;
        if (result.digit[i] == 255) {
            result.digit[i] += 10;
            dif = -1;
        } else {
            dif = 0;
        }
    }

    for (int i = result.digit.size() - 1; i > 0; i--) {
        if (result.digit[i] != 0) return result;
        else result.digit.erase(result.digit.end() - 1);
    }
    dprint(a.to_str() + " - " + b.to_str() + " = " + result.to_str() + '\n');
    return result;
}

//умножение на 10 в степени k
void N::mul10k (int k)
{
    if (*this == 0) {
        return;
    }
    for (int i = 0; i < k; i++) {
        this->digit.insert(this->digit.begin(), 0);
    }
}
//умножение на цифру
void N::mulK(int k)
{
    if (k == 0) {
        *this = 0;
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

N operator * (const N& a, const N& b)
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
    dprint(a.to_str() + " * " + b.to_str() + " = " + result.to_str() + '\n');
    return result;
}

N operator / (const N& a, const N& b)
{
    if (a == 0 || a < b) return 0;
    if (b == 0) throw;
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
            if (tmp == 0) tmp.digit.erase(tmp.digit.begin());
            tmp.digit.insert(tmp.digit.begin(), a.digit[i--]);
        } else break;
    }
    dprint(a.to_str() + " / " + b.to_str() + " = " + result.to_str() + '\n');
    return result;
}


N operator % (const N& a, const N& b)
{
    N result = a - (a/b)*b;
    dprint(a.to_str() + " % " + b.to_str() + " = " + result.to_str() + '\n');
    return result ; //Для ускорения можно реализовать как изменённое целочисленное деление
}

//перевод в двоичную строку
QString N::to_binstr() const
{
    dprint("BINSTR started\n");
    QString t, nstr;
    N tmp = *this, ntmp;
    while (tmp > 2 || tmp == 2) {
        ntmp = (tmp % 2);
        t += ntmp.digit[0] + '0';
        tmp = tmp / 2;
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

//Перевод длинного числа в массив байтов big-endian
QByteArray N::to_bytearray() const
{
    QByteArray bin_array;
    N div = *this, rem; //divident, remainder
    char byte, bit;

    while (div != 0)
    {
        byte = bit = 0;     //Обнуляем байт и начинаем формировать его с первого с конца бита (нулевого)

        while (bit != 8)    //Побайтно формируем результирующий массив
        {
            rem = div % 2;              //Находим очередной остаток "лесенки"
            if(rem.digit[0] == 1)       //Если остаток от деления на 2 - еденица
                byte = byte | 1<<bit;   //Устанавливаем в еденицу соответствующий бит
            div = div / 2;              //Обновляем очередное делимое "лесенки
            bit++;                      //Идём к следующему биту в формируемом байте
        }

        //bin_array.insert(bin_array.length(), byte);      //(Little endian) Вставляем очередной сформированный байт в начало массива
        bin_array.insert(0, byte);      //(Big endian) Вставляем очередной сформированный байт в начало массива
    }

    return bin_array;
}

//возведение в степень по модулю
N N::powmod(const N pow, const N mod) const
{
    N res = *this;
    if (pow == 1) {
        res = res % mod;
        return res;
    }

    QString powStr = pow.to_binstr();

    for (int i = 1; i < powStr.length(); i++) {
        dprint(QString::number(i) + "\n");

        res = res * res;
        if (powStr[i] == '1') {
            res = res *(*this);
        }
        res = res % mod;
    }
    dprint(this->to_str() + " ^ " + pow.to_str() + " mod " + mod.to_str() + " = " + res.to_str() + '\n');
    return res;
}
//нахождение обратного по модулю
N N::revmod(const N mod) const
{
    N a = *this, b = mod, q, result;
    Z Va = 1, Vb = 0;

    dprint('\n' + a.to_str() + ' ' + mod.to_str() + '\n');

    while(b!=0)
    {
        q = a/b;
        a = a-b*q;
        Va = Va-Vb*q;
        qSwap(a,b);
        qSwap(Va, Vb);
    }

    if(a == 1)//НОД должен быть равен 1 для существования обратного
        if(Va < 0) result = (Va + mod).abs();
        else result = Va.abs();
    else result = 0;
    dprint(this->to_str() + " = 1/" + result.to_str() + " mod " + mod.to_str() + '\n');
    return result;
}

// Zclass =================================================================
Z::Z(int a)
{
    sign = a<0?true:false;
    module = a;
}
Z::Z(const QString str)
{
    sign = (str[0] == '-')?true:false;
    module = str + sign;
}
Z::Z(const N& a)
{
    sign = false;
    module = a;
}
QString Z::to_str() const
{
    QString t;
    if(sign) t += '-';
    t+= module.to_str();
    return t;
}
N Z::abs()
{
    return module;
}

bool operator == (const Z& a, const Z& b){
    //Два числа равны только тогда, когда они равны и по модулю и по знаку.
    return (a.module == b.module && a.sign == b.sign);
}
bool operator > (const Z& a, const Z& b){
    //Если числа равны по модулю, то a>b только тогда, когда a>0, а b<0.
    if(a.module == b.module) return (b.sign - a.sign);
    //Иначе, если число с большим модулем отрицательно, то оно меньше.
    return (a.module > b.module)?(a.sign?false:true):(b.sign?true:false);
}
bool operator < (const Z& a, const Z& b){
    //Если числа равны по модулю, то a<b только тогда, когда a<0, а b>0.
    if(a.module == b.module) return (a.sign - b.sign);
    //Иначе, если число с большим модулем отрицательно, то оно меньше.
    return (a.module < b.module)?(b.sign?false:true):(a.sign?true:false);
}

Z operator + (const Z& a, const Z& b)
{
    Z res;
    if (a.sign != b.sign) {
        if (a.module != b.module) {
            //Если знаки чисел и их модули различны, то
            //результирующий модуль вычисляется как разность большего и меньшего модулей чисел,
            //результирующий знак равен знаку большего по модулю числа
            res.module = (a.module > b.module)?(a.module - b.module):(b.module - a.module);
            res.sign = (a.module > b.module)?(a.sign):(b.sign);
        }
    } else {
        //Если знаки чисел совпадают, то результирующий модуль равен сумме модулей,
        //результирующий знак равен знаку первого числа
        res.module = (a.module + b.module);
        res.sign = a.sign;
    }
    return res;
}

Z operator - (const Z& a, const Z& b)
{
    Z res;
    if (b.sign) {
        //Если знак второго числа "минус", то вычитание превращается в сложение -> обращаемся к оператору сложения
         Z tmpB = b;
         tmpB.sign = false;
         return (a + tmpB);
    }
    if (a.sign) {
        //Если знак первого числа "минус" (знак второго числа всегда "плюс"),
        //то складываем модули чисел, знак результриюущего числа отрицателен
        res.module = a.module + b.module;
        res.sign = a.sign;
    } else {
        //Если знак обоих чисел "минус", то находим модуль результриующего числа как разность модулей
        //большего и меньшего чисел, если разность модулей чисел >= 0,
        //то знак результирующего числа "плюс", в противном случае - "минус"
        res.module = (a.module > b.module)?(a.module - b.module):(b.module - a.module);
        res.sign = (a.module > b.module || a.module == b.module)?(false):(true);
    }
    return res;
}

Z operator * (const Z& a, const Z& b)
{
    Z res;
    res.module = a.module * b.module;
    res.sign = a.sign? b.sign?false:true : b.sign?true:false;
    return res;
}
