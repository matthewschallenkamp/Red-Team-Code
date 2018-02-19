#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <queue>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;


int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int> > adjMatrix, dist;
    adjMatrix.resize(n, vector<int>(n));
    dist.resize(n,vector<int>(n,1e9));//initialize distances to infinity
    int u, v, w;
    for(int i = 0; i < m; ++i) {
        cin >> u >> v >> w;//edge from node u to node v having weight w
        adjMatrix[u][v] = w;
        dist[u][v] = w;
    }
    for(int i = 0; i < n; ++i) dist[i][i] = 0;
    for(int k = 0; k < n; ++k) {
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(dist[i][j] == 1e9) {
                cout << "path from node " << i << " to node " << j << " does not exist.\n";
            } else {
                cout << "min distance from node " << i << " to node " << j << " is " << dist[i][j] << '\n';
            }
        }
    }
    return 0;
}




