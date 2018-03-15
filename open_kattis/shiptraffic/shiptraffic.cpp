#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    double w, u, v, t1, t2;
    cin >> n >> w >> u >> v >> t1 >> t2;
    char dir;
    int num;
    double length, pos;
    vector<pair<double, double> > ships;
    for(int i = 0; i < n; ++i) {
        cin >> dir >> num;
        double time = w/v*i;
        while(num--) {
            cin >> length >> pos;
            double endPos;
            if(dir == 'E') {
                endPos = pos-length;
                pos *= -1;
                endPos *= -1;
            } else {
                endPos = pos+length;
            }
            pos -= time*u;
            endPos -= time*u;
            ships.push_back({pos/u-w/v, endPos/u});
        }
    }
    sort(ships.begin(), ships.end());
    double maxSoFar = t1, maxTime = 0;
    for(auto &p : ships) {
        if(p.first > maxSoFar) {
            maxTime = max(maxTime, min(p.first,t2) - maxSoFar);
        }
        maxSoFar = max(maxSoFar, p.second);
        if(maxSoFar >= t2) break;
    }
    cout << setprecision(4) << fixed << max(maxTime, t2-maxSoFar) << '\n';
    return 0;
}
