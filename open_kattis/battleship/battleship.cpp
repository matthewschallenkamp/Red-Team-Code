#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <functional>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

bool takeTurn(pair<vector<string>, int>* map, int x, int y)
{
    if(map->first[map->first.size() - y - 1][x] == '#')
    {
        map->first[map->first.size() - y - 1][x] = '_';
        map->second--;
        return true;
    }
    return false;
}

void doCase()
{
    int w, h, n;
    cin >> w >> h >> n;

    pair<vector<string>, int> map1, map2;
    map1.first.resize(h);
    map2.first.resize(h);

    for(string& s : map1.first)
    {
        cin >> s;
        for(char c : s)
            if(c == '#') map1.second++;
    }
    for(string& s : map2.first)
    {
        cin >> s;
        for(char c : s)
            if(c == '#') map2.second++;
    }

    //cerr << "Player 1 has " << map1.second << " spots" << endl;
    //cerr << "Player 2 has " << map2.second << " spots" << endl;

    bool gameDone = false;
    int winner = 0;

    auto checkForWinner =
    [&]()
    {
        if(map1.second == 0 && map2.second == 0)
        {
            //cerr << "Both maps empty" << endl;
            gameDone = true;
            winner = 0;
        }
        else if(map1.second == 0)
        {
            //cerr << "Map 1 empty" << endl;
            winner = 2;
            gameDone = true;
        }
        else if(map2.second == 0)
        {
            //cerr << "Map 2 empty" << endl;
            winner = 1;
            gameDone = true;
        }
    };

    checkForWinner();

    bool player1 = true;
    pair<vector<string>, int>* currMap = &map2;

    int x, y;
    bool lastTurn = false;
    for(int i=0; i<n; i++)
    {
        cin >> x >> y;
        if(!gameDone || lastTurn)
        {
            bool hit = takeTurn(currMap, x, y);
            if(hit)
            {
                //cerr << "Player " << (player1?1:2) << " hit " << x << ", " << y << endl;
            }
            else
            {
                //cerr << "Player " << (player1?1:2) << " miss " << x << ", " << y << endl;
            }
            checkForWinner();

            //If player misses, check for
            //game over and switch turns
            if(!hit || (gameDone && !lastTurn))
            {
                if(gameDone)
                {
                    //If player 1 killed player 2
                    //player 2 gets 1 more turn
                    if(player1 == true)
                    {
                        lastTurn = true;
                        //cerr << "Player 2 gets one final turn" << endl;
                    }
                    else
                    {
                        lastTurn = false;
                    }
                }

                player1 = !player1;
                currMap = player1 ? &map2 : &map1;
            }
        }
    }

    if(winner == 0)
    {
        cout << "draw" << endl;
    }
    else if(winner == 1)
    {
        cout << "player one wins" << endl;
    }
    else if(winner == 2)
    {
        cout << "player two wins" << endl;
    }
    //cerr << endl;
}

int main()
{
    int t;
    cin >> t;

    for(int i=0; i<t; i++)
    {
        doCase();
    }
}