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

double dist(double a, double b, double c, double d)
{
	return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

int main()
{
	double sx, sy, dx, dy;
	cin >> sx >> sy >> dx >> dy;
	int n;
	cin >> n;
	vector<double> xs(n), ys(n), ts(n+1, -1);
	for(int i = 0; i < n; i++)
	{
		cin >> xs[i] >> ys[i];
	}
	// xs.push_back(sx);
	xs.push_back(dx);
	// ys.push_back(sy);
	ys.push_back(dy);

	priority_queue<pair<double, int>> q;
	for(int i = 0; i < ts.size(); i++)
	{
		q.push(make_pair(-(dist(sx, sy, xs[i], ys[i])/5), i));
	}
	while(q.size())
	{
		auto top = q.top();
		q.pop();
		if((int)ts[top.second] != -1 && -top.first > ts[top.second])
			continue;
		else
			ts[top.second] = -top.first;
		// cerr << top.first << " " << top.second << endl;
		if(top.second == ts.size() - 1)
			break;
		for(int i = 0; i < ts.size(); i++)
		{
			//we want to run for < 30, catapult otherwise
			if(top.second != i)
			if(dist(xs[top.second], ys[top.second], xs[i], ys[i]) < 30)
			{
				//run
				q.push(make_pair(-ts[top.second] - (dist(xs[top.second], ys[top.second], xs[i], ys[i])/5), i));
			}
			else
			{
				q.push(make_pair(-ts[top.second] - 2 - (abs(50 - dist(xs[top.second], ys[top.second], xs[i], ys[i]))/5), i));
			}
		}
	}
	cout << ts.back() << endl;
}