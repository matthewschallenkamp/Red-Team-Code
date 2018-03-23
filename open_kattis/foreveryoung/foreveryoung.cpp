#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll age, lower;

bool divide(ll base) {
    ll divisor = 1;
    while(divisor < age/base+2) {
        divisor *= base;
    }
    vector<int> digits;
    ll tempAge = age;
    while(divisor > 0) {
        ll mult = tempAge/divisor;
        if(mult >= 10) return false;
        digits.push_back(mult);
        tempAge -= mult * divisor;
        divisor /= base;
    }
    int numDigits = (int)floor(log10l(lower))+1;
    if(digits.size() > numDigits) return true;
    if(digits.size() < numDigits) return false;
    ll sum = 0;
    for(int x : digits) {
        sum *= 10;
        sum += x;
    }
    return sum >= lower;
}

ll getNum(ll newAge, ll base) {
    vector<int> digits;
    while(newAge > 0) {
        digits.push_back(newAge%10);
        newAge /= 10;
    }
    ll oldAge = 0;
    for(int i = digits.size()-1; i >= 0; --i) {
        if(oldAge <= age/base+1) oldAge *= base;
        else {
            return age+1;
        }
        oldAge += digits[i];
    }
    return oldAge;
}

int main() {
    cin >> age >> lower;
    ll answer = 10;
    int bound = min((ll)1e5, age);
    for(int base = 11; base <= bound; ++base) {
        if(divide(base)) {
            answer = base;
        }
    }
    bound = lower+1e4;//change this later
    //binary search for the base
    for(int tempAge = lower; tempAge < bound; ++tempAge) {
        ll start = 10, end = age+1, mid;
        while(end-start > 1) {
            mid = (start+end)>>1;
            if(getNum(tempAge, mid) <= age) start = mid;
            else end = mid;
        }
        if(getNum(tempAge, start) == age) {
            answer = max(answer, start);
        }
    }
    cout << min(answer, age) << '\n';
    return 0;
}


























