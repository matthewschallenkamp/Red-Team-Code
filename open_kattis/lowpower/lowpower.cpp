#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<int> arr;

bool possible(int diff) {
    if(arr[1] - arr[0] > diff) return false;
    int cnt = 0;
    for(int i = 0; i < arr.size()-1; ++i) {
        if(i > cnt*k*2) {
            return false;
        }
        if(arr[i+1] - arr[i] <= diff) {
            cnt++;
            if(cnt == n) return true;
            i++;
        }
    }
    return false;
}

int main() {
    cin >> n >> k;
    arr.resize(2*n*k);
    for(int &x : arr) cin >> x;
    sort(arr.begin(), arr.end());
    int start = -1, end = (int)1e9, mid;
    while(end-start > 1) {
        mid = (start+end)>>1;
        if(possible(mid)) {
            end = mid;
        } else start = mid;
    }
    cout << end << '\n';
    return 0;
}
