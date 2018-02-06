#include <bits/stdc++.h>
using namespace std;

vector<vector<int> > adj, adjInv;
vector<int> scc;
int sccID;
vector<bool> visited;

void dfs1(int curr, stack<int> &seen) {
    visited[curr] = true;
    for(int x : adj[curr]) {
        if(!visited[x]) {
            dfs1(x, seen);
        }
    }
    seen.push(curr);
}

void dfs2(int curr) {
    visited[curr] = true;
    scc[curr] = sccID;
    for(int x : adjInv[curr]) {
        if(!visited[x]) {
            dfs2(x);
        }
    }
}

void doTest() {
    int n,m;
    cin >> n >> m;
    scc.clear();
    scc.resize(n+1);
    adj.clear();
    adj.resize(n+1);
    adjInv.clear();
    adjInv.resize(n+1);
    int a,b;
    vector<pair<int, int> > edges;
    for(int i = 0; i < m; ++i) {
        cin >> a >> b;
        adj[a].push_back(b);
        adjInv[b].push_back(a);
        edges.push_back(make_pair(a,b));
    }
    visited.clear();
    visited.resize(n+1,false);
    stack<int> seen;
    for(int i = 1; i <= n; ++i) {
        if(!visited[i]) {
            dfs1(i, seen);
        }
    }
    visited.clear();
    visited.resize(n+1,false);
    sccID = 0;
    while(!seen.empty()) {
        while(!seen.empty() && visited[seen.top()]) seen.pop();
        if(!seen.empty()) {
            dfs2(seen.top());
            sccID++;
        }
    }
    //now sccID = number of SCC's
    vector<int> indegree(sccID, 0);
    for(auto &p : edges) {
        if(scc[p.first] != scc[p.second]) {
            indegree[scc[p.second]]++;
        }
    }
    int answer = 0;
    for(int i = 0; i < sccID; ++i) {
        if(indegree[i] == 0) answer++;
    }
    cout << answer << '\n';
}

int32_t main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int t;
    cin >> t;
    while(t--) doTest();
    return 0;
}














