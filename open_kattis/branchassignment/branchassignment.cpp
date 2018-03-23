#include <bits/stdc++.h>
using namespace std;
typedef long long ll;


void dijkstras(int start, vector<vector<pair<int, ll> > > &adj, vector<ll> &dist) {
    dist[start] = 0;
    set<pair<int, ll> > q;
    q.insert({start, 0});
    while(!q.empty()) {
        int node = q.begin()->first;
        q.erase(q.begin());
        for(auto &to : adj[node]) {
            if(dist[to.first] > dist[node] + to.second) {
                dist[to.first] = dist[node] + to.second;
                q.insert({to.first, dist[to.first]});
            }
        }
    }
}

int main() {
    int n, b, s, r;
    cin >> n >> b >> s >> r;
    vector<vector<pair<int, ll> > > adj(n+1), adjInv(n+1);
    vector<ll> toHead(n+1, 1e17), fromHead(n+1, 1e17);
    int u, v, w;
    for(int i = 0; i < r; ++i) {
        cin >> u >> v >> w;
        adj[u].push_back({v,w});
        adjInv[v].push_back({u,w});
    }
    dijkstras(b+1, adj, fromHead);
    dijkstras(b+1, adjInv, toHead);
    vector<int> sum(b+1);
    for(int i = 1; i <= b; ++i) sum[i] = toHead[i]+fromHead[i];
    sort(sum.begin()+1, sum.end());
    vector<vector<ll> > memo(b+3, vector<ll>(s+3,1e17));
    memo[0][0] = 0;
    for(int pos = 1; pos <= b; ++pos) {
        for(int groups = 1; groups <= s; ++groups) {
            ll currSum = 0;
            ll size = 0;
            for(int i = pos; i >= 1; --i) {
                currSum += sum[i];
                size++;
                memo[pos][groups] = min(memo[pos][groups], memo[i-1][groups-1] + (size-1)*currSum);
                if(size*groups > pos) break;
            }
        }
    }
    cout << memo[b][s] << '\n';
    return 0;
}


































