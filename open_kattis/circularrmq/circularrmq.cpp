// modified from https://sites.google.com/site/indy256/algo/segment_tree_fast

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <deque>

using namespace std;

typedef long long ll;

typedef ll data;
typedef ll lazy;

struct segment {
  lazy noChange = 0;
  data zeroVal = 0;

  int n;
  vector<data> value;
  vector<lazy> delta; // delta[i] affects value[i], delta[2*i+1] and delta[2*i+2]

  // Modify the following 5 methods to implement your custom operations on the tree.

  //data combined with delta
  data modifyOperation(data x, lazy y) {
    return x + y;
  }

  // data combine operation
  data combineVals(data leftValue, data rightValue) {
    return min(leftValue, rightValue);
  }

  lazy deltaEffectOnSegment(lazy delta, int segmentLength) {
    return delta;
  }

  data joinValueWithDelta(data value, lazy delta) {
    return value + delta;
  }

  lazy joinDeltas(lazy delta1, lazy delta2) {
    return delta1 + delta2;
  }

  // generic code
  segment(int nn) {
    n = nn;
    value.resize(2 * n, zeroVal);
    delta.resize(2 * n, noChange);
  }

  void build() {
    for (int i = n - 1; i > 0; i--) {
      value[i] = combineVals(value[2 * i], value[2 * i + 1]);
    }
  }

  void pushDelta(int i) {
    int d = 0;
    for (; (i >> d) > 0; d++) {
    }
    for (d -= 2; d >= 0; d--) {
      int x = i >> d;
      value[x / 2] = subResult(x / 2, 1 << (d + 1));
      delta[x] = joinDeltas(delta[x], delta[x / 2]);
      delta[x ^ 1] = joinDeltas(delta[x ^ 1], delta[x / 2]);
      delta[x / 2] = noChange;
    }
  }

  data subResult(int i, int len) {
    return joinValueWithDelta(value[i], deltaEffectOnSegment(delta[i], len));
  }

  data query(int from, int to) {
    from += n;
    to += n;
    pushDelta(from);
    pushDelta(to);
    deque<pair<int, int>> left, right;
    for (int len = 1; from <= to; from = (from + 1) / 2, to = (to - 1) / 2, len *= 2) {
      if ((from & 1) != 0) {
        left.push_back(make_pair(from, len));
      }
      if((to & 1) == 0) {
        right.push_front(make_pair(to, len));
      }
    }
    for(auto item : right) {
      left.push_back(item);
    }
    data res = subResult(left[0].first, left[0].second);
    for(int i = 1; i < left.size(); i++) {
      res = combineVals(res, subResult(left[i].first, left[i].second));
    }
    return res;
  }

  void modify(int from, int to, lazy deltav) {
    from += n;
    to += n;
    int a = from, b = to;
    pushDelta(from);
    pushDelta(to);
    for (; from <= to; from = (from + 1) / 2, to = (to - 1) / 2) {
      if ((from & 1) != 0) {
        delta[from] = joinDeltas(delta[from], deltav);
      }
      if ((to & 1) == 0) {
        delta[to] = joinDeltas(delta[to], deltav);
      }
    }
    for (int i = a, len = 1; i > 1; i >>= 1, len <<= 1) {
      value[i / 2] = combineVals(subResult(min(i, i ^ 1), len), subResult(max(i, i ^ 1), len));
    }
    for (int i = b, len = 1; i > 1; i >>= 1, len <<= 1) {
      value[i / 2] = combineVals(subResult(min(i, i ^ 1), len), subResult(max(i, i ^ 1), len));
    }
  }
};

// Circular rmq
//http://codeforces.com/contest/52/problem/C
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  int n, m;
  cin >> n;
  segment s(n);
  for(int i = 0; i < n; i++)
    cin >> s.value[i+n];
  s.build();
  cin >> m;
  string t;
  getline(cin, t);
  int a, b, c;
  while(m--)
  {
    getline(cin, t);
    stringstream ss(t);
    ss >> a >> b;
    if(ss >> c)
    {
      if(b < a)
      {
        s.modify(0, b, c);
        s.modify(a, n - 1, c);
      }
      else
        s.modify(a, b, c);
    }
    else
    {
      if(b < a)
      {
        cout << min(s.query(0, b), s.query(a, n - 1)) << "\n";
      }
      else
        cout << s.query(a, b) << "\n";
    }
  }
  return 0;
}
