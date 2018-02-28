#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>

using namespace std;

const int MOD = 10000000;

struct rule
{
	string allow, needs;
	int id;
	rule() {}
	void reverse()
	{
		::reverse(allow.begin(), allow.end());
		::reverse(needs.begin(), needs.end());
	}
};

struct state
{
	string prefix;
	//on move in we do checks
	bitset<15> mask, sets, fmask, fsets;
	int id;
	//map from rulestate -> newrulestate, newstate -> ct
	unordered_map<bitset<15>, unordered_map<bitset<15>, vector<int> > > next_cts;
	state(string s, int iid)
	{
		prefix = s;
		id = iid;
	}
	state() {}
};

//lets introduce the idea of instant. That means that this item is the end of a line and 
//simply applies our sets, then splits. this occurs when we have the same next_cts as the empty state
//and we can default to that

vector<state> build_states(vector<rule> rules, string alphabet)
{
	//for each rule we want to add the appropriate states first
	map<string, state> places;
	places[""] = state("", 0); //we need an empty state
	int id = 1;
	for(auto r : rules)
	{
		for(int i = 1; i <= r.allow.size(); i++)
		{
			auto sub = r.allow.substr(0, i);
			if(places.find(sub) == places.end())
				places[sub] = state(sub, id++);
		}
		for(int i = 1; i <= r.needs.size(); i++)
		{
			auto sub = r.needs.substr(0, i);
			if(places.find(sub) == places.end())
				places[sub] = state(sub, id++);
		}
	}

	// now that we have states in, lets add requirements and sets
	for(auto r : rules)
	{
		//to hit the end of an allow, we need idx
		places[r.needs].mask[r.id] = 1;
		//when we hit the end of the need, we start to allow
		places[r.allow].sets[r.id] = 1;
	}
	//this doesnt cover everything: we may have some larger prefix, say ababab
	//and we need to make sure we contain the requirements and sets of the smaller prefixes
	//such as abab or bab
	//to do so, we will walk each one forward, or-ing with any required and set that is there

	for(auto &place : places)
	{
		string prefix = place.first;
		while(prefix.size())
		{
			prefix.pop_back();
			if(places.find(prefix) != places.end()) //if exists
			{
				places[prefix].fmask |= place.second.mask;
				places[prefix].fsets |= place.second.sets;
			}
		}
	}

	for(auto &place : places)
	{
		string suffix = place.first;
		while(suffix.size())
		{
			suffix = suffix.substr(1);
			if(places.find(suffix) != places.end()) //if exists
			{
				place.second.mask |= places[suffix].mask;
				place.second.sets |= places[suffix].sets;
			}
		}
	}

	//we also need to walk and set any larger suffixes t

	for(auto &place : places)
	{
		place.second.mask.flip();
		place.second.fmask.flip();
	}


	vector<state> states(id);
	for(auto &place : places)
	{
		states[place.second.id] = place.second;
	}

	//now we have actual states, lets find moves
	//we can do this with a dfs
	vector<pair<bitset<15>, int> > spots;
	spots.push_back(make_pair(0, 0));
	while(spots.size())
	{
		auto top = spots.back(); spots.pop_back();
		if(states[top.second].next_cts[top.first].size())
			continue;	
		//ok, so now for the new move. 
		for(auto chr : alphabet)
		{
			auto next = top;
			string new_prefix = states[top.second].prefix; new_prefix.push_back(chr);

			while(1) {
				while(places.find(new_prefix) == places.end())
				{
					new_prefix = new_prefix.substr(1);
				}
				next.second = places[new_prefix].id; //new state we move into
				//we're now at the first place that we might move to.
				//lets check if this place has our interest
				//sets apply on entry
				next.first |= places[new_prefix].sets;
				next.first &= places[new_prefix].mask; // new bitset we move into
				if(new_prefix.size() && (((next.first | places[new_prefix].fsets) & places[new_prefix].fmask) == next.first))
				{
					new_prefix = new_prefix.substr(1);
					continue; //this is uninteresting bc we can't change from here
				}
				else
				{
					//this is our new state, add it to our counts
					states[top.second].next_cts[top.first][next.first].resize(states.size());
					states[top.second].next_cts[top.first][next.first][next.second]++;
					spots.push_back(next);
					break;
				}
			}
		}
	}

	return states;
}

