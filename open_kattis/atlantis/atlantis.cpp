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
    return y ? 0 : x;
  }

  // data combine operation
  data combineVals(data leftValue, data rightValue) {
    return leftValue + rightValue;
  }

  lazy deltaEffectOnSegment(lazy delta, int segmentLength) {
    return delta;
  }

  data joinValueWithDelta(data value, lazy delta) {
    return delta ? 0 : value;
  }

  lazy joinDeltas(lazy delta1, lazy delta2) {
    return delta1 | delta2;
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
    data res = zeroVal;
    for(int i = 0; i < left.size(); i++) {
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

int main()
{
	int n;
	cin >> n;
	vector<pair<int, int> > stores(n);
	vector<int> heightsort(n);
	while(n--)
	{
		heightsort[n] = n;
		cin >> stores[n].first >> stores[n].second;
	}
	sort(stores.begin(), stores.end(), [](pair<int, int> lp, pair<int, int> rp) {
		return lp.first == rp.first ?
			lp.second > rp.second :
			lp.first < rp.first;
	});
	sort(heightsort.begin(), heightsort.end(), [&stores](int l, int r) {
		return stores[l].second < stores[r].second;
	});
	vector<int> invhs(heightsort.size());
	invhs[heightsort[0]] = 0;
	for(int i = 1; i < heightsort.size(); i++)
	{
			invhs[heightsort[i]] = i;
			if(stores[heightsort[i-1]].second == stores[heightsort[i]].second)
				invhs[heightsort[i]] = invhs[heightsort[i-1]];
	}
	// for(auto item : stores)
	// 	cerr << item.first << " " << item.second << " ";
	// cerr << endl;
	// for(auto item : heightsort)
	// 	cerr << item << "    ";
	// cerr << endl;

	segment seg(stores.size());
	int last = 0;
	for(int i = 0; i < heightsort.size(); i++)
	{
		seg.value[heightsort.size() + i] = stores[heightsort[i]].second - last;
		last = stores[heightsort[i]].second;
	}

	int ct = 0;
	seg.build();
	for(int i = 0; i < stores.size(); i++) {
		// cerr << seg.query(0, invhs[i]) << endl;
		int q = seg.query(0, invhs[i]);
		if(q >= stores[i].first)
		{
			int s = 0, e = invhs[i] + 1;
			while(e - s > 1)
			{
				int mid = (e + s) / 2;
				if(seg.query(mid, invhs[i]) < stores[i].first)
					e = mid;
				else
					s = mid;
			}
			int got = seg.query(e, invhs[i]);
			// cerr << got << " " << e << " " << invhs[i] << endl;
			if(got > 0)
				seg.modify(e, invhs[i], 1);
			seg.value[stores.size() + s] -= (stores[i].first - got);
			seg.modify(s, s, 0); // push the recombine ops
			ct++;
		}
	}
	cout << ct << endl;

	return 0;
}