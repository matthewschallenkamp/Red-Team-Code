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

vector<pair<int, int> > arr;

int distSqr(int i, int j) {
    int dist = (arr[i].first - arr[j].first)*(arr[i].first - arr[j].first);
    dist += (arr[i].second - arr[j].second)*(arr[i].second - arr[j].second);
    return dist;
}

void dfs(int node, vector<bool> &visitedA, vector<bool> &visitedB, BipartiteMatcher &bm) {
    visitedA[node] = true;
    for(auto to : bm.G[node]) {
        if(!visitedB[to] && bm.R[to] != -1) {
            visitedB[to] = true;
            dfs(bm.R[to], visitedA, visitedB, bm);
        }
    }
}

int main() {
    int n, tgtDist;
    cin >> n >> tgtDist;
    arr.resize(n);
    for(auto &p : arr) cin >> p.first >> p.second;
    vector<int> answer;
    answer.push_back(0);
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            int currDist = distSqr(i,j);
            if(currDist > tgtDist*tgtDist) continue;
            for(int k = 0; k < n; ++k) {
                if(k == i || k == j) continue;
                if(distSqr(i,k) > currDist) continue;
                if(distSqr(j,k) > currDist) continue;
            }
            vector<int> A, B;
            for(int k = 0; k < n; ++k) {
                if(k == i || k == j) continue;
                if(distSqr(i,k) > currDist) continue;
                if(distSqr(j,k) > currDist) continue;
                pair<int, int> U = make_pair(arr[k].first-arr[i].first, arr[k].second-arr[i].second);
                pair<int, int> V = make_pair(arr[j].first-arr[i].first, arr[j].second-arr[i].second);
                if(U.first*V.second - U.second*V.first > 0) {
                    A.push_back(k);
                } else {
                    B.push_back(k);
                }
            }
            BipartiteMatcher bm(A.size(), B.size());
            for(int k = 0; k < A.size(); ++k) {
                for(int l = 0; l < B.size(); ++l) {
                    if(distSqr(A[k],B[l]) > tgtDist*tgtDist) {
                        bm.AddEdge(k,l);
                    }
                }
            }
            bm.Solve();
            vector<bool> visitedA(A.size(), false);
            vector<bool> visitedB(B.size(), false);
            for(int k = 0; k < A.size(); ++k) {
                if(bm.L[k] == -1) {
                    dfs(k, visitedA, visitedB, bm);
                }
            }
            vector<int> indexes;
            indexes.push_back(i);
            indexes.push_back(j);
            for(int k = 0; k < A.size(); ++k) {
                if(visitedA[k]) indexes.push_back(A[k]);
            }
            for(int k = 0; k < B.size(); ++k) {
                if(!visitedB[k]) indexes.push_back(B[k]);
            }
            if(indexes.size() > answer.size()) {
                answer = indexes;
            }
        }
    }
    cout << answer.size() << '\n';
    for(int x : answer) cout << x+1 << ' ';
    cout << '\n';
    return 0;
}















