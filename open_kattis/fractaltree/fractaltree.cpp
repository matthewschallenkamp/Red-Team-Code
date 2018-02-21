#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n, k, currLabel;
vector<vector<int> > adj, memo, relabelled;
vector<int> depth, parent, numNodes, leaves, label;

void dfs(int node) {
    label[node] = currLabel;
    currLabel++;
    for(int to : adj[node]) {
        dfs(to);
    }
}

vector<int> getNodes(int node) {
    vector<int> arr;
    int iteration = k-1;
    while(iteration >= 0) {
        int subtreeSize = numNodes[iteration];
        iteration--;
        int start = 0, end = leaves.size(), mid = 0;
        bool found = false;
        while(end-start>1) {
            mid = (start+end)>>1;
            ll label = (ll)leaves[mid] + 1LL*mid*(subtreeSize-1);
            if(label <= node && node < label + subtreeSize) {
                arr.push_back(leaves[mid]);
                if(node == label) return arr;
                node -= label;
                node++;
                found = true;
                break;
            }
            if(node < label) {
                end = mid;
            } else start = mid;
        }
        if(!found) {
            ll label = (ll)leaves[start] + 1LL*start*(subtreeSize-1);
            if(label <= node && node < label + subtreeSize) {
                arr.push_back(leaves[start]);
                if(node == label) return arr;
                node -= label;
                node++;
            } else if(node < leaves[start]) {
                arr.push_back(node);
                return arr;
            } else {
                ll label = 1LL*(start+1)*(subtreeSize-1);
                arr.push_back(node - label);
                return arr;
            }
        }
    }
    arr.push_back(node);
    return arr;
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);//fast IO
    cin >> n;
    
    adj.resize(n+1);//one based labeling for nodes.
    depth.resize(n+1);
    parent.resize(n+1);
    label.resize(n+1);
    int par;
    for(int i = 2; i <= n; ++i) {
        cin >> par;
        par++;
        adj[par].push_back(i);
    }
    currLabel = 1;
    dfs(1);
    relabelled.resize(n+1);
    parent[1] = depth[1] = 0;
    for(int i = 1; i <= n; ++i) {
        for(int to : adj[i]) {
            relabelled[label[i]].push_back(label[to]);
            parent[label[to]] = label[i];
            depth[label[to]] = depth[label[i]] + 1;
        }
    }
    adj.swap(relabelled);
    relabelled.clear();
    for(int i = 1; i <= n; ++i) {
        if(adj[i].size() == 0) leaves.push_back(i);
    }
    int q, a, b;
    cin >> k >> q;
    if(leaves.size()==1) {//special case linked lists
        while(q--) {
            cin >> a >> b;
            cout << abs(a-b) << '\n';
        }
        return 0;
    }
    int curr = 1;
    int pathLength = 0;
    while(adj[curr].size() > 0) {
        curr = adj[curr][0];
        pathLength++;
    }
    //calculate binary-lifting table
    memo.resize(n+1, vector<int>(18,0));
    for(int i = 1; i <= n; ++i) memo[i][0] = parent[i];
    for(int k = 1; k <= 17; ++k) {//17 = ceil(log2(10^5))
        for(int i = 1; i <= n; ++i) {
            memo[i][k] = memo[memo[i][k-1]][k-1];
        }
    }
    //calculate number of nodes for each iteration of the tree
    numNodes.push_back(n);
    for(int i = 1; i <= min(k,30); ++i) {
        if(log(n-1) + i*log(leaves.size()) <= 50*log(2)) {
            ll temp = (ll)numNodes[i-1] + 1LL*(n-1)*powl(leaves.size(),i);
            if(temp <= (1LL<<30)) {
                numNodes.push_back(temp);
            } else {
                numNodes.push_back(1LL<<30);
            }
        } else {
            numNodes.push_back(1LL<<30);
        }
    }
    ll root = (1LL*max(0,k-((int)numNodes.size()-1)))*pathLength+1;
    k = min(k, 30);
    while(q--) {
        cin >> a >> b;
        if(a==b) {
            cout << "0\n";
            continue;
        }
        if(a < root && b < root) {
            cout << abs(a-b) << '\n';
            continue;
        }
        int distance = 0;
        if(a < root) {
            distance = root-a;
            a = 1;
        } else {
            a -= root;
            a++;
        }
        if(b < root) {
            distance = root-b;
            b = 1;
        } else {
            b -= root;
            b++;
        }
        vector<int> nodesA = getNodes(a);
        vector<int> nodesB = getNodes(b);
        int i, n = min(nodesA.size(), nodesB.size());
        for(i = 0; i < n; ++i) {
            if(nodesA[i] != nodesB[i]) break;
        }
        if(i < n) {
            int lca = LCA(nodesA[i], nodesB[i]);
            distance += depth[nodesA[i]]-depth[lca] + depth[nodesB[i]]-depth[lca];
            i++;
        }
        n = max(nodesA.size(), nodesB.size());
        for(; i < n; ++i) {
            if(i < nodesA.size()) {
                distance += depth[nodesA[i]];
            }
            if(i < nodesB.size()) {
                distance += depth[nodesB[i]];
            }
        }
        cout << distance << '\n';
    }
    return 0;
}

































