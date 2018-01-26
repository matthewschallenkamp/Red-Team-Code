#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;


int main()
{
  int tc;
  cin >> tc;
  while(tc--)
  {
    int f, e;
    cin >> f >> e;
    vector<int> rm(e), md(e);
    for(int i = 0; i < e; i++)
    {
        cin >> rm[i] >> md[i];
    }
    //first, lets get the base stair distances between each of the elevators
    //this might be able to be improved by doing some sort of gcd algo, but this
    //seems to be fast enough
    vector<vector<int> > bdists(e);
    for(int i = 0; i < e; i++)
    {
        bdists[i].resize(e);
        for(int j = 0; j < e; j++)
        {
            int diff = f + 5;
            int ic = rm[i], jc = rm[j];
            while(ic < f && jc < f)
            {
                diff = min(diff, abs(ic - jc));
                if(ic < jc)
                    ic += md[i];
                else
                    jc += md[j];
            }
            bdists[i][j] = diff;
        }
    }
    //now djikstras to each elevator from floor 0
    //seed our starting with each of the elevators
    priority_queue<pair<int, int>> dk;
    vector<int> dists(e, f);
    for(int i = 0; i < e; i++)
    {
        dk.push(make_pair(-rm[i], i));
    }

    //now djikstras to get the smallest cost to each elevator
    while(!dk.empty())
    {
        auto top = dk.top();
        dk.pop();
        if((-top.first) >= dists[top.second])
            continue;
        else
            dists[top.second] = -top.first;
        for(int i = 0; i < e; i++)
        {
            if(dists[top.second] + bdists[top.second][i] < dists[i])
                dk.push(make_pair(-(dists[top.second] + bdists[top.second][i]), i));
        }
    }

    //with that, we need to find the furthest floor. lets get a sorted list of
    //each floor that has an elevator outlet, paired with the cost of that outlet
    vector<pair<int, int> > sp;
    for(int i = 0; i < e; i++)
    {
        for(int ic = rm[i]; ic < f; ic+=md[i])
        {
            sp.emplace_back(ic, dists[i]);
        }
    }
    sort(sp.begin(), sp.end());
    //now we walk up looking for a best. At each floor pair we need to find the furthest floor in the middle of them
    //the furthest floor is going to be their middle weighted by the exit cost of each one
    //so each step to get to the lower floor will move us down one floor from the middle
    //and each step to get to the upper floor will move us up one floor from the middle
    int furthest = 0;
    int loc = 0;

    for(int i = 1; i < sp.size(); i++)
    {
        int meet = 0;
        int score = 0;

        meet = (sp[i-1].first - (sp[i-1].second - sp[i].second) + sp[i].first) / 2;
        score = (meet - sp[i-1].first) + sp[i-1].second;

        if(furthest < score)
        {
            furthest = score;
            loc = meet;
        }
    }

    //finally, we need to consider the last floor from the highest elevator
    if(sp.size() > 1 && furthest < (f - 1 - sp.back().first + sp.back().second))
    {
        furthest = (f - 1 - sp.back().first + sp.back().second);
        loc = f - 1;
    }
    //or if there is no elevators
    if(sp.size() < 2)
    {
        furthest = f - 1;
        loc = f - 1;
    }

    cout << furthest << " " << loc << endl;

  }
  return 0;
}

