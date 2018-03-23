#include <iostream>
#include <vector>

using namespace std;

int main()
{
	string s, bomb;
	cin >> s >> bomb;
	vector<int> state(1, 0);
	string res;
	for(auto chr : s)
	{
		int tocheck = state.back();
		if(chr == bomb[tocheck])
		{
			tocheck++;
			state.push_back(tocheck);
		}
		else if (chr == bomb[0])
		{
			state.push_back(1);
		}
		else
		{
			state.push_back(0);
		}
		res.push_back(chr);
		if(state.back() == bomb.size())
		{
			for(int i = 0; i < bomb.size(); i++)
			{
				state.pop_back();
				res.pop_back();
			}
		}
	}
	if(res.size())
		cout << res << endl;
	else
		cout << "FRULA" << endl;
	return 0;
}
