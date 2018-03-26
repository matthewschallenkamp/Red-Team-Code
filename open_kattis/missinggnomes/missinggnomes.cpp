#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> arr(k);
    vector<bool> used(n+1,false);
    for(int &x : arr) {
        cin >> x;
        used[x] = true;
    }
    int pos = 1;
    for(int i = 0; i < k; ++i) {
        while(pos <= n && pos < arr[i]) {
            if(pos <= n && !used[pos]) cout << pos << '\n';
            pos++;
        }
        cout << arr[i] << '\n';
    }
    while(pos <= n) {
        if(pos <= n && !used[pos]) cout << pos << '\n';
        pos++;
    }
    return 0;
}

