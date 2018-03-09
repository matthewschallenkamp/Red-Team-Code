#include <bits/stdc++.h>
using namespace std;

int memoFull[51][51][401][51], memo[51][51][401][51];
string s;
vector<pair<string, int> > deals;

int maxValueFull(int i, int j, int p, int k) {
    if(i >= j) {
        if(k == deals[p].first.size()) {
            return deals[p].second;
        }
        return 0;
    }
    if(p == deals.size()) {
        return -2;
    }
    int &largest = memoFull[i][j][p][k];
    if(largest != -1) return memoFull[i][j][p][k];
    largest = -2;
    if(k == deals[p].first.size()) {
        return largest = deals[p].second;
    }
    if(s[i] == deals[p].first[k]) {
        int temp = maxValueFull(i+1,j,p,k+1); 
        if(temp != -2) {
            largest = max(largest, temp);
        }
    }
    for(int m = i+1; m <= j; ++m) {
        int temp = maxValueFull(i,m,0,0);
        if(temp != -2) {
            int temp2 = maxValueFull(m,j,p,k);
            if(temp2 != -2) {
                largest = max(largest, temp + temp2);
            }
        }
    }
    largest = max(largest, maxValueFull(i,j,p+1,0));
    return largest;
}

int maxValue(int i, int j, int p, int k) {
    if(p == deals.size() || i >= j) return 0;
    if(k == deals[p].first.size()) {
        return deals[p].second + maxValue(i,j,p+1,0);
    }
    if(memo[i][j][p][k] != -1) return memo[i][j][p][k];
    int largest = maxValue(i,j,p+1,0);
    if(s[i] == deals[p].first[k]) {
        largest = maxValue(i+1,j,p,k+1);
    }
    for(int m = i+1; m <= j; ++m) {
        int temp = maxValueFull(i,m,0,0);
        if(temp != -2) {
            largest = max(largest, temp + maxValue(m,j,p,k));
        }
    }
    memo[i][j][p][k] = largest;
    return largest;
}

int main() {
    map<string, int> best;
    int n;
    cin >> s >> n;
    string segment;
    int value;
    for(int i = 0; i < n; ++i) {
        cin >> segment >> value;
        best[segment] = max(best[segment], value);
        reverse(segment.begin(), segment.end());
        best[segment] = max(best[segment], value);
    }
    for(auto &p : best) {
        deals.push_back(p);
    }
    n = s.size();
    for(int i = 0; i <= n; ++i)
        for(int j = 0; j <= n; ++j)
            for(int p = 0; p < deals.size(); ++p)
                for(int k = 0; k <= deals[p].first.size(); ++k)
                    memo[i][j][p][k] = memoFull[i][j][p][k] = -1;
    cout << maxValue(0,n,0,0) << '\n';
    return 0;
}




























