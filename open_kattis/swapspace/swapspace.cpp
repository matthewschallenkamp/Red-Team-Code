#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

bool cmp(pair<ll, ll> x, pair<ll, ll> y) {
    ll diffX = x.second - x.first;
    ll diffY = y.second - y.first;
    if((diffX >= 0) ^ (diffY >= 0)) {
        return diffX > diffY;
    }
    if(diffX >= 0) {
        if(x.first == y.first) return diffX > diffY;
        return x.first < y.first;
    }
    if(x.second == y.second) return diffX > diffY;
    return x.second > y.second;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    vector<pair<ll, ll> > arr(n);
    for(auto &p : arr) cin >> p.first >> p.second;
    sort(arr.begin(), arr.end(), cmp);
    ll swap = 0, extra = 0;
    for(auto &p : arr) {
        ll oldSpace = p.first;
        ll newSpace = p.second;
        if(oldSpace > swap + extra) {
            swap += oldSpace - (swap + extra);
        }
        extra += newSpace - oldSpace;
    }
    cout << swap << '\n';
    return 0;
}
