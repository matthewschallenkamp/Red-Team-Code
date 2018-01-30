#include<iostream>
#include<vector>
#include<queue>
#include<cstdio>
#include<limits>
#include<utility>

using namespace std;

vector<vector<int>> dists;
vector<bool> visited;


int dfs(int spot, int air)
{
	//to prevent partial paths, by default best starts negative if
	int best = -10;
	if (air < 0) return best;
	//special case returning to 0 - no treasures and we complete the path
	// bumping our base best up to 0
	if (spot != 0)
	{
		if (air - dists[spot][0] >= 0)
		{
			best = max(best, 0);
		}
	}
	//try to visit each neighbor
	for (int i = 1; i < visited.size(); i++)
	{
		if (!visited[i])
		{
			visited[i] = true;
			best = max(best, 1 + dfs(i, air - dists[i][spot]));
			visited[i] = false;
		}
	}
	return best;
}

vector<int> dijkstra(int start, vector<vector<pair<int, int>>>& graph, vector<int>& treasure)
{
	priority_queue<pair<int, int> > q;//-edge weight, node so that it sorts small weights first
	vector<int> minDist(graph.size(), 1e9);
	minDist[start] = 0;
	q.push({0, start});
	while (!q.empty()) {

		int node = q.top().second;
		int oldw = -q.top().first;
		q.pop();
		if (oldw > minDist[node])
			continue;
		else
			minDist[node] = oldw;
		for (auto &p : graph[node]) {

			int nodeTo = p.first;
			int weight = p.second;
			int &temp = minDist[nodeTo];
			if (temp > weight + minDist[node]) {
				temp = weight + minDist[node];
				q.push({ -minDist[node] - weight, nodeTo});
			}
		}
	}

	vector<int> res;
	res.push_back(minDist[0]);
	for (auto item : treasure)
	{
		res.push_back(minDist[item]);
	}
	return res;
}

void doCase()
{
	int n, m;
	cin >> n >> m;
	cin.ignore(numeric_limits<int>::max(), '\n');

	int a, b, t;
	vector<vector<pair<int, int>>> graph(n);
	for (int i = 0; i < m; i++)
	{
		scanf("%d %d %d ", &a, &b, &t);
		graph[a].emplace_back(b, t);
		graph[b].emplace_back(a, t);
	}

	// we want to ignore treasures at 0, as they pose problems for our treasure detection
	// and they are easy to remove anyways
	cin >> t;
	vector<int> treasures;
	int otreasures = 0;
	for (int i = 0; i < t; i++)
	{
		int x;
		cin >> x;
		if (x == 0)
			otreasures++;
		else
			treasures.push_back(x);
	}


	dists.clear();
	dists.resize(1);
	for (int& x : treasures)
	{
		//dijkstras returns a list of the path lengths to 0 and our treasures
		dists.push_back(dijkstra(x, graph, treasures));
	}

	visited.clear();
	visited.resize(treasures.size() + 1, false);
	int air;
	cin >> air;
	if (air == 0 || treasures.size() == 0)
		cout << otreasures << endl;
	else
		cout << otreasures + max(0, dfs(0, air)) << endl;

}

int main()
{
	int t;
	cin >> t;

	for (int i = 0; i < t; i++)
	{
		doCase();
	}
}