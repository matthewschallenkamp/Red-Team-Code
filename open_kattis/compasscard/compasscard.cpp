#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

typedef long long ll;

struct s
{
	int c[3], id, u;
	s() {}
	s(int rr, int gg, int bb, int iid, int uu = 0): id(iid), u(uu) {
		c[0] = rr;
		c[1] = gg;
		c[2] = bb;
	}
};

bool operator<(s l, s r)
{
	return make_pair(l.u, -l.id) < make_pair(r.u, -r.id);
}

vector<vector<set<int>>> cs(3);
set<s> cards2;
map<int, int> locs;
vector<s> cards;

void recalc_uniqueness(s card)
{
	cards2.erase(card);
	int uniq = 0;
	for(int j = 0; j < 3; j++)
	{
		cs[j][card.c[j]].erase(card.id);
		for(int i = card.c[j]; i != (card.c[j] + 359) % 360; i = (i + 1) % 360)
		{
			if(cs[j][i].size())
				break;
			uniq++;
		}
		for(int i = card.c[j]; i != (card.c[j] + 1) % 360; i = (i + 359) % 360)
		{
			if(cs[j][i].size())
				break;
			uniq++;
		}
		cs[j][card.c[j]].insert(card.id);
	}
	card.u = uniq;
	cards[locs[card.id]] = card;
	cards2.insert(card);
}

void recalc_all_from(s card)
{
	for(int j = 0; j < 3; j++)
	{
		for(int i = card.c[j]; i != (card.c[j] + 359) % 360; i = (i + 1) % 360)
		{
			if(cs[j][i].size())
			{
				recalc_uniqueness(cards[locs[*cs[j][i].begin()]]);
				break;
			}
		}
		for(int i = card.c[j]; i != (card.c[j] + 1) % 360; i = (i + 359) % 360)
		{
			if(cs[j][i].size())
			{
				recalc_uniqueness(cards[locs[*cs[j][i].begin()]]);
				break;
			}
		}
	}
}


int main()
{
	int n;
	cin >> n;
	cards.resize(n);
	int r, g, b, id;
	for(int i = 0; i < n; i++)
	{
		cin >> r >> g >> b >> id;
		cards[i] = s(r, g, b, id);
		locs[id] = i;
	}

	for(int j = 0; j < 3; j++)
		cs[j].resize(360);

	for(auto &item : cards)
	{
		for(int j = 0; j < 3; j++)
			cs[j][item.c[j]].insert(item.id);
	}

	for(auto &item : cards)
	{
		cards2.insert(item);
	}

	for(auto &item : cards)
	{
		recalc_uniqueness(item);
	}

	while(cards2.size())
	{
		s item = *(cards2.begin());
		cards2.erase(cards2.begin());
		//remove me from each of the 6 spots
		//then walk and recalc the nexts
		
		for(int j = 0; j < 3; j++)
			cs[j][item.c[j]].erase(item.id);

		recalc_all_from(item);
		cout << item.id << "\n";
	}

	return 0;
}