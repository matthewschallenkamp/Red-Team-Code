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
	up(dt i): add(i) {};
};

struct dp
{
	dt val;
	dp(): val(0) {};
	dp(dt i): val(i) {};
	void update(const up &u)
	{
		val = u.add;
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
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int n, k;
	// cin >> n;
	scanf(" %i", &n);
	segment s(n);
	int t;
	vector<int> idx(n), val(n);
	for(int i = 0; i < n; i++)
	{
		// cin >> t;
		scanf(" %i", &val[i]);
		idx[i] = i;
	}
	sort(idx.begin(), idx.end(), [&val](int l, int r){return val[l] < val[r];});
	// cin >> k;
	scanf(" %i", &k);
	vector<int> a(k), b(k), mid(k), idx2(k);
	for(int i = 0; i < k; i++)
	{
		// cin >> a >> b >> mid;
		scanf(" %i", &a[i]);
		scanf(" %i", &b[i]);
		scanf(" %i", &mid[i]);
		idx2[i] = i;
	}
	sort(idx2.begin(), idx2.end(), [&mid](int l, int r){return mid[l] > mid[r];});
	vector<int> res(k);

	int sm = n - 1;
	for(int i = 0; i < k; i++)
	{
		while(sm >= 0 && mid[idx2[i]] < val[idx[sm]])
		{
			s.update(idx[sm--], 1);
		}
		res[idx2[i]] = s.query(a[idx2[i]]-1, b[idx2[i]]).val;
	}
	for(int i = 0; i < k; i++)
	{
		cout << res[i] << "\n";
	}
	return 0;
}