#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

int color[257], amount[257];
ll minIntensity[257][257], cost[257][257];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int d, k;
    cin >> d >> k;
    for(int i = 0; i < d; ++i) cin >> color[i] >> amount[i];
    for(int i = 0; i < d; ++i) {
        ll sum = 0, size = 0;
        for(int j = i; j < d; ++j) {
            sum += 1LL*color[j]*amount[j];
            size += amount[j];
            ll bestColor = round(sum/(ld)size);
            cost[i][j] = 0;
            for(int l = i; l <= j; ++l) {
                cost[i][j] += 1LL * amount[l] * (color[l]-bestColor) * (color[l]-bestColor);
            }
        }
    }
    for(int i = 0; i < d; ++i) {
        for(int j = 0; j < d; ++j) {
            minIntensity[i][j] = 1e18;
        }
    }
    for(int i = 0; i < d; ++i) {
        minIntensity[i][1] = cost[0][i];
    }
    for(int i = 0; i < d; ++i) {
        for(int j = 2; j <= k; ++j) {
            for(int l = 0; l < i; ++l) {
                minIntensity[i][j] = min(minIntensity[i][j], minIntensity[l][j-1] + cost[l+1][i]);
            }
        }
    }
    cout << minIntensity[d-1][k] << '\n';
    return 0;
}





























