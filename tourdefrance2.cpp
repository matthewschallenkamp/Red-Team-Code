#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_map>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

//these stay the same per dfs, so globals are simple.
vector<vector<pair<int, int> > > edges;
ll all;

int dfs_len(int current = 0, ll visited = 0)
{
  //guaranteed max is 37 * 10000 = 370000, and that there is a solution, so this is safe
  int best = 10000000;
  for(auto p : edges[current])
  {
    //we always start and end at node 0. check if this is a full path
    if(p.first == 0)
    {
      if((visited | 1LL) == all)
      {
        //we are finished, this is a valid path, update our best
        best = min(best, p.second);
      }
      //else this is not a full path (cannot repeat nodes), ignore it
    }
    else
    {
      //there is more to this path, check we can visit this spot and update our best
      if(! (visited & (1LL << p.first)))
        best = min(best, p.second + dfs_len(p.first, visited | (1LL << p.first)));
    }    
  }
  
  return best;
}

int main()
{
  int tc;
  cin >> tc;
  while(tc--)
  {
    // nodes, edges, from, to, cost
    int n, m, f, t, c;
    cin >> n >> m;
    // 1's for each node to visit
    all = (1LL << n) - 1;

    edges.clear();
    edges.resize(n);
    while(m--)
    {
      cin >> f >> t >> c;
      // the first index is the from node, second index doesnt matter
      // first item in the pair is the to node, and the second item is the cost
      edges[f].push_back(make_pair(t, c));
    }

    cout << dfs_len() << endl;
  }
  return 0;
}
