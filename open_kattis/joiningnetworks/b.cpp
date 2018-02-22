#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n,m;
const int Max = (int)1e5+1;
vector<int> adj[Max];
bool visited[Max];
int parent[Max];
int numNodes[Max], sizes[Max];
ll sum1[Max], sum2[Max], sum1Sqr[Max], sum2Sqr[Max];
ll sumA[Max], sumASqr[Max], sumB[Max], sumBSqr[Max];

void dfs(int node) {
    visited[node] = true;
    numNodes[node] = 1;
    sum1[node] = 0;
    sum1Sqr[node] = 0;
    for(int to : adj[node]) {
        if(!visited[to]) {
            dfs(to);
            numNodes[node] += numNodes[to];
            sum1[node] += sum1[to] + (ll)numNodes[to];
            sum1Sqr[node] += sum1Sqr[to] + (ll)2*sum1[to] + (ll)numNodes[to];
        }
    }
}

void bfs(int start, ll treeSize) {
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
                sum2[node] += sum2[parent]-sum1[node]-(ll)numNodes[node];
                sum2[node] += treeSize-(ll)numNodes[node];
                
                sum2Sqr[node] = sum2Sqr[parent];
                sum2Sqr[node] -= sum1Sqr[node] + (ll)2*sum1[node] + (ll)numNodes[node];
                sum2Sqr[node] += (ll)2*(sum2[parent]-sum1[node]-(ll)numNodes[node]) + treeSize-(ll)numNodes[node];
                sum2Sqr[node] += sum1Sqr[node];
            }
        }
    }
}

ll costEdge(int i, int j) {
    return 1LL*m*sumASqr[i] + 1LL*n*sumBSqr[j] + 2LL*sumA[i]*sumB[j] + 2LL*m*sumA[i] + 2LL*n*sumB[j] + 1LL*n*m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cin >> n;
    int a,b;
    for(int i = 0; i < n-1; ++i) {
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    cin >> m;
    for(int i = 0; i < m-1; ++i) {
        cin >> a >> b;
        a += n;
        b += n;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    dfs(1);
    dfs(n+1);
    memset(visited,false,sizeof(bool)*(n+m+1)); 
    bfs(1,n);
    bfs(n+1,m);
    ll costA = 0, costB = 0;
    int i = 1, j = n+1;
    ll lowest = 1e18;
    for(int k = 1; k <= n; ++k) {
        costA += sum2Sqr[k];
        if(sum2Sqr[k] < lowest) {
            lowest = sum2Sqr[k];
            i = k;
        }
        sumA[k] = sum2[k];
        sumASqr[k] = sum2Sqr[k];
    }
    costA /= 2;
    lowest = 1e18;
    for(int k = n+1; k <= n+m; ++k) {
        costB += sum2Sqr[k];
        if(sum2Sqr[k] < lowest) {
            lowest = sum2Sqr[k];
            j = k;
        }
        sumB[k] = sum2[k];
        sumBSqr[k] = sum2Sqr[k];
    }
    costB /= 2;
    ll minCost = costEdge(i,j);
    bool improved = true;
    while(improved) {
        improved = false;
        for(int toI : adj[i]) {
            ll temp = costEdge(toI, j);
            if(temp < minCost) {
                minCost = temp;
                improved = true;
                i = toI;
                break;
            }
        }
        for(int toJ : adj[j]) {
            ll temp = costEdge(i, toJ);
            if(temp < minCost) {
                minCost = temp;
                improved = true;
                j = toJ;
                break;
            }
        }
    }
    cout << minCost + costA + costB << '\n';
    return 0;
}














