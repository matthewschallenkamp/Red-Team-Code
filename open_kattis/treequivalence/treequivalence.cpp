#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef int ll;

vector<ll> as, bs, ts;

struct node
{
	char name;
	int subsize;
	deque<node> children;
};

node build_tree(string &tbase)
{
	node n;
	if(tbase.size())
	{
		n.name = tbase.back(); //trees start with a letter
		tbase.pop_back(); // letter
	}
	int sum = 0;
	if(tbase.size() && tbase.back() == '(')
	{
		//there is a subtree
		tbase.pop_back(); // (
		while(tbase.size() && 'A' <= tbase.back() && 'Z' >= tbase.back())
		{
			n.children.push_back(build_tree(tbase));
			sum += 1 + n.children.back().subsize;
			if(tbase.size() && tbase.back() == ',')
				tbase.pop_back(); // ,
		}
		if(tbase.size())
			tbase.pop_back(); // )
	}
	n.subsize = sum;
	return n;
}

int tc(const node &n)
{
	int sum = 1;
	for(auto &child : n.children)
		sum += tc(child);
	return sum;
}

deque<pair<char, int>> make_path(const node &n, int size = -1)
{
	if(size == -1)
		size = tc(n);
	//so we know that the trees are ordered clockwise
	//if we start making a path around them,
	//at least one of the rotations have to be equal
	//we need to include this node once for each of its children2
	//and once for it's parent
	//root has no parent, so we'll remove the last node after
	deque<pair<char, int> > path;
	int sum = 0;
	for(auto &child : n.children)
	{
		auto sub = make_path(child, size);
		path.push_back(make_pair(n.name, (2 + 2 * child.subsize)));
		sum += 2 + 2 * child.subsize;
		for(auto c : sub)
			path.push_back(c);
	}
	path.push_back(make_pair(n.name, (2 * size - 2) - sum));
	return path;
}

int main()
{
	int tc;
	cin >> tc;
	while(tc--)
	{
		string a, b;
		cin >> a >> b;
		reverse(a.begin(), a.end());
		reverse(b.begin(), b.end());
		deque<pair<char, int> > ap = make_path(build_tree(a)), bp = make_path(build_tree(b));
		bool same = false;
		if(ap == bp)
			same = true;
		ap.pop_back();
		bp.pop_back();

		// for(auto item : ap)
		// 	cerr << item.first << " " << item.second << " ";
		// cerr << endl;
		// for(auto item : bp)
		// 	cerr << item.first << " " << item.second << " ";
		// cerr << endl;
		//these may be up to 200 chars 2x graph size ~= 200
		//all rotations should be find
		if(ap.size() == bp.size())
		for(int i = 0; i < bp.size(); i++)
		{
			//walk for equals, with map from node id to node id
			if(ap == bp)
			{
				same = true;
				break;
			}
			bp.push_back(bp.front());
			bp.pop_front();
		}

		if(same)
			cout << "same" <<  endl;
		else
			cout << "different" << endl;
	}
	return 0;
}
