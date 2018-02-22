#include <iostream>
#include <vector>

using namespace std;

//http://www.spoj.com/problems/NDS/
//a variant of increasing subsequence, uses a similar algorithm
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int t, n, l;
	scanf(" %i", &t);
	while(t--)
	{
		scanf(" %i", &n);
		vector<int> nums(n);
		for(int i = 0; i < n; i++)
			scanf(" %i", &nums[i]);
		scanf(" %i", &l);
		vector<int> res(l + 1, -1);
		for(int i = 0; i < n; i++)
		{
			int s = 0, e = l + 2, m;
			while(e - s > 1)
			{
				m = (s + e) / 2;
				if(res[m] >= nums[i] || res[m] == -1)
					e = m;
				else
					s = m;
			}
			//start is the first number less than us
			if(s < l)
				res[s+1] = nums[i];
		}
		printf("%d\n", res.back());
	}
	return 0;
}