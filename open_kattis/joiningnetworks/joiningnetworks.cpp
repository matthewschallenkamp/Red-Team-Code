#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("sse2")

// #include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <map>
#include <queue>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstring>
using namespace std;
#define ll long long
#define ld unsigned long long
#define pb push_back
#define mp make_pair
#define D(x) cout<<#x<<" -> "<<x<<'\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define uni(x) (x).erase(unique(all(x)), (x).end())
#define rep(i, n) for (int32_t i = 0; i < (int32_t)(n); ++i)
#define rep1(i, n) for (int32_t i = 1; i <= (int32_t)(n); ++i)
const ld pi = 4.0*atanl(1.0);
const ll infll = (ll)(1e18) + 10;
const ll mod = powl(10, 9) + 7;

int n,m;
set<int> adj[(int)1e5+1];
bool visited[(int)1e5+1];
int parent[(int)1e5+1];
int numNodes[(int)1e5+1], sizes[(int)1e5+1];
ld sum1[(int)1e5+1], sum2[(int)1e5+1], sum1Sqr[(int)1e5+1], sum2Sqr[(int)1e5+1], sum2SqrSave[(int)1e5+1];

void dfs(int node) {
    visited[node] = true;
    numNodes[node] = 1;
    sum1[node] = 0;
    sum1Sqr[node] = 0;
    for(int to : adj[node]) {
        if(!visited[to]) {
            dfs(to);
            numNodes[node] += numNodes[to];
            sum1[node] += sum1[to] + (ld)numNodes[to];
            sum1Sqr[node] += sum1Sqr[to] + (ld)2*sum1[to] + (ld)numNodes[to];
        }
    }
}

void bfs(int start, ld treeSize) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    sum2[start] = sum1[start];
    sum2Sqr[start] = sum1Sqr[start];
    while(!q.empty()) {
        int parent = q.front();
        q.pop();
        for(int node : adj[parent]) {
            if(!visited[node]) {
                q.push(node);
                visited[node] = true;
                sum2[node] = sum1[node];
                sum2[node] += sum2[parent]-sum1[node]-(ld)numNodes[node];
                sum2[node] += treeSize-(ld)numNodes[node];
                
                sum2Sqr[node] = sum2Sqr[parent];
                sum2Sqr[node] -= sum1Sqr[node] + (ld)2*sum1[node] + (ld)numNodes[node];
                sum2Sqr[node] += (ld)2*(sum2[parent]-sum1[node]-(ld)numNodes[node]) + treeSize-(ld)numNodes[node];
                sum2Sqr[node] += sum1Sqr[node];
            }
        }
    }
}

void dfs1(int node) {
    visited[node] = true;
    sizes[node] = 1;
    for(int to : adj[node]) {
        if(!visited[to]) {
            dfs1(to);
            sizes[node] += sizes[to];
        }
    }
}

int getCentroid1(int node, int tgtSize) {
    bool isCentroid = true;
    visited[node] = true;
    int greatestChild = -1;
    for(int to : adj[node]) {
        if(!visited[to]) {
            if(sizes[to] > tgtSize/2) isCentroid = false;
            if(greatestChild == -1 || sizes[to] > sizes[greatestChild]) {
                greatestChild = to;
            }
        }
    }
    if(isCentroid && tgtSize - sizes[node] <= tgtSize/2) return node;
    return getCentroid1(greatestChild, tgtSize);
}

int getCentroid(int start, int tgtSize) {
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    dfs1(start);
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    return getCentroid1(start, tgtSize);
}

ld getCost(int nodeA, int nodeB) {
    static map<pair<int, int>, ld> memo;
    pair<int, int> index = mp(min(nodeA, nodeB), max(nodeA, nodeB));
    if(memo.count(index)) return memo[index];
    adj[nodeA].insert(nodeB);
    adj[nodeB].insert(nodeA);
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    dfs(1);
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    bfs(1, n+m);
    ld minCost = 0, answer1 = -1;
    rep1(i,n+m) minCost += sum2Sqr[i];
    adj[nodeA].erase(nodeB);
    adj[nodeB].erase(nodeA);
    return memo[index] = minCost/2;
}

