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
};

struct state
{
	string prefix;
	//on move in we do checks
	int id;
	unordered_map<char, int> transitions;
	vector<pair<char, int> > t2;
	int td;
	void finish()
	{
		for(auto item : transitions)
			t2.push_back(item);
		td = transitions['*'];
	}
	bool good;
	state(string s, int iid, bool ggood)
	{
		prefix = s;
		id = iid;
		good = ggood;
	}
	state() {}
};

vector<state> build_sub_dfa(rule r, string a)
{

	//for each rule we want to add the appropriate states first
	vector<state> places(r.allow.size() + r.needs.size());
	places[0] = state("", 0, true); //we need an empty state to start from
	places[0].transitions['*'] = 0;
	int id = 1;
	for(int i = 1; i < r.allow.size(); i++, id++)
	{
		auto sub = r.allow.substr(0, i);
		places[id] = state(sub, id, true);
		//default transition is to 0
		places[id].transitions['*'] = 0;
		for(auto item : a)
		{
			//add to prefix, then walk back for match - it must be the same length
			auto nsub = sub; nsub.push_back(item); nsub = nsub.substr(1);
			while(places[nsub.size()].prefix != nsub)
				nsub = nsub.substr(1);
			if(nsub.size() != 0)
				places[id].transitions[item] = nsub.size();
		}
	}

	auto sub = r.allow;
	places[id] = state(sub, id, false);
	//default transition is to 0
	places[id].transitions['*'] = id;
	//we might have a partial of ourself on certain items, but that is ignored because we've already got a copy
	id++;

	int off = r.allow.size();
	for(int i = 1; i < r.needs.size(); i++, id++)
	{
		auto sub = r.needs.substr(0, i);
		places[id] = state(sub, id, false);
		places[id].transitions['*'] = r.allow.size(); //point to the finished allow by default
		for(auto item : a)
		{
			//add to prefix, then walk back for match - it must be the same length
			auto nsub = sub; nsub.push_back(item); nsub = nsub.substr(1);
			while(nsub.size() && places[off + nsub.size()].prefix != nsub)
				nsub = nsub.substr(1);
			if(nsub.size() != 0)
				places[id].transitions[item] = off + nsub.size();
		}	
	}



	//now lets add the forward edges
	for(int i = 0; i < r.allow.size(); i++, id++)
	{
		places[i].transitions[r.allow[i]] = i + 1;
	}

	for(int i = 0; i < r.needs.size(); i++, id++)
	{
		places[off + i].transitions[r.needs[i]] = off + i + 1;
	}

	//connect the need back to the start state
	places.back().transitions[r.needs.back()] = 0;

	return places;
}

int get_move(state &st, char letter)
{
	if(st.transitions.find(letter) != st.transitions.end())
		return st.transitions[letter];
	else return st.transitions['*'];
}

vector<state> combine_and(vector<state> &ldfa, vector<state> &rdfa, string alphabet)
{
	//we will use a 2d array to mark the naive possible states, then do a bfs from the start
	//combining states as we go. each state we meet will recieve updated transitions, id, and good 
	//based on the two states it is composed of.
	vector<pair<int, int>> dfs;
	dfs.push_back(make_pair(0, 0));
	vector<state> ndfa(1);
	ndfa[0].transitions['*'] = 0;
	vector<vector<int> > names;
	names.resize(ldfa.size(), vector<int>(rdfa.size(), -1));
	names[0][0] = 0;
	while(dfs.size())
	{
		auto top = dfs.back();
		dfs.pop_back();
		//this is a new item, we need to update it
		auto spot = names[top.first][top.second];
		ndfa[spot].id = spot;
		ndfa[spot].good = ldfa[top.first].good && rdfa[top.second].good;
		// ndfa[spot].prefix = ldfa[top.first].prefix + "+" + rdfa[top.second].prefix;
		//add the default transition
		pair<int, int> nm = make_pair(get_move(ldfa[top.first], '*'), get_move(rdfa[top.second], '*'));	
		if(names[nm.first][nm.second] == -1)
		{
			names[nm.first][nm.second] = ndfa.size();
			ndfa.push_back(state());
			dfs.push_back(nm);
		}
		ndfa[spot].transitions['*'] = names[nm.first][nm.second];
		//and all the others
		for(auto chr : alphabet)
		{
			nm = make_pair(get_move(ldfa[top.first], chr), get_move(rdfa[top.second], chr));
			if(names[nm.first][nm.second] == -1)
			{
				names[nm.first][nm.second] = ndfa.size();
				ndfa.push_back(state());
				dfs.push_back(nm);
			}
			if(names[nm.first][nm.second] != ndfa[spot].transitions['*'])
				ndfa[spot].transitions[chr] = names[nm.first][nm.second];
		}
	}
	return ndfa;
}

vector<state> build_dfa(vector<rule> &rules, string alphabet)
{
	vector<vector<state> > subdfas(rules.size() * 2);
	for(int i = 0; i < rules.size(); i++)
	{
		subdfas[rules.size() + i] = build_sub_dfa(rules[i], alphabet);
	}

	for(int i = rules.size() - 1; i > 0; i--)
	{

		// cerr << "dfa: " << i << endl << endl;
		// for(auto item : dfa)
		// {
		// 	cerr << "/" << item.prefix << "/ " << item.good << " " << item.id << endl;
		// 	for(auto item2 : item.transitions)
		// 		cerr << " " << item2.first << " " << item2.second << endl;
		// }

		// cerr << "subdfa: " << i << " " << rules[i].allow << " " << rules[i].needs << endl << endl;
		// for(auto item : subdfas[i])
		// {
		// 	cerr << "/" << item.prefix << "/ " << item.good << " " << item.id << endl;
		// 	for(auto item2 : item.transitions)
		// 		cerr << " " << item2.first << " " << item2.second << endl;
		// }

		subdfas[i] = combine_and(subdfas[2*i], subdfas[2*i+1], alphabet);
	}

	// cerr << "dfa final: " << endl << endl;
	// for(auto item : subdfas[1])
	// {
	// 	cerr << "/" << item.prefix << "/ " << item.good << " " << item.id << endl;
	// 	for(auto item2 : item.transitions)
	// 		cerr << " " << item2.first << " " << item2.second << endl;
	// }

	return subdfas[1];
}

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
		// cerr << "rule: " << rl[i] << " > " << rr[i] << endl;
		r.push_back(rule());
		r.back().allow = rl[i];
		r.back().needs = rr[i];
		r.back().id = i;
	}

	vector<state> dfa = build_dfa(r, alphabet);
	for(auto &item : dfa)
		item.finish();
	vector<long long> statects(dfa.size(), 0), nstatects(dfa.size(), 0);
	statects[0] = 1;
	cerr << dfa.size() << endl;

	for(int ki = 0; ki < k; ki++)
	{
		// cerr << ki << endl;
		for(int i = 0; i < dfa.size(); i++)
		{
			for(const auto &move : dfa[i].transitions)
			{
				nstatects[move.second] += statects[i];
			}
			//add the extra for defaults. We've already added 1 copy
			nstatects[dfa[i].td] += statects[i] * (alphabet.size() - dfa[i].t2.size());
		}
		statects.swap(nstatects);
		for(auto &item : statects)
			item %= MOD;
		for(auto &item : nstatects)
			item = 0;
	}

	int count = 0;
	for(int i = 0; i < dfa.size(); i++)
	{
		if(dfa[i].good)
		{
			count += statects[i];
			count %= MOD;
		}
	}
	cout << count << endl;

	return 0;
}