#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <array>
#include <bitset>
// #include <multiset>

using namespace std;

//arr[indexes used]unordered_map[indexes]

void superset(const array<int8_t, 26> &l, const array<int8_t, 26> &r, int &comp)
{
	for(int i = 0; i < 26; i++)
	{
		if(l[i] < r[i])
		{
			if(comp == 1)
			{
				comp = 0;
				break;
			}
			comp = -1;
		}
		else if(l[i] > r[i])
		{
			if(comp == -1)
			{
				comp = 0;
				break;
			}
			comp = 1;
		}
	}
}

bool operator<(bitset<26> l, bitset<26> r)
{
	return l.to_ulong() < r.to_ulong();
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	string strs[15005];
	char arr[15];
	int ct = 0;
	while(scanf("%10s", arr) != EOF) strs[ct++] = arr;
	vector<pair<string, int>> newstrs(ct);
	for(int i = 0; i < ct; i++)
	{
		newstrs[i].second = i;
		newstrs[i].first = strs[i];
	}
	for(auto &item : newstrs) sort(item.first.begin(), item.first.end());
	sort(newstrs.begin(), newstrs.end());
	//remove eq strings
	vector<vector<int> > eq(ct);
	int eq_tgt = newstrs[0].second;
	eq[eq_tgt].push_back(newstrs[0].second);
	//do need equal target tests - fails on 3 to end
	for(int i = 1; i < ct; i++)
	{
		if(newstrs[i].first != newstrs[i-1].first)
		{
			eq_tgt = newstrs[i].second;
		}
		eq[eq_tgt].push_back(newstrs[i].second);
		cerr << eq_tgt << " " << newstrs[i].second << endl;
	}
	// sort(strs.begin(), strs.end(), [](string &l, string &r) {return l.size() == r.size() ? l < r : l.size() < r.size();});
	

	//say we do bitset of 10 used
	//then only need to compare if we are 0 masked by their bitset - very fast
	cerr << "read" << endl;
	set<int> maximals;
	vector<array<int8_t, 26> > strcts(ct);
	vector<bitset<26> > used(ct);
	vector<int> size(ct, 0);
	int msize = 0;

	for(int i = 0; i < ct; i++)
	{
		if(!eq[i].size()) continue;
		for(auto item : strs[i]) {
			if(!item) break;
			strcts[i][item - 'a']++;
			used[i].set(item - 'a');
			size[i]++;	
		}
		msize = max(msize, size[i]);
	}
	//dont need max size, although it does help speed
	for(int i = 0; i < ct; i++)
	{
		if(!eq[i].size()) continue;
		if(size[i] == msize)
			maximals.insert(i);
	}	
	for(int i = 0; i < ct; i++)
	{
		if(!eq[i].size()) continue;
		//compare to all given maximals
		vector<int> toerase;
		bool good = true;
		if(size[i] == msize) continue;
		for(auto m : maximals)
		{
			int comp = 0;
			//if we have something they don't, and they have something we dont
			auto o = used[m] | used[i];
			if(used[m] < o && used[i] < o)
			{
				//both have something the others dont
				//skip
				continue;
			}
			if(used[m] < used[i])
				comp = -1;
			if(used[i] < used[m])
				comp = 1;
			superset(strcts[m], strcts[i], comp);
			if(comp == -1)
			{
				toerase.push_back(m);
			}
			if(comp == 1)
			{
				good = false;
				break;
			}
		}
		if(good)
			maximals.insert(i);
		for(auto item : toerase)
			maximals.erase(item);
	}
	vector<int> newm;
	newm.reserve(maximals.size());
	for(auto i2 : maximals)
		for(auto item : eq[i2])
			newm.push_back(item);

	sort(newm.begin(), newm.end(), [&strs] (int l, int r) {return strs[l] < strs[r];});
	for(auto item : newm)

		printf("%s\n", strs[item].c_str());

	return 0;
}