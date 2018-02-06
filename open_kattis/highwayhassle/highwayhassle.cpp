#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <queue>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

vector<ll> d(vector<vector<pair<ll, ll> > > &g, ll s, ll dist)
{
	vector<ll> mindist(g.size(), 1L << 60);
	priority_queue<pair<ll, ll> > b;
	b.push(make_pair(0, s));
	while(b.size())
	{
		auto top = b.top();
		b.pop();
		if(mindist[top.second] <= -top.first)
			continue;
		else
			mindist[top.second] = -top.first;
		for(auto p : g[top.second])
		{
			if(p.second + mindist[top.second] <= dist)
			{
				b.push(make_pair(-p.second - mindist[top.second], p.first));
			}
		}
	}
	return mindist;
}

int main()
{
	ll tc;
	cin >> tc;
	while(tc--)
	{
		ll n, m, s, t;
		cin >> n >> m >> s >> t;
		vector<vector<pair<ll, ll>>> g(n+1);
		for(ll i = 0; i < m; i++)
		{
			ll a, b, c;
			cin >> a >> b >> c;
			g[a].emplace_back(b,c);
			g[b].emplace_back(a,c);
		}
		vector<pair<ll, ll>> ss(s);
		for(ll i = 0; i < s; i++)
		{
			cin >> ss[i].first >> ss[i].second;
		}
		ll source, dest, newsource;
		cin >> source >> dest;

		// cerr << endl;
		ss.push_back(make_pair(dest, 0));
		vector<vector<ll> > adj;
		for(auto item : ss)
		{
			if(item.first == source)
				newsource = adj.size();
			auto temp = d(g, item.first, t);
			adj.push_back({});
			for(auto item2 : ss)
			{
				adj.back().push_back(temp[item2.first]);
				// cerr<< temp[item2.first] << " ";
			}
			// cerr << endl;
		}
		// cerr << newsource << endl;
		//we now have an adjacency list for the ss
		//do a djikstras for shortest path with the gas buying rules known.
		//but give trucks with gas left a pass

		vector<ll> mindist(adj.size(), 1L << 60);
		//        cost to get to, gas left, loc
		priority_queue<pair<ll, pair<ll, ll>>> b;
		b.push(make_pair(0, make_pair(0, newsource)));
		while(b.size())
		{
			auto top = b.top();
			b.pop();
			// cerr << top.first << " " << top.second.first << " " << top.second.second << endl;
			if(top.second.second == (adj.size() - 1))
			{
				//this is our guy
				cout << -top.first << endl;
				break;
			}
			if(mindist[top.second.second] > -top.first)
				mindist[top.second.second] = -top.first;
			else if(top.second.first == 0) //we traveled to a less expensive, so no extra gas
				continue;
			for(ll i = 0; i < adj.size(); i++)
			{
				if(adj[top.second.second][i] != 1L << 60 && top.second.second != i)
				{
					if(ss[i].second < ss[top.second.second].second)
					{
						//we are going to a less expensive place. buy just enough gas
						auto np = top;
						ll tobuy = adj[top.second.second][i] - np.second.first;
						if(tobuy < 0) continue;
						np.first -= tobuy * ss[top.second.second].second;
						np.second.first = 0;
						np.second.second = i;
						b.push(np);
					}
					else
					{
						//here is less expensive, buy what we can
						auto np = top;
						ll tobuy = t - np.second.first;
						np.first -= tobuy * ss[top.second.second].second;
						np.second.first = t - adj[top.second.second][i];
						np.second.second = i;
						b.push(np);
					}
				}
			}
		}
	}
	return 0;
}