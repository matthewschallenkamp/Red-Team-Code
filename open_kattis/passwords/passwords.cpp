#include <iostream>
#include <vector>
#include <map>

using namespace std;

char serialize(char in)
{
	if(in == '7') return 't';
	if(in == '5') return 's';
	if(in == '3') return 'e';
	if(in == '1') return 'i';
	if(in == '0') return 'o';
	if(in >= 'A' && in <= 'Z') return (in - 'A') + 'a';
	return in;
}

struct node {
	node() {};
	string prefix = "";
	int idx = -1;
	bool count = true;
	map<char, int> fspots;
};

vector<node> get_moves(vector<string> words)
{
	map<string, node> spots;
	spots[""] = node();
	for(auto str : words)
	{
		while(str.size())
		{
			spots[str].prefix = str;
			str.pop_back();
		}
	}
	int id = 0;
	for(auto &pr : spots)
	{
		pr.second.idx = id++;
	}
	string togo;
	for(char c = '0'; c <= '9'; c++)
	{
		togo.push_back(c);
	}
	for(char c = 'A'; c <= 'Z'; c++)
	{
		togo.push_back(c);
	}
	for(char c = 'a'; c <= 'z'; c++)
	{
		togo.push_back(c);
	}

	for(auto &pr : spots)
	{
		for(auto c : togo) {
			char go = serialize(c);
			string next = pr.second.prefix;
			next.push_back(go);
			while(next.size() && spots.find(next) == spots.end())
				next = next.substr(1);
			pr.second.fspots[c] = spots[next].idx;
		}
	}

	for(auto word : words) {
		spots[word].count = false;
		spots[word].fspots.clear();
	}

	for(auto &pr : spots)
	{
		string next = pr.second.prefix;
		while(next.size())
		{
			next = next.substr(1);
			if(spots.find(next) != spots.end())
			{
				if(spots[next].count == false)
				{
					pr.second.count = false;
					pr.second.fspots.clear();
				}
			}
		}
	}

	

	// for(auto item : spots)
	// {
	// 	cerr << item.second.prefix << " = " << item.first << " "
	// 		<< item.second.idx << endl;
	// 	for(auto pr : item.second.fspots)
	// 		cerr << " " << pr.first << " -> " << pr.second;
	// 	cerr << endl;
	// }

	vector<node> moves(id);
	for(auto pr : spots)
	{
		moves[pr.second.idx] = pr.second;
	}
	return moves;
}

int main()
{
	int slen, llen, n;
	cin >> slen >> llen >> n;
	vector<string> words(n);
	for(int i = 0; i < n; i++)
	{
		cin >> words[i];
	}
	vector<node> moves = get_moves(words);
	vector<vector<long long>> counts(1 << 3, vector<long long>(moves.size(), 0)), ncts;
	ncts = counts;
	for(auto item : moves)
		if(item.prefix == "")
		{
			// cerr << "got null" << endl;
			counts[0][item.idx] = 1;
			break;
		}

	long long tot = 0;
	long long mod = 1000003L;

	for(int i = 1; i <= llen; i++)
	{
		for(int msk = 0; msk < 1 << 3; msk++)
		{
			for(int spot = 0; spot < moves.size(); spot++)
			{
				for(char c = '0'; c <= '9'; c++)
				{
					int nmsk = msk | (1 << 0);
					if(moves[spot].fspots.size())
					{
						ncts[nmsk][moves[spot].fspots[c]] += counts[msk][spot];
						ncts[nmsk][moves[spot].fspots[c]] %= mod;
					}
				}
				for(char c = 'A'; c <= 'Z'; c++)
				{
					int nmsk = msk | (1 << 1);
					if(moves[spot].fspots.size())
					{
						ncts[nmsk][moves[spot].fspots[c]] += counts[msk][spot];
						ncts[nmsk][moves[spot].fspots[c]] %= mod;
					}
				}
				for(char c = 'a'; c <= 'z'; c++)
				{
					int nmsk = msk | (1 << 2);
					if(moves[spot].fspots.size())
					{
						ncts[nmsk][moves[spot].fspots[c]] += counts[msk][spot];
						ncts[nmsk][moves[spot].fspots[c]] %= mod;
					}
				}
			}
		}
		//add counts if applicable
		if(i >= slen) {
			cerr << i << endl;
			for(int spot = 0; spot < moves.size(); spot++)
			{
				if(moves[spot].count)
				{
					tot += ncts[7][spot];	
					tot %= mod;
				}
			}
		}
		counts.swap(ncts);
		for(auto &row : ncts)
			for(auto &item : row)
				item = 0;
	}

	cout << tot % mod << endl;
	return 0;
}