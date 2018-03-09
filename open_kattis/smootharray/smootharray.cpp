#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int minchange(vector<unordered_map<int, int> > &numcts, int s, int loc, bool any)
{
	static int memo[2][5000][5000] = {};
	if(s < 0) return -1;
	if(loc == numcts.size())
	{
		if(s == 0)
			return 0;
		if(any)
			return 0;
		return -1;
	}
	if(memo[any][s][loc] != 0)
		return memo[any][s][loc] - 1;
	int sum = 0;
	for(auto item : numcts[loc])
	{
		sum += item.second;
	}
	int best = -1;
	int next = minchange(numcts, s - 0, loc + 1, true); //always not -1
	best = sum + next;
	for(auto item : numcts[loc])
	{
		next = minchange(numcts, s - item.first, loc + 1, any);
		if(next != -1)
		{
			next = sum - item.second + next;
			best = min(best, next);
		}
	}
	memo[any][s][loc] = best + 1;
	return best;
}

int main()
{
	int n, k, s, t;
	cin >> n >> k >> s;
	vector<unordered_map<int, int> > numcts(k);
	for(int i = 0; i < n; i++)
	{
		cin >> t;
		numcts[i % k][t]++;
	}

	cout << minchange(numcts, s, 0, false) << endl;

	return 0;
}