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

vector<vector<pair<int, int> > > edges;
ll all;

int dfs_len(int current = 0, ll visited = 0)
{
  // cerr << current << " " << visited << endl;
  int best = 10000000;
  for(auto p : edges[current])
  {
    if(p.first == 0)
    {
      if((visited | 1LL) == all)
      {
        //we are finished, this is a path
        best = min(best, p.second);
      }
      //else this is not a full path, ignore
    }
    else
    {
      //there is more to this path, check we can visit this spot
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
    // cerr << "\n\n\n\n";
    int n, m, f, t, c;
    cin >> n >> m;
    all = (1LL << n) - 1;

    edges.clear();
    edges.resize(n);
    while(m--)
    {
      cin >> f >> t >> c;
      edges[f].push_back(make_pair(t, c));
    }

    cout << dfs_len() << endl;
  }
  return 0;
}
