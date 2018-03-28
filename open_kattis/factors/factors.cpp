#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ll;
typedef long double ld;
const ll infll = powl(2,63) + 2;

vector<ll> primes = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89};
ll n, mults[150][150];
ll primePowers[100][70];

void smallestK(int pos, int prevMax, int numSize, ll combinations, ll kVal, ll &smallestVal) {
    if(combinations == n) {
        if(kVal < smallestVal) {
            smallestVal = kVal;
        }
        return;
    }
    if(pos == primes.size()) return;
    for(int currPow = 1; currPow <= prevMax; ++currPow) {
        ll currMult = mults[numSize][currPow];
        if(currMult == infll) return;
        if(combinations >= n/currMult + (n%currMult==0)) return;
        if(primePowers[pos][currPow] == 0) return;
        if(kVal >= infll/primePowers[pos][currPow] + (infll%primePowers[pos][currPow] != 0)) return;
        smallestK(pos+1, currPow, numSize + currPow, combinations * currMult, kVal * primePowers[pos][currPow], smallestVal);
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    for(int numSize = 0; numSize < 70; ++numSize) {
        for(int currPow = 0; currPow <= 70; ++currPow) {
            ld newCombs = 1;
            int dem = 2;
            for(int i = numSize + currPow; i > numSize; --i) {
                if(log10l(newCombs) > 19) {
                    mults[numSize][currPow] = infll;
                    break;
                }
                newCombs *= (ld)i;
                while(dem <= currPow && newCombs > 1 && abs(floor(newCombs/dem)*(ld)dem-newCombs) < 0.000001) {
                    newCombs /= dem;
                    dem++;
                }
            }
            mults[numSize][currPow] = (ll)newCombs;
        }
    }
    for(int i = 0; i < primes.size(); ++i) {
        ll currPow = 1;
        for(int j = 0;; j++) {
            primePowers[i][j] = currPow;
            if(currPow >= infll/primes[i] + (infll/primes[i] != 0)) break;
            currPow *= primes[i];
        }
    }
    while(cin >> n) {
        if(n == 1) {
            cout << "1 2\n";
            continue;
        }
        ll k = infll-2;
        smallestK(0,1000,0,1,1,k);
        cout << n << ' ' << setprecision(0) << fixed << k << '\n';
    }
    return 0;
}





































