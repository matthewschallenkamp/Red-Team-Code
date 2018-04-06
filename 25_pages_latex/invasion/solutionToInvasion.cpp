#include <bits/stdc++.h>
using namespace std;

int numSafe;
vector<int> minDist;
vector<vector<pair<int, int> > > adj;

void dfs(int node, int dist, int &k) {
    if(dist >= minDist[node]) return;
    if(minDist[node] == k) numSafe--;
    minDist[node] = dist;
    for(auto to : adj[node]) {
        dfs(to.first, dist + to.second, k);
}   }

//This is the last thing which should be added to the 25 pages, I'm only
//considering adding it because the solution wasn't like anyting I've seen before
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, a, k, u, v, w;
    while(cin >> n >> m >> a >> k) {
        adj.clear();
        adj.resize(n);
        minDist.clear();
        minDist.resize(n,k);
        numSafe = n;
        for(int i = 0; i < m; ++i) {
            cin >> u >> v >> w;
            u--,v--;
            adj[u].push_back({v,w});
            adj[v].push_back({u,w});
        }
        for(int i = 0; i < a; ++i) {
            cin >> v;
            if(numSafe > 0) {
                dfs(v-1, 0, k);
            }
        cout << numSafe << '\n';    }
    cout << '\n';   }
return 0;   }


