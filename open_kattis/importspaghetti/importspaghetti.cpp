#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <queue>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

vector<vector<bool> > adj;
unordered_map<string, int> table;
vector<string> tableInv;
vector<int> visited;
vector<int> previous;
int n;

bool dfs(int node) {
  visited[node] = 1;
  for(int i = 0; i < n; ++i) {
    if(adj[node][i]) {
      if(visited[i]==1) {
        return true;
      }
      if(visited[i] == 0 && dfs(i)) return true;
    }
  }
  visited[node] = 2;
  return false;
}

void bfs(int node, int goal) {
  visited[node] = 1;
  queue<int> q;
  q.push(node);
  while(!q.empty()) {
    int curr = q.front();
    q.pop();
    for(int i = 0; i < n; ++i) {
      if(!visited[i] && adj[curr][i]) {
        previous[i] = curr;
        if(i==goal) return;
        visited[i] = 1;
        q.push(i);
      }
    }
  }
}

int main()
{
  cin >> n;
  adj.resize(n, vector<bool>(n,false));
  tableInv.resize(n);
  visited.resize(n,0);
  string node;
  for(int i = 0; i < n; ++i) {
    cin >> node;
    table[node] = i;
    tableInv[i] = node;
  }
  int k;
  string to;
  while(cin >> node >> k) {
    while(k--) {
      cin >> to;
      while(cin >> to) {
        if(to.back() != ',') {
          if(node == to) {
            cout << node << '\n';
            return 0;
          }
          adj[table[node]][table[to]] = true;
          break;
        }
        to = to.substr(0,to.size()-1);
        if(node == to) {//self-edge, shortest cycle is this one node
          cout << node << '\n';
          return 0;
        }
        adj[table[node]][table[to]] = true;
      }
    }
  }
  //check for cycles using DFS, This turned out to be unnecessary, as we can check after running Floyd-Warshall.
  bool cycle = false;
  for(int i = 0; i < n; ++i) {
    if(!visited[i]) cycle |= dfs(i);
  }
  if(!cycle) {
    cout << "SHIP IT\n";
    return 0;
  }
  vector<vector<int> > dist(n,vector<int>(n,1e8));
  for(int i = 0; i < n; ++i) adj[i][i] = 0;
  //Floyd-Warshall
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      if(adj[i][j]) dist[i][j] = 1;
    }
  }
  for(int k = 0; k < n; ++k) {
    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
  //end of Floyd-Warshall
  int minCycle = 1e8;
  int index1, index2;
  //find shortest cycle
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      if(minCycle > dist[i][j] + dist[j][i]) {
        minCycle = dist[i][j] + dist[j][i];
        index1 = i;
        index2 = j;
      }
    }
  }
  //run BFS to retrieve path
  visited.clear();
  visited.resize(n,0);
  previous.resize(n);
  bfs(index1, index2);
  vector<int> path1;
  int curr = index2;
  while(curr != index1) {
    path1.push_back(curr);
    curr = previous[curr];
  }
  path1.push_back(curr);
  reverse(path1.begin(), path1.end());
  path1.pop_back();
  visited.clear();
  visited.resize(n,0);
  vector<int> temp;
  previous.resize(n);
  bfs(index2, index1);
  
  vector<int> path2;
  curr = index1;
  while(curr != index2) {
    path2.push_back(curr);
    curr = previous[curr];
  }
  path2.push_back(curr);
  reverse(path2.begin(), path2.end());
  path2.pop_back();
  
  for(int x : path1) cout << tableInv[x] << ' ';
  for(int x : path2) cout << tableInv[x] << ' ';
  cout << '\n';
}





