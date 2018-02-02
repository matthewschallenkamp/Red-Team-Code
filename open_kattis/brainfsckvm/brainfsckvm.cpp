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

struct i3
{
	int pc = 0, dc = 0, ic = 0;
};

int main()
{
	int tc;
	cin >> tc;
	while(tc--)
	{
		int m, c, in;
		cin >> m >> c >> in;
		string prog, put;
		cin >> prog >> put;
		i3 spot;
		vector<char> state(m);
		map<int, int> brackets;

		vector<int> tags(prog.size(), 0);
		for(int i = prog.size() - 2; i >= 0; i--)
		{
			if(prog[i] == prog[i+1])
				tags[i] = tags[i+1] + 1;
		}

		vector<int> b;
		for(int i = 0; i < prog.size(); i++)
		{
			if(prog[i] == '[')
				b.push_back(i);
			else if(prog[i] == ']')
			{
				brackets[i] = b.back();
				brackets[b.back()] = i;
				b.pop_back();
			}
		}

		bool loops = true;
		for(int i = 0; i < 50000000; i++)
		{
			if(spot.pc >= prog.size())
			{
				loops = false;
				break;
			}
			if(prog[spot.pc] == '[')
			{
				if(state[spot.dc] == 0)
					spot.pc = brackets[spot.pc] - 1;
			}
			else if(prog[spot.pc] == ']')
			{
				if(state[spot.dc] != 0)
					spot.pc = brackets[spot.pc] - 1;
			}
			else if(prog[spot.pc] == '>')
			{
				spot.dc = (spot.dc + 1 + tags[spot.pc]) % m;
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '<')
			{
				spot.dc = (m + spot.dc - 1 - tags[spot.pc]) % m;
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '+')
			{
				state[spot.dc]+= 1 + tags[spot.pc];
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '-')
			{
				state[spot.dc]-=1 + tags[spot.pc];
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == ',')
			{
				if(spot.ic < put.size())
					state[spot.dc] = put[spot.ic++];
				else
					state[spot.dc] = 255;
			}
			spot.pc++;
		}

		int maxpc = spot.pc;
		if(loops)
		for(int i = 50000000; i < 100000000; i++)
		{
			if(prog[spot.pc] == '[')
			{
				if(state[spot.dc] == 0)
					spot.pc = brackets[spot.pc] - 1;
			}
			else if(prog[spot.pc] == ']')
			{
				maxpc = max(maxpc, spot.pc);
				if(state[spot.dc] != 0)
					spot.pc = brackets[spot.pc] - 1;
			}
			else if(prog[spot.pc] == '>')
			{
				spot.dc = (spot.dc + 1 + tags[spot.pc]) % m;
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '<')
			{
				spot.dc = (m + spot.dc - 1 - tags[spot.pc]) % m;
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '+')
			{
				state[spot.dc]+= 1 + tags[spot.pc];
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == '-')
			{
				state[spot.dc]-=1 + tags[spot.pc];
				spot.pc += tags[spot.pc];
			}
			else if(prog[spot.pc] == ',')
			{
				if(spot.ic < put.size())
					state[spot.dc] = put[spot.ic++];
				else
					state[spot.dc] = 255;
			}
			spot.pc++;
		}

		if(loops)
		{
			cout << "Loops " << brackets[maxpc] << " " << maxpc << endl;
		}
		else
		{
			cout << "Terminates" << endl;
		}
	}
}