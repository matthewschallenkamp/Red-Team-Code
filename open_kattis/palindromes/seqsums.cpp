#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <deque>

using namespace std;

typedef long long ll;

typedef pair<int, int> data; // failct, othersum
typedef bool lazy;

struct segment {
  lazy noChange = 0;	
  data zeroVal = make_pair(0, 0);

  int n;
  vector<data> value;
  vector<lazy> delta; // delta[i] affects value[i], delta[2*i+1] and delta[2*i+2]

  // Modify the following 5 methods to implement your custom operations on the tree.

  //data combined with delta
  data modifyOperation(data x, lazy y) {
  	if(y)
  		return make_pair(1, 0);
    return x;
  }

  // data combine operation
  data combineVals(data leftValue, data rightValue) {
    return make_pair(leftValue.first + rightValue.first, leftValue.second + rightValue.second);
  }

  lazy deltaEffectOnSegment(lazy delta, int segmentLength) {
    return delta;
  }

  data joinValueWithDelta(data value, lazy delta) {
    return modifyOperation(value, delta);
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

int main()
{
	vector<int> arr(10000);
	srand(time(NULL));
	for(auto &item : arr)	item = rand() % arr.size();

	segment s(arr.size());
	// say we sub index from each item
	vector<int> newarr = arr;
	for(int i = 0; i < newarr.size(); i++)
		newarr[i] -= i;

	// then query looks like
	// min(i + arr[i], i - L)
	// we can pull out the i, for i + min(arr[i], -L)
	// then we can add sum(l->r) = (l*len + (r-l)*(r-l+1)/2) at the end
	// to get min(arr[i], -L)

	//seg tree
	for(int i = 0; i < newarr.size(); i++)
	{
		s.value[newarr.size() + i] = make_pair(0, newarr[i]);
	}
	s.build();

	//spots list
	deque<pair<int, int> > spots;
	for(int i = 0; i < newarr.size(); i++)
	{
		spots.push_back(make_pair(newarr[i], i));
	}
	sort(spots.begin(), spots.end());

	deque<pair<int, int> > queries;
	for(int i = 0; i < 100000; i++)
	{
		int l = rand() % arr.size(), r = rand() % arr.size();
		if(l > r) swap(l, r);
		queries.push_back(make_pair(l, r));
	}
	// order our queries in increasing L
	sort(queries.begin(), queries.end());

	for(auto q : queries) {
		int l = q.first, r = q.second;
		//slow query
		int sum = 0;
		for(int j = 0; j + l <= r; j++)
		{
			sum += min(j, arr[j + l]);
		}
		//fast
		while(spots.size() && spots.back().first >= -l)
		{
			//remove these items from seg tree, replace them with an L count
			s.value[spots.back().second + newarr.size()] = make_pair(1, 0);
			s.modify(spots.back().second, spots.back().second, 0); // push recombines
			spots.pop_back(); //and dont consider them again
		}

		auto res = s.query(l, r); //pair of L ct / remaining sum
		int sum2 = ((r - l) * (r - l + 1)) / 2; //get the i parts
		sum2 += l * (r - l + 1); // and the L parts
		sum2 += res.second; //and the remaining sum
		sum2 += res.first * (-l); //and the items set to L

		if(sum != sum2) {
			cout << "RANDOM TEST FAIL " << l << " " << r << " " << sum << " " << sum2 << endl;
		}
	}

}