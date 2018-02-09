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
typedef long long ll;

vector<bool> sieve(10000005, true);
vector<ll> mpfactor(10000005, 0);

void esieve()
{
	for(ll i = 3; i < 3170; i+=2)
	{
		if(sieve[i])
		{
			for(ll j = i * i; j < 10000000; j += i)
			{
				if(sieve[j])
					mpfactor[j] = i;
				sieve[j] = false;
			}
		}
	}
}

vector<ll> pfactors(ll n)
{
	vector<ll> ret;
	if(n % 2 == 0) ret.push_back(2);
	while(n % 2 == 0) n /= 2;
	while(!sieve[n])
	{
		if(!ret.size() || ret.back() != mpfactor[n])
			ret.push_back(mpfactor[n]);
		n /= mpfactor[n];
	}
	if(n != 1 && ((!ret.size()) || ret.back() != n))
		ret.push_back(n);
	return ret;
}

vector<ll> prevs, nexts, parents;

int main()
{

	esieve();
  for(auto pn : pfactors(1))
	{
		cerr << pn << endl;
	}
  ll n;
  cin >> n;
  vector<ll> arr(n);
  for(ll &x : arr) cin >> x;

  prevs.resize(n, -1);
	nexts.resize(n, n);
	parents.resize(n, -1);

	map<ll, vector<ll> > pnindexes;
	for(ll i = 0; i < n; i++)
	{
		for(auto pn : pfactors(arr[i]))
		{
			pnindexes[pn].push_back(i);
		}
	}

	for(auto il : pnindexes)
	{
		auto &indexlist = il.second;
		for(ll i = 1; i < indexlist.size(); i++)
		{
			prevs[indexlist[i]] = max(prevs[indexlist[i]], indexlist[i-1]);
			nexts[indexlist[i-1]] = min(nexts[indexlist[i-1]], indexlist[i]);
		}
	}

	bool good = true;
	vector<pair<pair<int, int>, int>> stk;
	stk.push_back(make_pair(make_pair(0, n), -1));

#define build_tree(a, b, c) stk.push_back(make_pair(make_pair(a, b), c))

	while(stk.size())
	{
		bool skip = false;
		auto top = stk.back();
		stk.pop_back();
		int si = top.first.first, ei = top.first.second, rt = top.second;
		if(si >= ei)
			continue;
		if(si + 1 == ei)
		{
			parents[si] = rt;
			continue;
		}
		for(ll i = 0; i < (ei - si + 1) / 2; i++)
		{
			if(prevs[si + i] < si && nexts[si + i] >= ei)
			{
				parents[si+i] = rt;
				//recurse and done
				build_tree(si, si + i, si + i);
				build_tree(si + i + 1, ei, si + i);
				skip = true;
				break;
			}
			if(prevs[ei - i - 1] < si && nexts[ei - i - 1] >= ei)
			{

				parents[ei-i-1] = rt;
				//recurse and done
				build_tree(si, ei - i - 1, ei - i - 1);
				build_tree(ei - i, ei, ei - i - 1);
				skip = true;
				break;
			}
		}
		if(skip)
			continue;
		good = false;
		break;
	}

	if(good)
	{
		for(auto item : parents)
		{
			cout << item + 1 << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "impossible" << endl;
	}
	return 0;
}