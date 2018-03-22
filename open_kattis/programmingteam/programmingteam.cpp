#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

vector<int> s, p, r;
vector<vector<int> > subs;
int n, k;

double oo =-1e100;

vector<vector<double> > scores;
vector<int> sz;

int score(int idx, double price) {
	//we are given that we take this item
	//lets find the best scores for each k that we care about by maximizing
	scores[idx].resize(min(sz[idx], k) + 1, oo);
	fill(scores[idx].begin(), scores[idx].end(), oo);
	for(auto &item : scores[idx]) item = oo;
	scores[idx][1] = p[idx] - (price * s[idx]);
	//we only need to calculate the items within our min/max range. This means that 
	//we want to take each sub size and check it against our own and k and n
	//min needed from subtree is max we can go without -> -n + k + ssize
	//max is either k or ssize
	int ct = 1;
	for(auto sub : subs[idx]) {
		score(sub, price);
		int mn = max(sz[sub] - n + k, 1); //max we can go without from that subtree
		int mx = min(k, sz[sub]);
		// cerr << mn << " " << mx << endl;
		for(int si = ct; si >= 1; si--)
		{
			//we want first length we can actually consider at this point
			//so whatever reaches
			for(int i = mn; si + i <= min(k, sz[idx]) && i <= mx; i++)
			{
				scores[idx][si + i] = max(scores[idx][si + i], scores[idx][si] + scores[sub][i]);
			}
		}
		ct += sz[sub];
		ct = min(ct, k);
	}
	// cerr << idx << " " << price << endl;
	// for(auto item : scores[idx]) {
	// 	cerr << " " << item;
	// }
	// cerr << endl;
	return scores[idx].size();
}

bool possible(double price) {
	score(0, price);
	return scores[0][k] > 0.0;
}

int main() {
	cin >> k >> n;
	k++; n++;
	s.resize(n, 0);
	p = r = s;
	subs.resize(n);
	scores.resize(n);	
	sz.resize(n, 1);
	for(auto item : scores) {
		item.resize(k + 1);
	}
	for(int i = 1; i < n; i++)
	{
		cin >> s[i] >> p[i] >> r[i];
		subs[r[i]].push_back(i);
	}
	for(int i = n - 1; i > 0; i--)
	{
		sz[r[i]] += sz[i];
	}
	double low = 0, hi = 1;
	while(possible(hi)) {
		low = hi;
		hi *= 2;
	}
	while(hi - low > 1e-5) {
		double mid = (hi + low) / 2;
		if(possible(mid)) {
			low = mid;
		} else {
			hi = mid;
		}
	}
	cout << fixed << setprecision(3) << low << endl;
}
