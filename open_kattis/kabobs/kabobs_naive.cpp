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
	bitset<15> mask, sets;
	int id;
	int instant = -1;
	unordered_map<int, int> next_cts;
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
		string suffix = place.first;
		while(suffix.size())
		{
			suffix = suffix.substr(1); // remove first char
			if(places.find(suffix) != places.end()) //if exists
			{
				//add rules
				place.second.mask |= places.find(suffix)->second.mask;
				place.second.sets |= places.find(suffix)->second.sets;
			}
		}
	}

	//now we have actual states, lets find moves
	//per state we will make move and add that to our next_cts
	for(auto &place : places)
	{
		for(auto chr : alphabet)
		{
			string newprefix = place.second.prefix; newprefix.push_back(chr);
			//we need to find the smallest prefix we can travel to
			while(places.find(newprefix) == places.end())
			{
				newprefix = newprefix.substr(1); // remove first
			}
			//add this new place to our moves
			place.second.next_cts[places.find(newprefix)->second.id]++;
		}
	}
	//last thing to do is get any instants we can
	//finally, lets move all these guys into an array
	vector<state> states(id);
	for(auto &place : places)
	{
		place.second.mask.flip();
		states[place.second.id] = place.second;
	}

	for(int i = 0; i < states.size(); i++)
	{
		for(int j = 0; j < states.size(); j++)
		{
			//we can combine these states if they have the same outward moves
			if(i != j && states[i].instant == -1 && states[j].instant == -1)
			{
				auto sets = states[j].sets;
				sets.flip();
				auto mask = states[i].mask;
				mask.flip();
				//we want to mask all the bits they do
				//compute by xoring them
				if(states[i].next_cts == states[j].next_cts &&
					(states[i].sets & sets).none() && (mask & states[j].mask).none())//we end up setting the samae states after the masks.
				{
					states[j].instant = i;
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

	for(auto state : states)
	{
		cerr << state.instant << " /" << state.prefix << "/ " << state.sets << " " << state.mask << " ";
	}
	cerr << endl;

	//now we need to run moves 500 times
	int ssize = 1 << r.size();
	vector< unordered_map<bitset<15>, int> > statects(states.size()), nstatects(states.size());
	statects[0][0] = 1;

	for(int i = 0; i < k; i++)
	{
		cerr << i << endl;
		//step the states and put cts into the nstatects, if good
		for(int j = 0; j < states.size(); j++)
		{
			for(const auto &pr : statects[j])
			{			
				// cerr << scs.first << " " << j << " " << scs.second[j] << endl;
				auto &st = states[j];
				// cerr << st.sets << " " << st.required << " " << st.prefix << " " << st.id << endl;
				for(auto m : st.next_cts)
				{
					pair<int, int> move = m;
					auto nallow = ((pr.first | st.sets) & st.mask); //get the new mask to check allowed
					if(states[move.first].instant != -1)
					{
						nallow = ((nallow | states[move.first].sets) & states[move.first].mask);
						move.first = states[move.first].instant;
					}
					nstatects[move.first][nallow] += move.second * pr.second;
					nstatects[move.first][nallow] %= MOD;
				}			
			}
		}
		statects.swap(nstatects);
		for(auto &item : nstatects) item.clear();
	}

	int count = 0;
	for(int j = 0; j < states.size(); j++)
	{
		for(auto ct : statects[j])
		{
			cerr << states[j].instant << " " << j << " " << ct.first << " " << ct.second << endl;
			//since we are here, this state is allowed
			//sum it
			if(((ct.first | states[j].sets) & states[j].mask).none()) {
				count += ct.second;
				count %= MOD;
			}
		}
	}
	cout << count << endl;

	return 0;
}