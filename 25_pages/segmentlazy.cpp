// modified from https://sites.google.com/site/indy256/algo/segment_tree_fast

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>

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
      value[x >> 1] = subResult(x >> 1, 1 << (d + 1));
      delta[x] = joinDeltas(delta[x], delta[x >> 1]);
      delta[x ^ 1] = joinDeltas(delta[x ^ 1], delta[x >> 1]);
      delta[x >> 1] = noChange;
    }
  }

  int subResult(int i, int len) {
    return joinValueWithDelta(value[i], deltaEffectOnSegment(delta[i], len));
  }

  int query(int from, int to) {
    from += n;
    to += n;
    int ofrom = from, oto = to;
    pushDelta(from);
    pushDelta(to);
    data res = 0;
    bool found = false;
    for (int len = 1; from <= to; from = (from + 1) >> 1, to = (to - 1) >> 1, len <<= 1) {
      if ((from & 1) != 0) {
        res = found ? combineVals(res, subResult(from, len)) : subResult(from, len);
        found = true;
      }
    }
    from = ofrom; to = oto;
    for (int len = 1; from <= to; from = (from + 1) >> 1, to = (to - 1) >> 1, len <<= 1) {
      if ((to & 1) == 0) {
        res = found ? combineVals(res, subResult(to, len)) : subResult(to, len);
        found = true;
      }
    }
    // if (!found) cerr << "unfound" << endl;
    return res;
  }

  void modify(int from, int to, lazy deltav) {
    from += n;
    to += n;
    pushDelta(from);
    pushDelta(to);
    int a = from;
    int b = to;
    for (; from <= to; from = (from + 1) >> 1, to = (to - 1) >> 1) {
      if ((from & 1) != 0) {
        delta[from] = joinDeltas(delta[from], deltav);
      }
      if ((to & 1) == 0) {
        delta[to] = joinDeltas(delta[to], deltav);
      }
    }
    for (int i = a, len = 1; i > 1; i >>= 1, len <<= 1) {
      value[i >> 1] = combineVals(subResult(i, len), subResult(i ^ 1, len));
    }
    for (int i = b, len = 1; i > 1; i >>= 1, len <<= 1) {
      value[i >> 1] = combineVals(subResult(i, len), subResult(i ^ 1, len));
    }
  }
};

//Circular rmq
// int main()
// {
//   ios_base::sync_with_stdio(false);
//   cin.tie(NULL);
//   cout.tie(NULL);
//   int n, m;
//   cin >> n;
//   segment s(n);
//   for(int i = 0; i < n; i++)
//     cin >> s.value[i+n];
//   s.build();
//   cin >> m;
//   string t;
//   getline(cin, t);
//   int a, b, c;
//   while(m--)
//   {
//     getline(cin, t);
//     stringstream ss(t);
//     ss >> a >> b;
//     if(ss >> c)
//     {
//       if(b < a)
//       {
//         s.modify(0, b, c);
//         s.modify(a, n - 1, c);
//       }
//       else
//         s.modify(a, b, c);
//     }
//     else
//     {
//       if(b < a)
//       {
//         cout << min(s.query(0, b), s.query(a, n - 1)) << "\n";
//       }
//       else
//         cout << s.query(a, b) << "\n";
//     }
//   }
//   return 0;
// }


// kquery
// int main()
// {
//   ios_base::sync_with_stdio(false);
//   cin.tie(NULL);
//   cout.tie(NULL);
//   int n, k;
//   // cin >> n;
//   scanf(" %i", &n);
//   segment s(n);
//   int t;
//   vector<int> idx(n), val(n);
//   for(int i = 0; i < n; i++)
//   {
//     // cin >> t;
//     scanf(" %i", &val[i]);
//     idx[i] = i;
//   }
//   sort(idx.begin(), idx.end(), [&val](int l, int r){return val[l] < val[r];});
//   // cin >> k;
//   scanf(" %i", &k);
//   vector<int> a(k), b(k), mid(k), idx2(k);
//   for(int i = 0; i < k; i++)
//   {
//     // cin >> a >> b >> mid;
//     scanf(" %i", &a[i]);
//     scanf(" %i", &b[i]);
//     scanf(" %i", &mid[i]);
//     idx2[i] = i;
//   }
//   sort(idx2.begin(), idx2.end(), [&mid](int l, int r){return mid[l] > mid[r];});
//   vector<int> res(k);

//   int sm = n - 1;
//   for(int i = 0; i < k; i++)
//   {
//     while(sm >= 0 && mid[idx2[i]] < val[idx[sm]])
//     {
//       s.modify(idx[sm]+1, idx[sm]+1, 1);
//       sm--;
//     }
//     res[idx2[i]] = s.query(a[idx2[i]], b[idx2[i]]);
//   }
//   for(int i = 0; i < k; i++)
//   {
//     cout << res[i] << "\n";
//   }
//   return 0;
// }

// Random test
// int main() {
//   srand(time(NULL));
//   for (int step = 0; step < 1000; step++) {
//     int n = (rand() % 50) + 1;
//     segment t(n);
//     vector<int> x;
//     x.resize(n, t.zeroVal);
//     for (int i = 0; i < 1000; i++) {
//       int b = (rand() % n);
//       int a = (rand() % n);
//       if(a > b) swap(a, b);
//       int cmd = (rand() % 3);
//       if (cmd == 0) {
//         int delta = (rand() % 100) - 50;
//         t.modify(a, b, delta);
//         for (int j = a; j <= b; j++) {
//           x[j] = t.joinValueWithDelta(x[j], delta);
//         }
//       } else if (cmd == 1) {
//         int res1 = t.query(a, b);
//         int res2 = x[a];
//         for (int j = a + 1; j <= b; j++) {
//           res2 = t.combineVals(res2, x[j]);
//         }
//         if (res1 != res2) {
//           cerr << "failed" << endl;
//           return 0;
//         }

//       } else {
//         for (int j = 0; j < n; j++) {
//           if (t.query(j, j) != x[j]) {
//             cerr << "failed" << endl;
//             return 0;
//           }
//         }
//       }
//     }
//   }
//   cout << "Tests passed" << endl;
//   return 0;
// }