#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

struct up
{
	ll add;
	up(): add(0) {};
	up(ll i): add(i) {};
};

struct dp
{
	ll val;
	dp(): val(0) {};
	dp(ll i): val(i) {};
	void update(const up &u)
	{
		val += u.add;
	}
};

dp combine(dp l, dp r)
{
	return dp(l.val + r.val);
}

struct segment
{
	vector<dp> data;
	segment *l = nullptr, *r = nullptr;
	int s;
	segment(int sz, const vector<dp> &base = vector<dp>(0)): s(sz)
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
	void update(int spot, const up &update)
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

int main()
{
	ios_base::sync_with_stdio(false);
	int n, q;
	scanf("%d %d", &n, &q);
	segment s(n);
	char type;
	ll idx, val;
	while(q--)
	{
		scanf(" %c", &type);
		// cin >> type;
		if(type == '+')
		{
			// cin >> idx >> val;
			scanf("%lli %lli", &idx, &val);
			s.update(idx, up(val));
		}
		else
		{
			// cin >> idx;
			scanf("%lli", &idx);
			cout << s.query(0, idx).val << "\n";
		}
	}
	return 0;
}

