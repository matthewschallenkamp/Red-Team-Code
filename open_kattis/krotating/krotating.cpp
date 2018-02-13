#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct segment
{
	// our recurrence relation is forward = l->forward[r->forward]
	// because each rotation takes us to the old location of the next item, 
	// which has to be found in the earlier time.
	unordered_map<int, int> forward, back;
	segment *l = nullptr, *r = nullptr;
	int s, e, m;
	segment(int start, int end)
	{
		s = start;
		e = end;
		//build subtrees. left will have half to middle, other will have rest
		//they always start empty
		//e is 1 past the end, so we are guaranteed that this splits correctly
		if(s + 1 != e)
		{
			m = (s + e)/2;
			l = new segment(s, m);
			r = new segment(m, e);
		}
	}
	int query(int week, int teacher)
	{
		if(week < s)
		{
			//we do nothing
			return teacher;
		}
		else if(week >= e - 1) // this whole set should be applied
		{
			if(forward.find(teacher) != forward.end())
			{
				return forward[teacher];
			}
			else
			{
				return teacher;
			}
		}
		else // we cant apply our whole range, so do the subtrees
		{
			return l->query(week, r->query(week, teacher));
		}
	}
	vector<int> update(int week, vector<int> items)
	{
		if(week == s && s == e-1)
		{
			//this is the spot
			for(int i = 1; i < items.size(); i++)
			{
				forward[items[i-1]] = items[i];
				back[items[i]] = items[i-1];
			}
			forward[items.back()] = items.front();
			back[items.front()] = items.back();
			//now propagate upwards by returning the start indexes that changed
			return items;
		}
		else if(week < m)
		{
			//we need to update the left subtree
			auto changes = l->update(week, items);
			//and recombine based on the indexes that changed
			//the indexes into l that changed are changes
			//we need to find the inputs to right that come to that spot
			//so we need to get the backs of the r to each change
			vector<int> nc;
			for(auto item : changes)
			{
				if(r->back.find(item) != r->back.end())
					nc.push_back(r->back[item]);
				else
					nc.push_back(item);
			}
			//and then update them
			for(int i = 0; i < nc.size(); i++)
			{
				forward[nc[i]] = l->forward[changes[i]];
				back[l->forward[changes[i]]] = nc[i];
			}
			return nc;
		}
		else
		{
			//this is easier than the other case - the changes are the the imputs we want to use
			auto changes = r->update(week, items);
			for(auto item : changes)
			{
				int spot = r->forward[item];
				if(l->forward.find(spot) != l->forward.end())
				{
					forward[item] = l->forward[spot];
					back[l->forward[spot]] = item;
				}
				else
				{
					forward[item] = spot;
					back[spot] = item;
				}
			}
			return changes;
		}
	}
};

int main()
{
	int n, m, q;
	cin >> n >> m >> q;
	segment tree(0, m);
	for(int i = 0; i < q; i++)
	{
		int type;
		cin >> type;
		if(type == 0)
		{
			int k, x;
			cin >> k >> x;
			vector<int> temp(k);
			for(int j = 0; j < k; j++)
			{
				cin >> temp[j];
			}
			tree.update(x - 1, temp);
		}
		else
		{
			int d, x;
			cin >> d >> x;
			cout << tree.query(x - 1, d) << endl;
		}
	}
}