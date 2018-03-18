#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct card {
    ll l, h, cnt;
};

ll cost[(1<<15)], memo[16][(1<<15)];

int main() {
    int n, k;
    cin >> n >> k;
    vector<card> arr(n);
    for(int i = 0; i < n; ++i) {
        cin >> arr[i].l >> arr[i].h >> arr[i].cnt;
    }
    for(int mask = 0; mask < (1<<n); ++mask) {
        ll maxL = 0, maxH = 0;
        for(int bit = 0; bit < n; ++bit) {
            if(mask&(1<<bit)) {
                maxL = max(maxL, arr[bit].l);
                maxH = max(maxH, arr[bit].h);
            }
        }
        cost[mask] = 0;
        for(int bit = 0; bit < n; ++bit) {
            if(mask&(1<<bit)) {
                cost[mask] += (maxL*maxH - arr[bit].l*arr[bit].h) * arr[bit].cnt;
            }
        }
    }
    for(int K = 0; K <= k; ++K) {
        for(int mask = 0; mask < (1<<n); ++mask) {
            memo[K][mask] = 1e16;
        }
    }
    memo[0][0] = 0;
    for(int K = 1; K <= k; ++K) {
        for(int mask = 0; mask < (1<<n); ++mask) {
            for(int subMask = mask; subMask > 0; subMask = (subMask-1)&mask) {
                memo[K][mask] = min(memo[K][mask], memo[K-1][mask&(~subMask)] + cost[subMask]);
            }
        }
    }
    cout << memo[min(k,n)][(1<<n)-1] << '\n';
    return 0;
}






























