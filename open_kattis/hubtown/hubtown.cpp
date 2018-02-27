#include <limits>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

void doCase()
{
    int people, trains;
    cin >> people >> trains;

    vector<pair<long double, int> > people_angles(people);
    vector<pair<long double, pair<int, int> > > train_angles(trains);

    long double tx, ty, tc;
    for(int i = 0; i < people; i++)
    {
        cin >> tx >> ty;
        people_angles[i] = make_pair(atan2(ty, tx), i);
    }

    for(int i = 0; i < trains; i++)
    {
        cin >> tx >> ty >> tc;
        train_angles[i] = make_pair(atan2(ty, tx), make_pair(tc, i));
    }

    sort(people_angles.begin(), people_angles.end());
    sort(train_angles.begin(), train_angles.end());
    int tindex = 0, count = 0;
    int pindex = 0;
    vector<int> trainto(people_angles.size(), -2);

    //starting angles
    for(; pindex < people_angles.size(); pindex++)
    {
        if(people_angles[pindex].first > train_angles.front().first)
            break;
        if(abs((people_angles[pindex].first + (2*M_PI - train_angles.back().first)) - (train_angles.front().first - people_angles[pindex].first)) < 1e-15)
        {
            trainto[pindex] = -1;
        }
        else if(people_angles[pindex].first + (2*M_PI - train_angles.back().first) < (train_angles.front().first - people_angles[pindex].first))
        {
            if(train_angles.back().second.first > 0)
            {
                train_angles.back().second.first--;
                count++;
                trainto[pindex] = trains - 1;
            }
        }
        else
        {
            if(train_angles.front().second.first > 0)
            {
                train_angles.front().second.first--;
                count++;
                trainto[pindex] = 0;
            }   
        }
    }
    // normal angles

    for(; pindex < people_angles.size(); pindex++)
    {
        while(tindex != trains && people_angles[pindex].first > train_angles[tindex].first)
            tindex++;
        if(tindex == trains)
            break;

        if(abs(people_angles[pindex].first - train_angles[tindex - 1].first - (train_angles[tindex].first - people_angles[pindex].first)) < 1e-15)
        {
            trainto[pindex] = -1;
        }
        else if((people_angles[pindex].first - train_angles[tindex - 1].first) < (train_angles[tindex].first - people_angles[pindex].first))
        {
            if(train_angles[tindex - 1].second.first > 0)
            {
                train_angles[tindex - 1].second.first--;
                count++;
                trainto[pindex] = tindex - 1;
            }
        }
        else
        {
            if(train_angles[tindex].second.first > 0)
            {
                train_angles[tindex].second.first--;
                count++;
                trainto[pindex] = tindex;
            }   
        }

    }
    int es = pindex;
    //ending angles
    for(; pindex < people_angles.size(); pindex++)
    {
        if(abs(people_angles[pindex].first - train_angles.back().first - (train_angles.front().first + (2*M_PI - people_angles[pindex].first))) < 1e-15)
        {
            trainto[pindex] = -1;
        }
        else if((people_angles[pindex].first - train_angles.back().first) < (train_angles.front().first + (2*M_PI - people_angles[pindex].first)))
        {
            if(train_angles.back().second.first > 0)
            {
                train_angles.back().second.first--;
                count++;
                trainto[pindex] = trains - 1;
            }
        }
        else
        {
            if(train_angles.front().second.first > 0)
            {
                train_angles.front().second.first--;
                count++;
                trainto[pindex] = 0;
            }   
        }
    }

    //now do swappables
    //assign to right first, then lefts
    pindex = es;
    vector<vector<int> > swappables(trains);
    vector<int> pushed;
    //starting angles
    pindex = tindex = 0;

    for(; pindex < people_angles.size(); pindex++)
    {
        if(people_angles[pindex].first > train_angles.front().first)
            break;
    }
    // normal angles

    for(; pindex < people_angles.size(); pindex++)
    {
        while(tindex != trains && people_angles[pindex].first > train_angles[tindex].first)
            tindex++;
        if(tindex == trains)
            break;
        if(trainto[pindex] == -1)
        {
            //assign to right
            if(swappables[tindex - 1].size() < train_angles[tindex - 1].second.first)
                swappables[tindex - 1].push_back(pindex);
            else if(swappables[tindex].size() < train_angles[tindex].second.first)
                swappables[tindex].push_back(pindex);
        }
    }

    vector<int> order = swappables.front();
    swappables.front().clear();
    train_angles.front().second.first -= order.size();
    //ending angles
    for(; pindex < people_angles.size(); pindex++)
    {
        if(trainto[pindex] == -1)
        {
            //assign to right
            if(swappables.back().size() < train_angles.back().second.first)
                swappables.back().push_back(pindex);
            else if(swappables.front().size() < train_angles.front().second.first)
                swappables.front().push_back(pindex);
            else pushed.push_back(pindex);
        }
    }
    tindex = pindex = 0;
    //starting angles
    for(; pindex < people_angles.size(); pindex++)
    {
        if(people_angles[pindex].first > train_angles.front().first)
            break;
        if(trainto[pindex] == -1)
        {
            //assign to right
            if(swappables.back().size() < train_angles.back().second.first)
                swappables.back().push_back(pindex);
            else if(swappables.front().size() < train_angles.front().second.first)
                swappables.front().push_back(pindex);
            else pushed.push_back(pindex);
        }
    }
    for(auto item : order)
        swappables.front().push_back(item);
    train_angles.front().second.first += order.size();


    //now we have to walk through swappables that we missed

    //we've inserted all of them up till now in order, so each is sorted. Now we have some extras here in the first spot.
    //we'll try to move them in by pushing them into the new one, and pulling out whatever might get moved to the next
    //if we come across somewhere that isnt full, we'll push extra in

    // cerr << pushed.size() << endl;
    vector<int> newpushed;
    for(int s = 0; s < swappables.size(); s++)
    {
        int free = -swappables[s].size() + train_angles[s].second.first;
        if(pushed.size())
            cerr << free << " " << pushed.size() << endl;
        for(int i = free; i < pushed.size(); i++)
        {
            //we want to take as many items that are after this train that we can out of it, and put them into our newpushed
            if(swappables[s].size() && 
                people_angles[swappables[s].back()].first > train_angles[s].first &&
                (s + 1 >= swappables.size() || people_angles[swappables[s].back()].first < train_angles[s + 1].first))
            {
                newpushed.push_back(swappables[s].back());
                swappables[s].pop_back();
            }
            else
                break;
        }
        for(int i = 0; i < pushed.size() && swappables[s].size() < train_angles[s].second.first; i++)
            swappables[s].push_back(pushed[i]);
        pushed = newpushed;
        newpushed.clear();
        count += swappables[s].size();
    }
    // for(int i = 0; i < trainto.size(); i++)
    // {
    //     cerr << trainto[i] << " ";
    // } cerr << endl;
    cout << count << endl;
    for(int i = 0; i < trainto.size(); i++)
    {
        if(trainto[i] >= 0)
            cout << people_angles[i].second << " " << train_angles[trainto[i]].second.second << endl;
    }
    for(int i = 0; i < swappables.size(); i++)
    {
        for(auto item : swappables[i])
        {
            cout << people_angles[item].second << " " << train_angles[i].second.second << endl;
        }
    }
    cerr << count << endl;

}

int main()
{
    doCase();
    return 0;
}