// modified from https://sites.google.com/site/indy256/algo/segment_tree_fast

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <deque>

using namespace std;

typedef long long ll;

struct data
{
  int ct0 = 0, ct1 = 0, up = 1, down = 1;
  data(int i) {
    if(i)
      ct1 = 1;
    else
      ct0 = 1;
  }
  data() {
    ct0 = 1;
  }
  data reverse()
  {
    data d;
    d.ct1 = ct0;
    d.ct0 = ct1;
    d.up = down;
    d.down = up;
    return d;
  }
};

typedef bool lazy;

struct segment {
  lazy noChange = 0;
  data zeroVal = 0;

  int n;
  vector<data> value;
  vector<lazy> delta; // delta[i] affects value[i], delta[2*i+1] and delta[2*i+2]

  // Modify the following 5 methods to implement your custom operations on the tree.

  //data combined with delta
  data modifyOperation(data x, lazy y) {
    if(y) {
      return x.reverse();
    }
    return x;
  }

  // data combine operation
  data combineVals(data leftValue, data rightValue) {
    data d;
    d.ct0 = leftValue.ct0 + rightValue.ct0;
    d.ct1 = leftValue.ct1 + rightValue.ct1;
    d.up = max(leftValue.ct0 + rightValue.up, leftValue.up + rightValue.ct1);
    d.down = max(leftValue.ct1 + rightValue.down, leftValue.down + rightValue.ct0);
    return d;
  }

  lazy deltaEffectOnSegment(lazy delta, int segmentLength) {
    return delta;
  }

  data joinValueWithDelta(data value, lazy delta) {
    if(delta)
      return value.reverse();
    return value;
  }

  lazy joinDeltas(lazy delta1, lazy delta2) {
    return delta1 != delta2;
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


//http://www.spoj.com/problems/QUERYIT/
int main()
{
  int n, q, t, l, r;
  char temp;
  cin >> n >> q;
  segment s(n);
  for(int i = n; i < 2*n; i++)
  {
    cin >> temp;
    s.value[i] = data((int)(temp - '0'));
  }
  s.build();
  for(int i = 0; i < q; i++)
  {
    cin >> t;
    if(t == 0)
    {
      cin >> l >> r;
      s.modify(l-1, r-1, 1);
    }
    else
    {
      cout << s.query(0, n-1).up << "\n";
    }
  }
  cout << flush;
  return 0;
}