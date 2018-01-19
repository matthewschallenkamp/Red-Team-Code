#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

//https://open.kattis.com/problems/wipeyourwhiteboards

//Solution: Bézout's identity + binary search
//given (R,S,Q), find (A,B) which satisfy: R*A+S*B=Q
//extended gcd for 2 numbers: (R,S) finds (A,B) such that R*A+S*B=gcd(R,S)
//Bézout's identity: given one solution (A,B) to (R*A+S*B=Q), all solutions
//to that equation are in form (A+k*S/gcd(R,|S|), B-k*R/gcd(R,|S|)) for any integer k
//we want both coefficients (A+k*S/gcd(R,|S|), B-k*R/gcd(R,|S|)) to be positive
//since the coefficients follow linear progression both with negative slope, we 
//can use binary search to find maximal k which give positive coefficients.
//it can be shown that the extended gcd runs in O(log(max(a,b))) time
//O(log(2*10^9)) per test case

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

