#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

typedef int dt;

struct up
{
	dt add;
	up(): add(1) {};
	up(dt i): add(i) {
		if(add > 0)
			add = 1;
		if(add < 0)
			add = -1;
	};
};

struct dp
{
	dt val;
	dp(): val(1) {};
	dp(dt i): val(i) {
		if(val > 0)
			val = 1;
		if(val < 0)
			val = -1;
	};
	void update(const up &u)
	{
		val = u.add;
	}
};

dp combine(dp l, dp r)
{
	return dp(l.val * r.val);
}

struct segment
{
	vector<dp> data;
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
	void print()
	{
		for(auto &item : data)
			cerr << item.val << " ";
		cerr << endl;
	}
};

int main()
{
	int n, k;
	while(cin >> n >> k)
	{
		vector<dp> base(n);
		int t;
		for(int i = 0; i < n; i++)
		{
			cin >> t;
			base[i] = dp(t);
		}
		segment s(n, base);
		char c;
		int a, b;
		for(int i = 0; i < k; i++)
		{
			cin >> c >> a >> b;
			if(c == 'C')
			{
				s.update(a-1, up(b));
			}
			else
			{
				switch(s.query(a-1, b).val)
				{
					case 1:
						cout << "+";
						break;
					case 0:
						cout << "0";
						break;
					case -1:
						cout << "-";
						break;
				}
			}
		}
		cout << endl;
	}
	return 0;
}