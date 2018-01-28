#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <deque>

using namespace std;

int gzx, gzy;
const int inf = 100000;

void f(vector<vector<int>> &city, int l, int h, deque<pair<int, int> > flood)
{
	//basic flood fill. Pulled out for reuse
	while (!flood.empty())
	{
		auto top = flood.front();
		flood.pop_front();
		for (auto off : { -1, 1})
		{
			if (0 <= off + top.first && off + top.first < h &&
			    city[top.first][top.second] + 1 < city[off + top.first][top.second])
			{
				city[off + top.first][top.second] = 1 + city[top.first][top.second];
				flood.push_back(make_pair(off + top.first, top.second));
			}
			if (0 <= off + top.second && off + top.second < l &&
			    city[top.first][top.second] + 1 < city[top.first][off + top.second])
			{
				city[top.first][off + top.second] = 1 + city[top.first][top.second];
				flood.push_back(make_pair(top.first, off + top.second));
			}
		}
	}
}


int destruction(vector<vector<int>> &city, int l, int h)
{
	//first we need to flood fill from each of the mechs
	deque<pair<int, int> > flood;
	auto visited = city;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < l; j++)
		{
			if (city[i][j] == 0)
			{
				flood.push_back(make_pair(i, j));
			}
			visited[i][j] = 0;
		}
	}
	f(city, l, h, flood);

	//now, start moving godzilla
	int steps = 0;
	int destroyed = 0;

	visited[gzx][gzy] = 1;
	while (1)
	{
		//move according to the steps
		//towards cities first, nesw
		if (gzx - 1 >= 0 && city[gzx - 1][gzy] == -2)
		{
			city[gzx - 1][gzy] = 100000;
			destroyed++;
			gzx--;
		}
		else if (gzy + 1 < l && city[gzx][gzy + 1] == -2)
		{
			city[gzx][gzy + 1] = 100000;
			destroyed++;
			gzy++;
		}
		else if (gzx + 1 < h && city[gzx + 1][gzy] == -2)
		{
			city[gzx + 1][gzy] = 100000;
			destroyed++;
			gzx++;
		}
		else if (gzy - 1 >= 0 && city[gzx][gzy - 1] == -2)
		{
			city[gzx][gzy - 1] = 100000;
			destroyed++;
			gzy--;
		}
		//then other places, nesw
		else if (gzx - 1 >= 0 && !visited[gzx - 1][gzy])
		{
			gzx--;
		}
		else if (gzy + 1 < l && !visited[gzx][gzy + 1])
		{
			gzy++;
		}
		else if (gzx + 1 < h && !visited[gzx + 1][gzy])
		{
			gzx++;
		}
		else if (gzy - 1 >= 0 && !visited[gzx][gzy - 1])
		{
			gzy--;
		}
		visited[gzx][gzy] = 1;

		//godzilla may have just destroyed a city, so we want to continue our flood fill from the squares next to him
		flood.clear();
		for (auto off : { -1, 1})
		{
			if (0 <= off + gzx && off + gzx < h &&
			    -2 != city[off + gzx][gzy])
			{
				flood.push_back(make_pair(off + gzx, gzy));
			}
			if (0 <= off + gzy && off + gzy < l &&
			    -2 != city[gzx][off + gzy])
			{
				flood.push_back(make_pair(gzx, off + gzy));
			}
		}
		f(city, l, h, flood);

		//finally, check if we are in the range of any weapons.
		steps++;
		for (int i = gzx; i < h; i++)
		{
			if (city[i][gzy] == -2 || city[i][gzy] == inf)
				break;
			if (city[i][gzy] <= steps)
				return destroyed;
		}
		for (int i = gzx; i >= 0; i--)
		{
			if (city[i][gzy] == -2 || city[i][gzy] == inf)
				break;
			if (city[i][gzy] <= steps)
				return destroyed;
		}
		for (int j = gzy; j < l; j++)
		{
			if (city[gzx][j] == -2 || city[gzx][j] == inf)
				break;
			if (city[gzx][j] <= steps)
				return destroyed;
		}
		for (int j = gzy; j >= 0; j--)
		{
			if (city[gzx][j] == -2 || city[gzx][j] == inf)
				break;
			if (city[gzx][j] <= steps)
				return destroyed;
		}
	}
	//we are guaranteed that godzilla will be caught
}

int main()
{
	int tc;
	cin >> tc;
	while (tc--)
	{
		int l, h;
		string trash;
		char temp;
		cin >> l >> h;
		vector<vector<int> > city(h);

		for (int i = 0; i < h; i++)
		{
			city[i].resize(l);
			for (int j = 0; j < l; j++)
			{
				cin >> temp;
				if (temp == 'G')
				{
					gzx = i;
					gzy = j;
					city[i][j] = inf; //mechs can move onto godzilla, he never starts at a city
				}
				else if (temp == 'R')
				{
					city[i][j] = -2;
				}
				else if (temp == 'M')
				{
					city[i][j] = 0;
				}
				else if (temp == '.')
				{
					city[i][j] = inf;
				}
			}
		}
		cout << destruction(city, l, h) << endl;
	}
}
