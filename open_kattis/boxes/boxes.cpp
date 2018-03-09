#include <bits/stdc++.h>
using namespace std;

vector<vector<int> > memo;
vector<vector<int> > adj;
vector<int> parent, depth, cnt;

void dfs(int node) {
    for(int x : adj[node]) {
        depth[x] = depth[node]+1;
        dfs(x);
        cnt[node] += cnt[x];
    }
}

int jump(int node, int amount) {
    for(int i = memo[0].size()-1; i >= 0; --i) {
        if(amount&(1<<i)) {
            node = memo[node][i];
        }
    }
    return node;
}

int main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int n;
    cin >> n;
    adj.resize(n+1);
    cnt.resize(n+1,1);
    parent.resize(n+1);
    depth.resize(n+1);
    int par;
    depth[0] = 0;
    for(int i = 1; i <= n; ++i) {
        cin >> par;
        adj[par].push_back(i);
        parent[i] = par;
    }
    dfs(0);
    int upper = floor(log2(n))+1;
    memo.resize(n+1, vector<int>(upper+1));
    for(int i = 1; i <= n; ++i) memo[i][0] = parent[i];
    for(int k = 1; k <= upper; ++k) {
        for(int i = 1; i <= n; ++i) {
            memo[i][k] = memo[memo[i][k-1]][k-1];
        }
    }
    int q,m;
    cin >> q;
    while(q--) {
        cin >> m;
        vector<int> query(m);
        vector<bool> valid(m,true);
        for(int &x : query) cin >> x;
        for(int i = 0; i < m; ++i) {
            for(int j = i+1; j < m; ++j) {
                if(depth[query[i]] > depth[query[j]]) {
                    if(jump(query[i],depth[query[i]]-depth[query[j]]) == query[j]) {
                        valid[i] = false;
                    }
                } else {
                    if(jump(query[j],depth[query[j]]-depth[query[i]]) == query[i]) {
                        valid[j] = false;
                    }
                }
            }
        }
        int answer = 0;
        for(int i = 0; i < m; ++i) {
            if(valid[i]) {
                answer += cnt[query[i]];
            }
        }
        cout << answer << '\n';
    }
    return 0;
}





























