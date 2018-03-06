#include <bits/stdc++.h>
using namespace std;

double table[51][51];
double choose(int n, int k) {
    if(k < 0 || k > n || n < 0) return 0;
    if(k == 0 || k == n) return 1;
    if(table[n][k] != -1) return table[n][k];
    return table[n][k] = choose(n-1,k) + choose(n-1, k-1);
}

int n, d;

double prob(int sharedCnt, int sizeA, int sizeB, int s, int a, int b) {
    double probability = choose(sharedCnt, s)*choose(sizeA,a)*choose(sizeB,b);
    probability *= choose(n-sharedCnt-sizeA-sizeB, d-s-a-b);
    return probability / choose(n,d);
}

double memo[11][11][11];

double expectedTurns(int s, int a, int b) {
    if(s == 0 && (a == 0 || b == 0)) return 0;
    double &sum = memo[s][a][b];
    if(sum >= 0) return sum;
    sum = 1;
    for(int i = 0; i <= d && i <= s; ++i) {
        for(int j = 0; i+j <= d && j <= a; ++j) {
            for(int k = 0; i+j+k <= d && k <= b; ++k) {
                if(i==0 && j==0 && k==0) continue;
                sum += prob(s,a,b,i,j,k)*expectedTurns(s-i,a-j,b-k);
            }
        }
    }
    sum *= 1/(double)(1-prob(s,a,b,0,0,0));
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    for(int i = 0; i <= 50; ++i) {
        for(int j = 0; j <= 50; ++j) {
            table[i][j] = -1;
        }
    }
    int size;
    cin >> n >> d >> size;
    for(int i = 0; i <= 10; ++i) {
        for(int j = 0; j <= 10; ++j) {
            for(int k = 0; k <= 10; ++k) {
                memo[i][j][k] = -1;
            }
        }
    }
    vector<int> A(size), B(size);
    for(int i = 0; i < size; ++i) cin >> A[i];
    for(int i = 0; i < size; ++i) cin >> B[i];
    sort(A.begin(), A.end());
    sort(B.begin(), B.end());
    int i = 0, j = 0;
    int sharedCnt = 0;
    while(i < size && j < size) {
        if(A[i] == B[j]) {
            sharedCnt++;
            i++, j++;
        } else if(A[i] < B[j]) {
            i++;
        } else {
            j++;
        }
    }
    cout << setprecision(6) << fixed << expectedTurns(sharedCnt, size - sharedCnt, size - sharedCnt) << '\n';
    return 0;
}

























