#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int Max = 1e5 + 3;
ll frac[Max], cnt[Max];

struct canTake {
    ll start, finish;
    int type;
    canTake() {}
    canTake(int a, int b, int c) : start(a), finish(b), type(c) {}
    bool operator< (const canTake &rhs) const {
        return this->finish < rhs.finish;
    }
};

struct cantTake {
    ll start, finish;
    int type;
    cantTake() {}
    cantTake(int a, int b, int c) : start(a), finish(b), type(c) {}
    bool operator< (const cantTake &rhs) const {
        if(this->start == rhs.start) {
            return this->finish < rhs.finish;
        }
        return this->start < rhs.start;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cout << setprecision(2) << fixed;
    int m, k;
    cin >> m >> k;
    ll n = 0, denom = 0;
    for(int i = 1; i <= m; ++i) {
        cin >> frac[i];
        denom += frac[i];
    }
    int sweet;
    for(int i = 1; i <= k; ++i) {
        cin >> sweet;
        cnt[sweet]++;
    }
    int time = k;
    multiset<canTake> can;
    multiset<cantTake> cant;
    for(int j = 1; j <= m; ++j) {
        ll start = (denom*(cnt[j]-1))/frac[j]+1;
        ll finish = (int)ceil(denom*(cnt[j]+1)/(double)frac[j])-1;
        if(start <= time && time <= finish) {
            can.insert(canTake(start, finish, j));
        } else {
            cant.insert(cantTake(start, finish, j));
        }
    }
    for(time = k; time - k < 1e6; ++time) {
        while(cant.size() > 0 && cant.begin()->start <= time) {
            cantTake curr = *cant.begin();
            cant.erase(cant.begin());
            can.insert(canTake(curr.start, curr.finish, curr.type));
        }
        if(can.size() == 0) {
            cout << time-k << '\n';
            return 0;
        }
        canTake curr = *can.begin();
        can.erase(can.begin());
        if(can.size() > 0 && curr.finish == can.begin()->finish && curr.finish == time) {
            cout << time-k << '\n';
            return 0;
        }
        int j = curr.type;
        cnt[j]++;
        ll start = (denom*(cnt[j]-1))/frac[j]+1;
        ll finish = (int)ceil(denom*(cnt[j]+1)/(double)frac[j])-1;
        if(start <= time && time <= finish) {
            can.insert(canTake(start, finish, j));
        } else {
            cant.insert(cantTake(start, finish, j));
        }
    }
    cout << "forever\n";
    return 0;
}























