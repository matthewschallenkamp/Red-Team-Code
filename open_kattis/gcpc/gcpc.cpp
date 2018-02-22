#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <set>
#include <queue>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

typedef pair<int, pair<int, int > > score;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;//order_of_key, key_of_order
template<class TI>
using indexed_set = tree<TI,null_type,less<TI>,rb_tree_tag,tree_order_statistics_node_update>;

int main()
{
  int n, k;
  cin >> n >> k;
  
  int team, penalty;
  int fprobs, fpen;
  indexed_set<score> is;
  vector<score> teams;
  teams.push_back(score(0, make_pair(0, 0)));
  for(int i = 1; i <= n; i++)
  {
    score s(0, make_pair(0, i));
    teams.push_back(s);
    is.insert(s);
  }
  
  for(int i = 0; i < k; i++)
  {
    cin >> team >> penalty;
    //remove old team
    score s = teams[team];
    is.erase(s);
    //insert new team
    s.first--;
    s.second.first += penalty;
    is.insert(s);
    teams[team] = s;
    const score temp = teams[1];
    //find place of teams[1] and print
    cout << is.order_of_key(temp) + 1 << endl;
  }

  return 0;
}
