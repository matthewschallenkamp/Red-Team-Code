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

int main()
{
    int tc;
    cin >> tc;
    while(tc--)
    {
        int n, g;
        cin >> n >> g;
        vector<vector<int> > roads(4);
        roads[0].resize(n+1);
        roads[1].resize(n+1);
        roads[2].resize(n+1);
        roads[3].resize(n+1);
        //edges must follow a cycle
        for(int i = 1; i <= n; i++)
        {
            cin >> roads[0][i] >> roads[1][i] >> roads[2][i] >> roads[3][i];
        }
        vector<bool> visitedn(n, false), visitede(n, false);
        visitedn[0] = true;
        visitede[0] = true;
        int signs = 0;
        for(int i = 0; i < n; i++)
        {
            if(!visitedn[i])
            {
                bool hitgoal = false;
                int loc = i;
                int dir = 2;
                do {
                    //this must loop eventually, lets follow through and mark visited
                    if(dir == 2 || dir == 0)
                        visitedn[loc] = true;
                    else
                        visitede[loc] = true;
                    if(loc == g)
                        hitgoal = true;

                    //go to the next spot and direction
                    int newloc = roads[dir][loc];
                    if(roads[0][newloc] == loc)
                        dir = 2;
                    else if(roads[2][newloc] == loc)
                        dir = 0;
                    else if(roads[1][newloc] == loc)
                        dir = 3;
                    else if(roads[3][newloc] == loc)
                        dir = 1;
                    loc = newloc;

                } while(loc != i || dir != 2);
                if(!hitgoal)
                    signs++;
            }

            if(!visitede[i])
            {
                bool hitgoal = false;
                int loc = i;
                char dir = 1;
                do {
                    //this must loop eventually, lets follow through and mark visited
                    if(dir == 2 || dir == 0)
                        visitedn[loc] = true;
                    else
                        visitede[loc] = true;
                    if(loc == g)
                        hitgoal = true;

                    //go to the next spot and direction
                    int newloc = roads[dir][loc];
                    if(roads[0][newloc] == loc)
                        dir = 2;
                    if(roads[2][newloc] == loc)
                        dir = 0;
                    if(roads[1][newloc] == loc)
                        dir = 3;
                    if(roads[3][newloc] == loc)
                        dir = 1;
                    loc = newloc;

                } while(loc != i || dir != 1);
                if(!hitgoal)
                    signs++;
            }
        }
        cout << signs << endl;
    }
}











