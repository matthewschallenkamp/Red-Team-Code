#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
	int n;
	cin >> n;
	int tmp;
	vector<int> nums(1);
	cin >> nums[0];
	for (int i = 1; i < n; i++)
	{
		cin >> tmp;
		//uniqueify numbers
		if (tmp != nums.back())
			nums.push_back(tmp);
	}
	//smush them
	map<int, int> smush;
	for (auto i : nums)
		smush[i] = 0;
	int id = 0;
	for (auto item : smush) //maps are sorted
		smush[item.first] = id++;

	for(auto &i : nums)
		i = smush[i];

	//get locs
	vector<vector<int> > groups(id);
	for(int i = 0; i < nums.size(); i++)
		groups[nums[i]].push_back(i);

	pair<int, int> b1, b2;
	b1 = {0, id};
	b2 = b1;
	for(int i = 0; i < id; i++)
	{
		//walk through each position
		auto nb1 = b1, nb2 = b2;
		for(auto spot : groups[i])
		{
			//we need to know this can be a pair
			if(spot + 1 == nums.size() || nums[spot + 1] != i + 1)
				continue;

			//add it to whichever pair works better
			pair<int, int> n;
			if(groups[i].size() != 1 && b1.second + 1 == spot)
				n = {b2.first + 1, spot};
			else
				n = {b1.first + 1, spot};

			if(n > nb1){
				nb2 = nb1;
				nb1 = n;
			}
			else if(n > nb2)
			{
				nb2 = n;
			}
		}
		b1 = nb1;
		b2 = nb2;
	}

	cout << nums.size() - 1 - b1.first << endl;
}