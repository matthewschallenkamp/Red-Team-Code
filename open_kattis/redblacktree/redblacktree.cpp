#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <vector>
#include <queue>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

typedef long long ll;
ll mod = 1000000007LL;

void mult(const vector<ll> &a, const vector<ll> &b, vector<ll>& c)
{
	c.resize(a.size() + b.size() - 1, 0);
	for(int i = 0; i < a.size(); i++) {
		for(int j = 0; j < b.size(); j++) {
			c[i+j] += a[i] * b[j];
			c[i+j] %= mod;
		}
	}
}


int main() {
	iostream::sync_with_stdio(false);
	cin.tie(NULL);
	int n, m;

  cin >> n >> m;

  // read input
  deque<int> parent(n-1);
	vector<vector<int> > children(n);
  for(auto &i : parent) cin >> i;
  parent.push_front(-1);
  for(int i = 1; i < parent.size(); i++) {
    children[parent[i]-1].push_back(i);
  }
	vector<bool> red(n, false);
  int tmp;
  for(int i = 0; i < m; i++) {
    cin >> tmp;
    red[tmp-1] = true;
  }

	//do DFS for ordering -> children after
  vector<int> todo;
  todo.push_back(0);
  vector<int> order;
  while(todo.size()) {
    order.push_back(todo.back());
    todo.pop_back();
    for(auto item : children[order.back()])
      todo.push_back(item);
  }
  //reverse -> children before
  reverse(order.begin(), order.end());

  //in order, calc subresults
	vector<vector<ll> > subreses(n);
  for(auto idx : order) {
    //subresults for children are already finished
    //lets recombine small items for speed
    priority_queue<pair<int, vector<ll>>> subs;
    subs.push(make_pair(-1, vector<ll>(1,1))); // we need a default always
    for(auto item : children[idx]) {
      subs.push(make_pair(-subreses[item].size(), subreses[item]));
    }

    while(subs.size() > 1) {
      auto l = subs.top().second; subs.pop();
      auto r = subs.top().second; subs.pop();
      vector<ll> subres;
      mult(l, r, subres);
      subs.push(make_pair(-subres.size(), subres));
    }
    subreses[idx] = subs.top().second; subs.pop();
    //for reds, add 0, 1
    //for blacks add 1, 0
    if(red[idx]) {
    	subreses[idx].resize(max(subreses[idx].size(), 2UL));
      subreses[idx][1]++;
    }
    else {
      subreses[idx][0]++;
    }
    while(subreses[idx].back() == 0) subreses[idx].pop_back();
  }
  auto res = subreses[0];
  res.resize(m + 1);
  //since we don't mod after increment at 82/85
  for(auto i : res) cout << i % mod << endl;
  return 0;
}





