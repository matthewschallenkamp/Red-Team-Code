#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <array>
#include <bitset>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int n;
	vector<int> indexes(10010);
	while(scanf("%i: ", &n) != EOF && n != 0) {
		int here;
		for(int i = 0; i < n; i++) {
			scanf("%i ", &here);
			indexes[here] = i;
		}
		bool good = true;
		for(int step = 1; step < n; step++)
		{
			for(int i = step + step; i < n; i++)
			{
				if((indexes[i] < indexes[i - step]) == (indexes[i - step] < indexes[i - step - step]))
				{
					cerr << i << " " << step << endl;
					good = false;
					break;
				}
			}
		}
		printf(good ? "yes\n" : "no\n");
	}
	return 0;
}