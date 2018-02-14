#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

typedef vector<int> dt;

struct up
{
	dt add;
	up(): add() {};
	up(const dt &i): add(i) {};
};

struct dp
{
	dt val;
	dp(): val() {};
	dp(const dt &i): val(i) {};
	void update(const up &u)
	{
		val = u.add;
	}
};

dp combine(const dp &l, const dp &r)
{
	dt combo;
	int li = 0, ri = 0;
	while(li < l.val.size() && ri < r.val.size())
	{
		if(l.val[li] < r.val[ri])
			combo.push_back(l.val[li++]);
		else
			combo.push_back(r.val[ri++]);
	}
	while(li < l.val.size())
	{
		combo.push_back(l.val[li++]);
	}

	while(ri < r.val.size())
	{
		combo.push_back(r.val[ri++]);
	}
	return dp(combo);
}

int val(const vector<int> &a, int mid)
{
	return a.end() - upper_bound(a.begin(), a.end(), mid);
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
	void update(int spot, const up &update)
	{
		data[spot + s].update(update);
		for(int i = (spot + s) / 2; i > 0; i /= 2)
			data[i] = combine(data[2*i], data[2*i+1]);
	}
	int query(int si, int ei, int mid)
	{
		int res = 0;
		for(si += s, ei += s; si < ei; si /= 2, ei /= 2)
		{
			if(si % 2 == 1)
				res += val(data[si++].val, mid);
			if(ei % 2 == 1)
				res += val(data[--ei].val, mid);
		}
		return res;
	}
	// void print()
	// {
	// 	for(auto &item : data)
	// 		cerr << item.val << " ";
	// 	cerr << endl;
	// }
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int n, k, last = 0;
	scanf(" %i", &n);
	// cin >> n;
	vector<dp> base(n);
	int t;
	for(int i = 0; i < n; i++)
	{
		// cin >> t;
		scanf(" %i", &t);
		base[i] = dp(vector<int>(1, t));
	}
	segment s(n, base);
	// cin >> k;
	scanf(" %i", &k);
	int a, b, mid;
	for(int i = 0; i < k; i++)
	{
		// cin >> a >> b >> mid;
		scanf(" %i", &a);
		scanf(" %i", &b);
		scanf(" %i", &mid);

		a ^= last;
		b ^= last;
		mid ^= last;
		if(a < 1) a = 1;
		if(b > n) b = n;
		if(a > b) 
			cout << (last = 0) << "\n";
		else
			cout << (last = s.query(a-1, b, mid)) << "\n";
	}
	
	return 0;
}