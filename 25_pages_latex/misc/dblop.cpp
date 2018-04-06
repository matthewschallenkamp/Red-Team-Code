#include <iostream>
using namespace std;

typedef long double dbl;
const dbl ITER = 100000000;

int main()
{
    dbl j=0;
    for(dbl i = 1; i < ITER; i++)
        j+=ITER*i;
    
    cout << j << endl;
    return 0;
}