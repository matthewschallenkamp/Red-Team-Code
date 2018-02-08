#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod = pow(10, 9)+7;

ll power(ll a, ll b) {
    if (b <= 0) return 1;
    ll temp = power(a, b / 2) % mod;
    if (b % 2 == 0) {
        return (temp * temp) % mod;
    }
    else {
        return (((a * temp) % mod) * temp) % mod;
    }
}

struct NchooseK {
    int range;
    vector<int> fact,ifact;
    NchooseK() {
        range = (int)1e6;
        fact.resize(range+1);
        ifact.resize(range+1);
        calcFacts();
    }
    void calcFacts() {
        fact[0] = 1;
        for(ll i = 1; i <= range; ++i) {
            fact[i] = (1LL*fact[i-1]*i)%mod;
        }
        ifact[range] = power(fact[range], mod-2);
        for(int i = range-1; i >= 0; --i) {
            ifact[i] = (1LL*ifact[i+1]*(i+1))%mod;
        }
    }
    int choose(int n, int k) {
        if(k < 0 || k > n || n < 0) return 0;
        return ((1LL*fact[n]*ifact[k])%mod * 1LL*ifact[n-k])%mod;
    }
};

ll triangle(ll num) {
    if (num <= 0) return 0;
    return (num * (num + 1) / 2)%mod;
}

int main() {
    NchooseK nk;
    string sequence;
    cin >> sequence;
    ll length = sequence.size(), iSum = 0, qSum = 0, firstPartOfSum = 0, secondPartOfSum = 0;
    int iCount = 0, k = 0;
    for (int i = 0; i < length; i++) {
        if (sequence[i] == '1') {
            iCount++;
            iSum += length - 1 - i;
        }
        if (sequence[i] == '?') {
            k++;
            qSum += length - 1 - i;
        }
    }
    /*first part of sum*/
    iSum%=mod;
    qSum%=mod; 
    firstPartOfSum = (iSum * power(2, k))%mod;
    firstPartOfSum += (qSum * power(2, k - 1))%mod;
    firstPartOfSum %= mod;
    
    /*second part of sum*/
    for (ll i = 0; i <= k; i++) {
        secondPartOfSum += (nk.choose(k, i) * triangle(i + iCount - 1))%mod;
        secondPartOfSum %= mod;
    }
    cout << (firstPartOfSum - secondPartOfSum + mod)%mod << '\n';
    return 0;
}



