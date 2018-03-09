#include <bits/stdc++.h>
using namespace std;

bool cmp(pair<int, int> a, pair<int, int> b) {
    if (a.second == b.second) return a.first < b.first;
    return a.second < b.second;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, k;
    cin >> n >> k;
    vector<pair<int, int> > arr(n);
    for (auto &p : arr) {
        cin >> p.first >> p.second;
        p.second++;
    }
    multiset<int> lasts;
    sort(arr.begin(), arr.end(), cmp);
    for(int i = 0; i < k; ++i) lasts.insert(0);
    int counter = 0;
    multiset<int>::iterator it;
    for (auto &p : arr) {
        if (p.first >= *(lasts.begin())) {
            if (lasts.count(p.first)) {
                lasts.erase(lasts.find(p.first));
                lasts.insert(p.second);
            } else {
                it = lasts.lower_bound(p.first);
                it--;
                lasts.erase(it);
                lasts.insert(p.second);
            }
            counter++;
        }
    }
    cout << counter << '\n';
    return 0;
}
