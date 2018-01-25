#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef int ll;

vector<ll> as, bs, ts;

int main()
{
	int n;
	cin >> n;
	as.resize(n);
	ts = bs = as;
	for (int i = 0; i < n; i++)
	{
		cin >> as[i] >> bs[i] >> ts[i];
	}

	//the table indexes represent [ride number][time left]
	vector<vector<ll> > table(n + 1);
	table[n].resize(25005, 0);
	for (int i = n - 1; i >= 0; i--)
	{
		table[i].resize(25005, 0);
		for (int tl = 0; tl < 25005; tl++)
		{
			ll best = 0;
			if (bs[i] == 0)
			{
				// 0 reducers recurse down or left
				if (tl >= ts[i])
				{
					best = max(best, as[i] + table[i][tl - ts[i]]);
				}
				best = max(best, table[i + 1][tl]);
			}
			else
			{
				// real reducers require us to try all the possible combos, up to ~32
				ll sum = 0;
				for (int k = 1; true; k++)
				{
					if (as[i] - (k - 1) * (k - 1) * bs[i] <= 0)
						break;
					sum += as[i] - (k - 1) * (k - 1) * bs[i];
					if (k * ts[i] <= tl)
						best = max(best, sum + table[i + 1][tl - k * ts[i]]);
					else
						break;
				}
				best = max(best, table[i + 1][tl]);
			}
			table[i][tl] = best;
		}
	}

	int q;
	cin >> q;
	vector<ll> qs(q);
	for (int i = 0; i < q; i++)
	{
		cin >> qs[i];
		cout << table[0][qs[i]] << endl;
	}
}
