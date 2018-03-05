#include <bits/stdc++.h>
using namespace std;

vector<int> LIS(vector<int> &arr) {
    vector<int> longest(arr.size(),0);
    multiset<int> seq;
    for(int i = 0; i < arr.size(); ++i) {
        seq.insert(arr[i]);
        auto it = seq.upper_bound(arr[i]);
        if(it != seq.end()) seq.erase(it);
        longest[i] = seq.size();
    }
    return longest;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n,0);
    for(int i = 0; i < n; ++i) cin >> arr[i];
    cout << "array:\n";
    for(int i = 0; i < n; ++i) cout << arr[i] << ' ';
    cout << '\n';
    vector<int> lis = LIS(arr);
    cout << "Longest Increasing Subsequence for each prefix:\n";
    for(int i = 0; i < n; ++i) cout << lis[i] << ' ';
    cout << '\n';
    return 0;
}

























