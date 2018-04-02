#include <bits/stdc++.h>
using namespace std;
typedef long long ll;


struct edge {
    int i, j, weight;
    edge(){}
    edge(int a, int b, int c) : i(a), j(b), weight(c) {}
};

const int Max = 405;
int n, sccID;
vector<int> adj[Max], adjInv[Max], mst[Max], scc;
bool visited[Max], marked[Max];
int Dist[Max][Max], status[Max];
vector<edge> edges;

vector<int> p(1001,-1);//change size here if needed
int find(int x) {return p[x] < 0 ? x : p[x] = find(p[x]);}
void merge(int x, int y) {
    if((x=find(x)) == (y=find(y))) return;
    if(p[y] < p[x]) swap(x,y);
    p[x] += p[y];//parents store size of set as negative integer
    p[y] = x;
}


bool cmp(edge x, edge y) {
    return x.weight > y.weight;
}

void dfsMst(int node, bool parody = true) {
    marked[node] = parody;
    visited[node] = true;
    for(int &to : mst[node]) {
        if(!visited[to]) {
            dfsMst(to, !parody);
        }
    }
}


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

void calcSCC() {
    for(int i = 0; i < 2*n; ++i) visited[i] = false;
    stack<int> seen;
    for(int i = 0; i < 2*n; ++i) {
        if(!visited[i]) {
            dfs1(i, seen);
        }
    }
    for(int i = 0; i < 2*n; ++i) visited[i] = false;
    sccID = 0;
    scc.resize(2*n+3);
    while(!seen.empty()) {
        while(!seen.empty() && visited[seen.top()]) seen.pop();
        if(!seen.empty()) {
            dfs2(seen.top());
            sccID++;
        }
    }
}


bool possible(int smallWeight, int largeWeight) {
    for(int i = 0; i < 2*n; ++i) {
        adj[i].clear();
        adjInv[i].clear();
    }
    for(auto &e : edges) {
        if(e.weight <= smallWeight) continue;
        if(e.weight <= largeWeight) {
            adj[e.i].push_back(e.j+n);
            adj[e.j].push_back(e.i+n);
            adjInv[e.j+n].push_back(e.i);
            adjInv[e.i+n].push_back(e.j);
        } else {
            adj[e.i+n].push_back(e.j);
            adj[e.j+n].push_back(e.i);
            adj[e.i].push_back(e.j+n);
            adj[e.j].push_back(e.i+n);
            adjInv[e.j].push_back(e.i+n);
            adjInv[e.i].push_back(e.j+n);
            adjInv[e.j+n].push_back(e.i);
            adjInv[e.i+n].push_back(e.j);
        }
    }
    calcSCC();
    vector<vector<int> > nodes(2*n);
    for(int i = 0; i < n; ++i) {
        if(scc[i] == scc[i+n]) {
            return false;
        }
    }
    for(int i = 0; i < 2*n; ++i) {
        status[i] = -1;
        nodes[scc[i]].push_back(i);
    }
    for(int i = sccID-1; i >= 0; --i) {
        if(status[i] != -1) continue;
        status[i] = true;
        for(int x : nodes[i]) {
            if(x < n) {
                if(status[scc[x+n]] == true) return false;
                status[scc[x+n]] = false;
            } else {
                if(status[scc[x-n]] == true) return false;
                status[scc[x-n]] = false;
            }
        }
    }
    return true;
}

int main() {
    cin >> n;
    if(n <= 2) {
        cout << "0\n";
        return 0;
    }
    int weight;
    vector<int> allWeights;
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            cin >> weight;
            allWeights.push_back(weight);
            Dist[i][j] = Dist[j][i] = weight;
            edges.push_back(edge(i,j,weight));
        }
    }
    allWeights.push_back(0);
    sort(edges.begin(), edges.end(), cmp);
    sort(allWeights.begin(), allWeights.end());
    allWeights.erase(unique(allWeights.begin(), allWeights.end()), allWeights.end());
    set<int> maxWeights;
    for(auto &e : edges) {
        if(find(e.i) != find(e.j)) {
            maxWeights.insert(e.weight);
            merge(e.i, e.j);
            mst[e.i].push_back(e.j);
            mst[e.j].push_back(e.i);
        }
    }
    for(int i = 0; i < n; ++i) marked[i] = visited[i] = false;
    dfsMst(0);
    int largestOne = -1, largestTwo = -1;
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            if(marked[i] == marked[j]) {
                if(marked[i]) {
                    largestOne = max(largestOne, Dist[i][j]);
                } else {
                    largestTwo = max(largestTwo, Dist[i][j]);
                }
            }
        }
    }
    if(largestOne != -1) maxWeights.insert(largestOne);
    if(largestTwo != -1) maxWeights.insert(largestTwo);
    ll largest = *maxWeights.rbegin();
    for(int w : maxWeights) {
        if(w >= largest) break;
        int start = -1, end = allWeights.size()-1, mid;
        while(end - start > 1) {
            mid = (start + end) >> 1;
            if(possible(allWeights[mid], w)) {
                end = mid;
            } else start = mid;
        }
        largest = min(largest, (ll)allWeights[end] + w);
    }
    cout << largest << '\n';
    return 0;
}

















