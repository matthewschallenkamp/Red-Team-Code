#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef int dt;

struct dp
{
	dt val;
	dp(): val(0) {};
	dp(dt i): val(i) {};
	void update(const dt &u)
	{
		//we may want to add, set, multiply, etc
		val = u;
	}
};

dp combine(dp l, dp r)
{
	//we may want to add, multiply, xor, etc
	return dp(l.val + r.val);
}

struct segment
{
	//segment tree for dp with point updates and range queries
	vector<dp> data;
	int s;
	segment(int sz, const vector<dp> &base = vector<dp>()): s(sz)
	{
		//first s elements are our tree, next s are the array, 0 unused
		//each node i has children 2i and 2i+1
		data.resize(2 * s);
		for(int i = 0; i < base.size(); i++)
		{
			data[s+i] = base[i];
		}
		for(int i = s - 1; i > 0; i--)
		{
			data[i] = combine(data[2*i], data[2*i+1]);
		}
	}
	void update(int spot, const dt &update)
	{
		data[spot + s].update(update);
		//walk back through parents and recombine each
		for(int i = (spot + s) / 2; i > 0; i /= 2)
			data[i] = combine(data[2*i], data[2*i+1]);
	}
	dp query(int si, int ei)
	{
		dp res;
		//[si, ei) are edges at this level. 
		//when last bit is set parent may include extra that we don't want
		//so add this subtree into result now and exclude it at higher levels
		for(si += s, ei += s; si < ei; si /= 2, ei /= 2)
		{
			if(si % 2 == 1)
				res = combine(res, data[si++]);
			if(ei % 2 == 1)
				res = combine(res, data[--ei]);
		}
		return res;
	}
};
