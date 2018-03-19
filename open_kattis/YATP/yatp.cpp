#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int Max = 2e5+2;
ll value[Max], Distance[Max], answer[Max], M[Max], B[Max];
int depth[Max], memo[Max][19], parent[Max], sizes[Max], len, ptr;
vector<pair<int, ll> > adj[Max], lines[Max];
bool removed[Max];

void dfs(int node, int par) {
    for(auto to : adj[node]) {
        if(to.first != par) {
            memo[to.first][0] = node;
            depth[to.first] = 1 + depth[node];
            Distance[to.first] = to.second + Distance[node];
            dfs(to.first,node);
        }
    }
}

int LCA(int x, int y) {
    if(x == y) return x;
    if(depth[x] < depth[y]) swap(x,y);
    int diff = depth[x] - depth[y];
    for(int k = 0; k < 19; ++k) {
        if(diff&(1<<k)) {
            x = memo[x][k];
        }
    }
    if(x == y) return x;
    for(int k = 18; k >= 0; --k) {
        if(memo[x][k] != memo[y][k]) {
            x = memo[x][k];
            y = memo[y][k];
        }
    }
    return memo[x][0];
}

ll dist(int x, int y) {
    return Distance[x] + Distance[y] - 2LL*Distance[LCA(x,y)];
}

//calc subtree sizes
void dfs2(int node, int par) {
    sizes[node] = 1;
    for(auto to : adj[node]) {
        if(to.first != par && !removed[to.first]) {
            dfs2(to.first, node);
            sizes[node] += sizes[to.first];
        }
    }
}

int getCentroid(int node) {
    dfs2(node,node);
    int tgtSize = sizes[node]/2;
    bool found = true;
    int par = node;
    while(found) {
        found = false;
        for(auto to : adj[node]) {
            if(to.first != par && !removed[to.first] && sizes[to.first] > tgtSize) {
                found = true;
                par = node;
                node = to.first;
                break;
            }
        }
    }
    return node;
}

//this calculates the Centroid Decomposition tree
void dfs1(int node) {
    removed[node] = true;
    for(auto to : adj[node]) {
        if(!removed[to.first]) {
            int child = getCentroid(to.first);
            parent[child] = node;
            dfs1(child);
        }
    }
}

void addLine(ll m, ll b) {
    while(len >= 2 && (B[len-2]-B[len-1])*(m-M[len-1]) >= (B[len-1]-b)*(M[len-1]-M[len-2])) {
        --len;
    }
    M[len] = m;
    B[len] = b;
    ++len;    
}
  
ll minValue(ll x) {
    ptr = min(ptr, len-1);
    while(ptr + 1 < len && M[ptr+1]*x + B[ptr+1] <= M[ptr]*x + B[ptr]) {
        ++ptr;
    }
    return M[ptr]*x + B[ptr];
}  

bool cmpSlope(pair<int, ll> x, pair<int, ll> y) {
    if(value[x.first] == value[y.first]) return x.second < y.second;//increasing y-intercepts
    return value[x.first] > value[y.first];//decreasing slopes
}

//Code modified from:
//http://serjudging.vanb.org/wp-content/uploads/YetAnotherTreeProblem_min.java
//https://sites.google.com/site/indy256/algo/convex_hull_optimization
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for(int i = 1; i <= n; ++i) cin >> value[i];
    int a,b,w;
    for(int i = 0; i < n-1; ++i) {
        cin >> a >> b >> w;
        adj[a].push_back({b,w});
        adj[b].push_back({a,w});
    }
    dfs(1,0);
    for(int k = 1; k < 19; ++k) {
        for(int i = 1; i <= n; ++i) {
            memo[i][k] = memo[memo[i][k-1]][k-1];
        }
    }
    int centroid = getCentroid(1);
    dfs1(centroid);
    for(int i = 1; i <= n; ++i) answer[i] = value[i]*value[i];
    for(int i = 1; i <= n; ++i) {
        for(int curr = parent[i]; curr; curr = parent[curr]) {
            ll currDist = dist(curr, i);
            answer[i] = min(answer[i], currDist + value[i]*value[curr]);
            answer[curr] = min(answer[curr], currDist + value[i]*value[curr]);
            lines[curr].push_back({i, currDist});
        }
    }
    for(int i = 1; i <= n; ++i) {
        if(lines[i].size()) {
            sort(lines[i].begin(), lines[i].end(), cmpSlope);
            len = ptr = 0;
            ll prevM = -1, prevB = -1;
            for(auto &line : lines[i]) {
                ll slope = value[line.first];
                if(slope == prevM && line.second > prevB) continue;
                prevM = slope;
                prevB = line.second;
                addLine(slope, line.second);
            }
            reverse(lines[i].begin(), lines[i].end());
            for(auto &line : lines[i]) {
                answer[line.first] = min(answer[line.first], minValue(value[line.first]) + line.second);
            }
        }
    }
    ll sum = 0;
    for(int i = 1; i <= n; ++i) sum += answer[i];
    cout << sum << '\n';
    return 0;
}



























