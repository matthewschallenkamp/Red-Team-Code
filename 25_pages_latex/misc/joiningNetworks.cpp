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
}   }   }

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
}   }   }   }