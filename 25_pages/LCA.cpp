#include <bits/stdc++.h>
using namespace std;

vector<vector<int> > adj, memo;
vector<int> depth, parent;
vector<bool> visited;

void dfs(int node, int Depth = 0) {
    depth[node] = Depth;
    visited[node] = true;
    for(int x : adj[node]) {
        if(!visited[x]) {
            parent[x] = node;
            dfs(x, Depth+1);
        }
    }
}

int LCA(int x, int y) {//O(log(n))
    if(depth[x] < depth[y]) swap(x,y);
    int diff = depth[x] - depth[y];
    for(int k = 17; k >= 0; --k) {
        if(diff&(1<<k)) {
            x = memo[x][k];
        }
    }
    for(int k = 17; k >= 0; --k) {
        if(memo[x][k] != memo[y][k]) {
            x = memo[x][k];
            y = memo[y][k];
        }
    }
    if(x != y) x = parent[x];
    return x;
}

int main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int n;
    cin >> n;
    adj.resize(n+1);//one based labeling for nodes.
    depth.resize(n+1);
    parent.resize(n+1);
    visited.resize(n+1,false);
    int a,b;
    for(int i = 0; i < n-1; ++i) {
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    parent[1] = 0;
    dfs(1);
    memo.resize(n+1, vector<int>(18,0));
    for(int i = 1; i <= n; ++i) memo[i][0] = parent[i];
    for(int k = 1; k <= 17; ++k) {//17 = ceil(log2(10^5))
        for(int i = 1; i <= n; ++i) {
            memo[i][k] = memo[memo[i][k-1]][k-1];
        }
    }
    //usage:
    for(int i = 1; i <= n; ++i) {
        for(int j = i+1; j <= n; ++j) {
            cout << "for nodes: " << i << ' ' << j << ":\t";
            int temp = LCA(i, j);
            cout << "LCA: " << temp << '\t';
            cout << "path length: " << depth[i]-depth[temp] + depth[j] -depth[temp] << '\n';
        }
    }
    return 0;
}
















