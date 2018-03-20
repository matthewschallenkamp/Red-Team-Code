#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int Max = 303;
int memo[Max][Max*Max+3];

struct piece {
    int size, opens, closes;
    piece() {}
    piece(int a, int b, int c) : size(a), opens(b), closes(c) {}
};

bool cmp(piece x, piece y) {
    int diffX = x.opens - x.closes;
    int diffY = y.opens - y.closes;
    if((diffX > 0) ^ (diffY > 0)) {
        return diffX > diffY;
    }
    if(diffX > 0) {
        if(x.closes == y.closes) return diffX > diffY;
        return x.closes < y.closes;
    }
    if(x.opens == y.opens) return diffX > diffY;
    return x.opens > y.opens;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<piece> arr(n);
    string s;
    for(int i = 0; i < n; ++i) {
        cin >> s;
        int opens = 0, closes = 0;
        for(char c : s) {
            if(c == '(') opens++;
            else if(opens > 0) opens--;
            else closes++;
        }
        arr[i] = piece(s.size(), opens, closes);
    }
    sort(arr.begin(), arr.end(), cmp);
    for(int pos = 0; pos <= n; ++pos) {
        for(int cnt = 0; cnt <= Max*Max; ++cnt) {
            memo[pos][cnt] = -1e7;
        }
    }
    memo[0][0] = 0;
    for(int pos = 1; pos <= n; ++pos) {
        int currSize = arr[pos-1].size;
        int currLow = arr[pos-1].closes;
        int currDiff = arr[pos-1].opens - arr[pos-1].closes;
        for(int cnt = 0; cnt <= Max*Max; ++cnt) {
            memo[pos][cnt] = max(memo[pos][cnt], memo[pos-1][cnt]);
            int prevCnt = cnt - currDiff;
            if(prevCnt-currLow >= 0 && 0 <= prevCnt && prevCnt <= Max*Max) {
                memo[pos][cnt] = max(memo[pos][cnt], memo[pos-1][prevCnt] + currSize);
            }
        }
    }
    cout << memo[n][0] << '\n';
    return 0;
}



