//ok, so trouble is we have a mask and a set to see what we get as our new state, 
// but we still have to do this for every combination of state and rules

//a dfa would make a seperate copy of the transitions for each set of activated rules
//then 


int main()
{
	int k;
	string alphabet;
	string rules;
	cin >> k >> alphabet;
	cin >> rules;
	vector<string> rl, rr;
	for(int i = 0; i < rules.size(); i++)
	{
		rl.push_back(""); rr.push_back("");
		while(i < rules.size() && rules[i] != '>')
			rl.back().push_back(rules[i++]);
		i++;
		while(i < rules.size() && rules[i] != '|')
			rr.back().push_back(rules[i++]);
	}
	vector<rule> r;
	for(int i = 0; i < rr.size(); i++)
	{
		cerr << "rule: " << rl[i] << " > " << rr[i] << endl;
		r.push_back(rule());
		r.back().allow = rl[i];
		r.back().needs = rr[i];
		r.back().id = i;
	}

	vector<state> states = build_states(r, alphabet);

	// for(auto state : states)
	// {
	// 	cerr << state.id << " /" << state.prefix << "/ " << state.sets << " " << state.mask << " |> " << state.fsets << " " << state.fmask << endl;
	// }
	// cerr << endl;

	vector<vector<pair<int, int> > > nexts; //state -> list(newstate, ct), pulled directly from the lists
	vector<int> to_ct;
	unordered_map<bitset<15>, unordered_map<int, int> > statelocs;
	statelocs[0][0] = 0;
	nexts.push_back(vector<pair<int, int>>());
	int sc = 0;
	for(auto state : states)
	{
		for(auto from_b : state.next_cts)
		{
			if(statelocs.find(from_b.first) == statelocs.end() || statelocs[from_b.first].find(state.id) == statelocs[from_b.first].end())
			{
				statelocs[from_b.first][state.id] = nexts.size();
				nexts.push_back(vector<pair<int, int>>());
			}
			if(from_b.first.none())
				to_ct.push_back(statelocs[from_b.first][state.id]);
			sc++;
			for(auto to_b : from_b.second)
			{
				for(int i = 0; i < to_b.second.size(); i++) {
				// for(auto to_s : to_b.second) {
					if(to_b.second[i] != 0) {
						if(statelocs.find(to_b.first) == statelocs.end() || statelocs[to_b.first].find(i) == statelocs[to_b.first].end())
						{
							statelocs[to_b.first][i] = nexts.size();
							nexts.push_back(vector<pair<int, int>>());
						}
						nexts[statelocs[from_b.first][state.id]].push_back(make_pair(statelocs[to_b.first][i], to_b.second[i]));
						// cerr << state.prefix << " " << from_b.first << " gives " << to_s.second << " to " << states[to_s.first].prefix << " " << to_b.first << endl;
					}
				}
			}
		}
	}
	cerr << endl;
	cerr << "total of " << sc << " states " << endl;

	vector<long long> statects(nexts.size(), 0), nstatects(nexts.size(), 0);
	statects[0] = 1;

	for(int i = 0; i < k; i++)
	{
		// cerr << i << endl;
		for(int i = 0; i < nexts.size(); i++)
		{
			for(auto pr : nexts[i])
			{
				nstatects[pr.first] += pr.second * statects[i];
				nstatects[pr.first] %= MOD;
			}
		}

		statects.swap(nstatects);
		nstatects.clear(); nstatects.resize(nexts.size(), 0);
	}

	long long count = 0;
	for(auto item : to_ct) {
		count += statects[item];
		count %= MOD;
	}
	cout << count << endl;

	return 0;
}



//so the optimization is that the dfa has all the different transitions it needs to do per state/rule set
//and atm we just have our masks
// lets say instead we save our transitions per state and rule set inside our states
// then we have essentially created a dfa

//so the plan is to make the table like we described, then we need to minimize all the different possible reachable states
//so whenever we move into a state and we have x or y, meaning that we don't really care about this prefix
//we can pop right out

//for this we need a set of cts for each ruleset/character pair, per state prefix
//then our calc loop becomes
//per len
//per state if non0
//per item in moves
	//add our counts by multiplier

//how to we calc the next state for each ruleset/char pair
//per item, we need the next smallest prefix
//while our cstate is same after masking, shortcut to next prefix
