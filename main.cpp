#include <iostream>

#include "common.h"
#include "nclass.h"

int main()
{

    //  трабла с нулями!! С НУЛЯМИ НЕЗНАЧАЩИМИ

    N a("1034");
    N pow("156");
    N b("254");

    //a.divN(b);
    //string vadim = a.to_binstr();

    cout << endl << a.to_binstr() << endl;
    a.powmod(pow, b);
    cout << a.to_str() << endl;
    //N c = a - b;
    //cout << "1-[" << a.to_str() << endl;

   // c.mulK(3);
    //cout << "2 " << c.to_str() << endl;

    //a.divN(b);
    //cout << "3 " << a.to_str() << endl;

    //a.divN(b);
    //cout << "a = " << a.to_str() << endl;

    //b.mulN(a);
    //a.mulN(b);
    //cout << "b = " << b.to_str() << endl;

    //N c = a % b;
    //cout << "c = " << c.to_str() << endl;

    //cout << "binstr[" << a.to_binstr() << "]" << endl;

    return 0;
}
