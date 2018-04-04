#include <bits/stdc++.h>
using namespace std;

struct BipartiteMatcher {
    vector<vector<int>> G;
    vector<int> L, R, Viz;

    BipartiteMatcher(int n, int m) :
        G(n), L(n, -1), R(m, -1), Viz(n) {}

    void AddEdge(int a, int b) {
        G[a].push_back(b);
    }

    bool Match(int node) {
        if(Viz[node]) 
            return false;
        Viz[node] = true;
    
        for(auto vec : G[node]) {
            if(R[vec] == -1 || Match(R[vec])) {
                L[node] = vec;
                R[vec] = node;
                return true;
            }
        }

        return false;
    }
    int Solve() {
        bool ok = true;
        while(ok) {
            ok = false;
            fill(Viz.begin(), Viz.end(), 0);
            for(int i = 0; i < L.size(); ++i)
                if(L[i] == -1)
                    ok |= Match(i);
        }
        
        int ret = 0;
        for(int i = 0; i < L.size(); ++i)
            ret += (L[i] != -1);
        return ret;
    }
};

void dfs(int node, vector<bool> &visitedA, vector<bool> &visitedB, BipartiteMatcher &bm) {
    visitedA[node] = true;
    for(auto to : bm.G[node]) {
        if(!visitedB[to] && bm.R[to] != -1) {
            visitedB[to] = true;
            dfs(bm.R[to], visitedA, visitedB, bm);
        }
    }
}

const int Max = 1e5+3;
vector<int> adj[Max];
bool visited[Max], parody[Max];
 
void dfs(int node, bool type = true) {
    parody[node] = type;
    visited[node] = true;
    for(int to : adj[node]) {
        if(!visited[to]) {
            dfs(to, !type);
        }
    }
}

vector<int> getMinimumVertexCover(vector<int> &A, vector<int> &B, BipartiteMatcher &bm) {
    bm.Solve();
    vector<bool> visitedA(A.size(), false);
    vector<bool> visitedB(B.size(), false);
    vector<int> minimumVertexCover;
    for(int k = 0; k < A.size(); ++k) {
        if(bm.L[k] == -1) {
            dfs(k, visitedA, visitedB, bm);
        }
    }
    for(int i = 0; i < A.size(); ++i) {
        if(!visitedA[i]) minimumVertexCover.push_back(A[i]);
    }
    for(int i = 0; i < B.size(); ++i) {
        if(visitedB[i]) minimumVertexCover.push_back(B[i]);
    }
    return minimumVertexCover;
}

int main() {
    //build bipartite graph
    //example usage for building a tree (which is always bipartite)
    int n;
    cin >> n;
    int u,v;
    vector<pair<int, int> > edges(n-1);
    for(int i = 0; i < n-1; ++i) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges[i] = make_pair(u,v);
    }
    dfs(1);
    vector<int> A, B;
    map<int, int> tableA, tableB;
    for(int i = 1; i <= n; ++i) {
        if(parody[i]) {
            tableA[i] = A.size();
            A.push_back(i);
        } else {
            tableB[i] = B.size();
            B.push_back(i);
        }
    }
    BipartiteMatcher bm(A.size(), B.size());
    for(auto &e : edges) {
        if(parody[e.first]) {
            bm.AddEdge(tableA[e.first], tableB[e.second]);
        } else {
            bm.AddEdge(tableA[e.second], tableB[e.first]);
        }
    }
    vector<int> vertexCover = getMinimumVertexCover(A, B, bm);
    //now, every vertex not in the minimum vertex cover is in the maximum independent set
    cout << vertexCover.size() << '\n';
    for(int ver : vertexCover) cout << ver << ' ';cout << '\n';
    return 0;
}
 
 
 
 





