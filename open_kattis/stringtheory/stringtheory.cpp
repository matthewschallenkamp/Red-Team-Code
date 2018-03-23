#include <bits/stdc++.h>
using namespace std;

const int Max = 102;
int arr[Max];
int memo[Max][Max][Max];

bool valid(int i, int j, int k) {
    if(i > j) return false;
    if(k == 1) return (arr[i] > 0);
    if(arr[i] < k || arr[j] < k) {
        return false;
    }
    bool result;
    arr[i] -= k;
    arr[j] -= k;
    if(arr[i] == 0 && arr[j] == 0) {
        result = valid(i+1, j-1, k-1);
    } else if(arr[i] == 0) {
        result = valid(i+1, j, k-1);
    } else if(arr[j] == 0) {
        result = valid(i, j-1, k-1);
    } else {
        result = valid(i, j, k-1);
    } 
    arr[i] += k;
    arr[j] += k;
    return result;
}

int main() {
    for(int i = 1; i <= 100; ++i) {
        for(int j = 1; j <= 100; ++j) {
            for(int k = 1; k <= 100; ++k) {
                memo[i][j][k] = -1;
            }
        }
    }
    int n, sum = 0;
    cin >> n;
    for(int i = 1; i <= n; ++i) {
        cin >> arr[i];
        sum += arr[i];
    }
    if(sum%2==1) {
        cout << "no quotation\n";
        return 0;
    }
    for(int k = min(arr[1], arr[n]); k >= 2; --k) {
        if(valid(1,n,k)) {
            cout << k << '\n';
            return 0;
        }
    }
    if(n==2 && arr[1] == 1 && arr[2] == 1) {
        cout << "1\n";
        return 0;
    }
    if(n == 1 && arr[1] == 2) {
        cout << "1\n";
        return 0;
    }
    cout << "no quotation\n";
    return 0;
}




























