#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<pair<int, int> > lower, upper;

ll area(int i, int j) {
    ll length = upper[j].second-lower[i].second;
    ll width = upper[j].first-lower[i].first;
    if(length <= 0 || width <= 0) return -abs(length)*abs(width);
    return length*width;
}

ll maxRange(int startL, int endL, int startU, int endU) {
    if(startL > endL) return 0;
    int midL = (startL+endL)>>1;
    ll answer = -1e18;
    int bestUindex = startU;
    for(int j = startU; j <= endU; ++j) {
        ll tempArea = area(midL, j);
        if(tempArea >= answer) {
            answer = tempArea;
            bestUindex = j;
        }
    }
    if(startL < endL) {
        answer = max(answer, maxRange(startL, midL-1, startU, bestUindex));
        answer = max(answer, maxRange(midL+1, endL, bestUindex, endU));
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    lower.resize(n);
    upper.resize(m);
    for(auto &p : lower) cin >> p.first >> p.second;
    for(auto &p : upper) cin >> p.first >> p.second;
    sort(lower.begin(), lower.end());
    int i = 0,j;
    vector<pair<int, int> > pruned;
    while(i < n) {
        j = i;
        pruned.push_back(lower[i]);
        while(j < n && lower[j].second >= lower[i].second) j++;
        i = j;
    }
    lower.swap(pruned);
    pruned.clear();
    sort(upper.rbegin(), upper.rend());
    i = 0;
    while(i < m) {
        j = i;
        pruned.push_back(upper[i]);
        while(j < m && upper[j].second <= upper[i].second) j++;
        i = j;
    }
    upper.swap(pruned);
    reverse(upper.begin(), upper.end());
    cout << max(0LL,maxRange(0,lower.size()-1,0,upper.size()-1)) << '\n';
    return 0;
}

