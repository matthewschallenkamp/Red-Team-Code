#include <bits/stdc++.h>
using namespace std;

const int upper = 2*1e6;
vector<int> seive, memo;

//returns 0 iff there exists a prime p s.t. n%(p^2)=0
//returns -1 iff n has an odd number of distinct prime factors
//returns 1 iff n has an even number of distinct prime factors
int mobius(int n) {
    int &temp = memo[n];
    if(temp != -2) return temp;
    int factors = 0, counter = 1, prev = 0;
    while(n > 1) {
        if(prev == seive[n]) counter++;
        else counter = 1;
        if(counter == 2) {
            return temp = 0;
        }
        prev = seive[n];
        n /= seive[n];
        factors++;
    }
    if(counter == 2) {
        return temp = 0;
    }
    return temp = (factors%2==0?1:-1);
}

void doSeive() {
    seive.resize(upper+1);
    seive[0] = seive[1] = 1;
    for (int i = 2; i <= upper; ++i) seive[i] = i;//O(n)
    int root = ceil(sqrt(upper));
    for (int i = 2; i <= root; ++i) {//O(nloglogn) ?
        if (seive[i] == i) {
            for (int j = 2 * i; j <= upper; j += i) seive[j] = min(seive[j], i);
        }
    }
}

int main() {
    doSeive();
    memo.resize(upper+1, -2);
    for(int i = 1; i <= 20; ++i) {
        cout << i << (seive[i]==i?" prime ":" not prime ") << seive[i] << '\n';
    }
    return 0;
}

