int ternarySearch(int centroid1, int centroid2) {
    vector<int> path;
    path.pb(centroid1);
    bool found = true;
    while(found) {
        found = false;
        for(int to : adj[centroid1]) {
            if(sum2SqrSave[to] < sum2SqrSave[centroid1]) {
                path.pb(to);
                centroid1 = to;
                found = true;
            }
        }
    }
    int start = 0, end = path.size()-1, mid1 = 0, mid2 = 0;
    int bestIndex1 = path[start];
    while(end-start > 2) {
        mid1 = (end-start)/3+start;
        mid2 = (end-start)/3+mid1;
        ld cost1 = getCost(path[mid1],centroid2);
        ld cost2 = getCost(path[mid2],centroid2);
        if(cost1 < cost2) {
            end = mid2;
            bestIndex1 = path[end];
        } else {
            start = mid1;
            bestIndex1 = path[start];
        }
    }
    ld bestCost = min(getCost(path[start],centroid2), getCost(path[end],centroid2));
    if(path.size() > 1) {
        ld cost = getCost(path[start+1],centroid2);
        if(cost < bestCost) bestIndex1 = path[start+1], bestCost = cost;
    }
    return bestIndex1;
}

int main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    cin >> n;
    int a,b;
    rep(i,n-1) {
        cin >> a >> b;
        adj[a].insert(b);
        adj[b].insert(a);
    }
    cin >> m;
    rep(i,m-1) {
        cin >> a >> b;
        a += n;
        b += n;
        adj[a].insert(b);
        adj[b].insert(a);
    }
    
    ld best = -1;
    
    
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    dfs(1);
    dfs(n+1);
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    bfs(1,n);
    bfs(n+1,m);
    
    rep1(i,n+m) sum2SqrSave[i] = sum2Sqr[i];

    vector<pair<pair<ld, ld>, int> > costn(n), costm(m);
    for(int i = 1; i <= n; i++)
    {
        costn[i - 1] = make_pair(make_pair(sum2[i], sum2Sqr[i]), i);
    }
    for(int i = n + 1; i <= n + m; i++)
    {
        costm[i - n - 1] = make_pair(make_pair(sum2[i], sum2Sqr[i]), i);
    }
    sort(costn.begin(), costn.end());
    sort(costm.begin(), costm.end());

    int oi = 0;
    ld min2 = costn[0].first.second;
    for(int i = 1; i < n; i++)
    {
        if(min2 > costn[i].first.second)
        {
            //add this item
            costn[++oi] = costn[i];
            min2 = costn[i].first.second;
        }
    }
    costn.resize(oi+1);
    cerr << oi << endl;
    oi = 0;
    min2 = costm[0].first.second;
    for(int i = 1; i < m; i++)
    {
        if(min2 > costm[i].first.second)
        {
            //add this item
            costm[++oi] = costm[i];
            min2 = costm[i].first.second;
        }
    }
    costm.resize(oi+1);
    cerr << oi << endl;

    int bestIndex1 = 1, bestIndex2 = n + 1;
    ld bestc = 1ULL << 63;
    for(int i = 0; i < costn.size(); i++)
    {
        for(int j = 0; j < costm.size(); j++)
        {
            //sum2, sum2sqr

            //total sum is 
            ld sum = costn[i].first.second * m + costm[j].first.second * n 
                + 2 * costn[i].first.first * costm[j].first.first
                + 2 * costn[i].first.first * m + 2 * costm[j].first.first * n
                + m * n;
            // cerr << i << " " << j << " " << sum << endl;
            if(sum <= bestc)
            {
                bestIndex1 = costn[i].second;
                bestIndex2 = costm[j].second;
                bestc = sum;
            }
        }
    }

    //dists are
    // (each t1 + each t2 + 1)^2
    // simple is t1^2 + t2^2 + 2*t1*t2
    // per t1 there are t2s, per t2 there are t1s, 
    // for 2*t1*t2 we can just multiply sums
    // with +1 we add 2*t1 + 2*t2 + 1, across 2*t1sum*m + 2*t2sum*n + m*n
    //for each t1 there are t2s, for each t2 there are t1s

    cerr << bestIndex1 << " " << bestIndex2 << endl;
    
    cout << setprecision(0) << fixed << getCost(bestIndex1, bestIndex2) << '\n';
    return 0;
}
