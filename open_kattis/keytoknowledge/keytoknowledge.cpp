#include <queue>
#include <iostream>
#include <vector>
#include <set>
#include <cstdio>
#include <map>

using namespace std;


typedef unsigned long long ull;
typedef long long ll;


void possible_sums(map<ll, pair<int, ll>> &sums, vector<ll> &spots)
{
  sums[0] = make_pair(1, 0);
  vector<ll> d1, d2, d3;
  for(auto item : spots)
  {
    d1.clear();
    d2.clear();
    d3.clear();
    auto rit = sums.rbegin();
    while(rit != sums.rend())
    {
      if(sums.find(item + rit->first) != sums.end())
      {
        //update instead of insert
        d1.push_back(item + rit->first);
        d2.push_back(rit->second.first + sums[item + rit->first].first);
        d3.push_back(sums[item + rit->first].second);
      }
      else
      {
        d1.push_back(item + rit->first);
        d2.push_back(rit->second.first);
        d3.push_back(rit->first);
      }
      rit++;
    }
    for(int i = 0; i < d1.size(); i++)
    {
      sums[d1[i]] = make_pair(d2[i], d3[i]);
    }
  }
}

void doCase()
{
    int n, m;
    cin >> n >> m;
    vector<string> answers(n);
    vector<int> correct(n);
    for(int i = 0; i < n; i++)
    {
      cin >> answers[i] >> correct[i];
    }
    vector<ll> ss;
    //we are going to score each student by powers of 2
    ll s = 1;
    for(int i = 0; i < n; i++)
    {
      ss.push_back(s);
      s *= 32;
    }

    //and add in their scores to the bit sums
    vector<ll> s0(m), s1(m);
    for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < m; j++)
      {
        if(answers[i][j] == '0')
          s0[j] += ss[i];
        else
          s1[j] += ss[i];
      }
    }

    ll totsum = 0;
    for(int i = 0; i < n; i++)
    {
      totsum += correct[i] * ss[i];
    }

    int half = m / 2;

    vector<ll> tl, th;
    for(int j = 0; j < m / 2; j++)
    {
      tl.push_back(abs(s0[j] - s1[j]));
      totsum -= min(s0[j], s1[j]);
    }
    for(int j = m / 2; j < m; j++)
    {
      th.push_back(abs(s0[j] - s1[j]));
      totsum -= min(s0[j], s1[j]);
    }

    //t[i] is now our list of items;

    //lets get our parts for the subsearches
    map<ll, pair<int, ll> > mlow, mhigh;
    possible_sums(mlow, tl);
    possible_sums(mhigh, th);

    ll count_solutions = 0;


    auto rit = mhigh.rbegin();
    ll hspot = rit->first;
    ll lspot = 0;

    for(auto &p : mlow) {
      ll key = p.first;//this will be sorted
      pair<int, ll> value = p.second;
      while(rit != mhigh.rend() && rit->first + key > totsum) rit++; //walk to the equal one
      if(rit != mhigh.rend() && rit->first + key == totsum)
      {
        count_solutions += ((long long) rit->second.first) * p.second.first;
        hspot = rit->first;
        lspot = key;
      }
    }

    string solution;
    solution.resize(m);

    if(count_solutions == 1)
    {
      //walk back through parents to reconstruct that solution
      for(int j = (m / 2) - 1; j >= 0; j--)
      {
        if((s0[j] < s1[j]) ^ (tl[j] != (lspot - mlow[lspot].second))) {
          solution[j] = '1';
        } else {
          solution[j] = '0';
        }
        if(tl[j] == (lspot - mlow[lspot].second))
        {
          lspot = mlow[lspot].second;
        }
      }
      for(int j = m - 1; j >= m/2; j--)
      {
        if((s0[j] < s1[j]) ^ (th[j - (m/2)] != (hspot - mhigh[hspot].second))) {
          solution[j] = '1';
        } else {
          solution[j] = '0';
        }
        if(th[j - (m/2)] == (hspot - mhigh[hspot].second))
        {
          hspot = mhigh[hspot].second;
        }
      }
      cout << solution << endl;
    }
    else
    {
      cout << count_solutions << " solutions" << endl;
    }
}

int main()
{
    int c;
    cin >> c;
    for(int i=0; i<c; i++)
    {
        doCase();
    }
}