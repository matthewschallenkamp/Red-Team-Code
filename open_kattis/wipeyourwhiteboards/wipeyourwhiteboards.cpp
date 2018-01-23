#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

ll extendedGCD(ll a, ll b, ll &x, ll &y) { 
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    ll x1, y1;
    ll d = extendedGCD(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

int main()
{
    int cases;
    cin >> cases;
    for(int i=0; i<cases; i++)
    {
        ll R, S, Q;
        cin >> R >> S >> Q;
        ll A, B, gcd;
        gcd = extendedGCD(R, -S, A, B);
        ll mult = Q/gcd;
        B = -B;
        ll start = -1e9, end = 1e9, k;
        while(end-start>1) {
            k = (start+end)>>1;
            if(mult*A + k*S/gcd > 0 && mult*B - k*R/gcd > 0) {
                start = k;
            } else end = k;
        }
        cout << mult*A + start*S/gcd << ' ' << mult*B - start*R/gcd << '\n';
    }
  
}

