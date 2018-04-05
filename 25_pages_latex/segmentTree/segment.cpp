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
		val = u;
	}
};
dp combine(dp l, dp r)
{
	return dp(l.val + r.val);
}

struct segment
{
	vector<dp> data;
	int s;
	segment(int sz, const vector<dp> &base = vector<dp>()): s(sz)
	{
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
		for(int i = (spot + s) / 2; i > 0; i /= 2)
			data[i] = combine(data[2*i], data[2*i+1]);
	}
	dp query(int si, int ei)
	{
		dp res;
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
