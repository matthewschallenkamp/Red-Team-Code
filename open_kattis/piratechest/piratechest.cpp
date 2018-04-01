#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

const int Max = 505;
int depth[Max][Max], minVal[Max][Max][Max], maxDepth[Max][Max];
int arr[Max], rights[Max], lefts[Max];

int main() {
    int a, b, n, m;
    cin >> a >> b >> n >> m;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            cin >> depth[i][j];
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            int least = depth[i][j];
            int len = 1;
            for(int k = i; k < n; ++k) {
                least = min(least, depth[k][j]);
                minVal[k][j][len] = least;
                len++;
            }
        }
    }
    //calculate maxDepth for each chest
    for(int i = 0; i < n; ++i) {
        for(int len = 1; len <= i+1; ++len) {
            for(int j = 0; j < m; ++j) {
                arr[j] = minVal[i][j][len];
            }
            stack<int> positions;
            for(int j = 0; j < m; ++j) {
                while(!positions.empty() && arr[positions.top()] >= arr[j]) positions.pop();
                if(positions.empty()) {
                    lefts[j] = -1;
                } else {
                    lefts[j] = positions.top();
                }
                positions.push(j);
            }
            while(!positions.empty()) positions.pop();
            for(int j = m-1; j >= 0; --j) {
                while(!positions.empty() && arr[positions.top()] >= arr[j]) positions.pop();
                if(positions.empty()) {
                    rights[j] = m;
                } else {
                    rights[j] = positions.top();
                }
                positions.push(j);
            }
            for(int j = 0; j < m; ++j) {
                int width = rights[j] - lefts[j] - 1;
                maxDepth[len][width] = max(maxDepth[len][width], arr[j]);
            }
        }
    }
    for(int i = 501; i >= 1; --i) {
        for(int j = 501-1; j >= 1; --j) {
            maxDepth[i][j] = max(maxDepth[i][j], maxDepth[i][j+1]);
        }
    }
    for(int j = 501; j >= 1; --j) {
        for(int i = 501-1; i >= 1; --i) {
            maxDepth[i][j] = max(maxDepth[i][j], maxDepth[i+1][j]);
        }
    }
    ll volume = 0;
    for(int i = 1; i <= a; ++i) {
        for(int j = 1; j <= b; ++j) {
            ll currDepth = max(maxDepth[i][j], maxDepth[j][i]);
            ll num = currDepth*n*m;
            ll dem = n*m-i*j;
            ll height = num/dem + (num%dem != 0) - 1;
            volume = max(volume, 1LL*i*j*height);
        }
    }
    cout << volume << '\n';
    return 0;
}























