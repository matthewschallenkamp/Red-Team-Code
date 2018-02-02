/*
 * Matthew Schallenkamp
 * Puce Team
 */

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <string>

using namespace std;

int log10(int n)
{
	int i = 0;
	while(n)
	{
		n /= 10;
		i++;
	}
	return i;
}

int len(int opener, int closer)
{
	return 2+log10(opener)+log10(closer);
}

int main()
{
	string line;
	cin >> line;

	//these will store our colon location estimates
	vector<int> colonlocs, oldlocs;
	//this will store the currently open brackets, for the purpose of closing them in the correct order
	vector<int> stack;
	//this will store the ending bracket, indexed by the opening bracket
	//we make this large, but we will not need all of the space.
	vector<int> closer(line.size());
	//this will store the current colon number. which is unique per open bracket
	int colonnum = 0;

	int closer_index;
	for(int i = 0; i < line.size(); i++)
	{
		//if this is a new colon, increment the colonnum and add it to the stack
		if(line[i] == '('){
			colonnum++;
			stack.push_back(colonnum);
		}
		//if this is a closing parenthesis, set the to of the corresponding opener
		//to the current colon and remove from the stack
		if(line[i] == ')')
		{
			closer_index = stack.back();
			stack.pop_back();
			closer[closer_index] = colonnum;
		}
	}
	//shrink our to vector to how large we actually need
	closer.resize(colonnum+1);
	//set up our colon locations to be all 4 (the smallest possible)
	colonlocs.resize(closer.size(),4);



	bool done = 0;
	//until we have a consistent estimation
	while(!done)
	{
		int loc = 0;
		//reestimate all of our colon locations in order
		for(int i = 1; i < colonlocs.size(); i++)
		{
			loc += len(colonlocs[i], colonlocs[closer[i]]);
			colonlocs[i] = loc;
		}
		//check if the estimate is consistent
		done = (oldlocs == colonlocs);
		oldlocs = colonlocs;
	}

	//output the consistent estimation
	for(int i = 1; i < colonlocs.size(); i++)
	{
		cout << colonlocs[i] << "," << colonlocs[closer[i]] << ":";
	}
	cout << endl;

	return 0;
}
