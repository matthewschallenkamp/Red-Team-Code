#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
using namespace std;

#define locate(i, j) ((i) | ((j) << 8))

//First element, second element
typedef pair<uint8_t, uint8_t> domino;
//Domino, location, | of orientations
typedef uint8_t orientation;
typedef uint16_t location;
constexpr orientation Right = 1, Down = 2;
typedef tuple<uint8_t, location, orientation> placement;


array<array<uint8_t, 20>, 20> board;
int m, n;

void printBoard()
{
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
            cout << (char)board[i][j];
        cout << endl;
    }
}

void backtrack(vector<bool>& used, const vector<tuple<uint8_t, uint64_t, unordered_map<location, orientation>>>& placements, uint64_t& count)
{
    int32_t min = numeric_limits<int32_t>::max();
    int32_t minInd;
    
    for(int i=0; i<placements.size(); i++)
    {
        if(!used[i])
        {
            int32_t count = 0;
            auto& m = get<2>(placements[i]);
            for(const pair<location, orientation>& p : m)
            {
                if(board[p.first & 0xff][p.first >> 8] <= 10)
                {
                    if((p.second & Down) && board[(p.first & 0xff) + 1][p.first >> 8] <= 10) count++;
                    if((p.second & Right) && board[p.first & 0xff][(p.first >> 8) + 1] <= 10) count++;
                }
            }

            if(count < min)
            {
                min = count;
                minInd = i;
            }
        }
    }

    //If any domino can't be placed
    //we skip out
    if(min == 0) return;

    //If all dominos used, print the first board
    //and then increment counter
    if(min == numeric_limits<int32_t>::max())
    {
        //cerr << "Answer!" << endl;
        if(count == 0)
            printBoard();
        count++;
        return;
    }

    //Attempt to place this domino in each location and orientation it can
    //have
    const tuple<uint8_t, uint64_t, unordered_map<location, orientation>>& here = placements[minInd];
    used[minInd] = true;
    //cerr << "Place domino " << (int)get<0>(here) << " " << get<2>(here).size() << " places" << endl;
    for(const pair<location, orientation>& p : get<2>(here))
    {
        int i = p.first & 0xff;
        int j = p.first >> 8;

        //Check the board is open
        auto& space = board[i][j];
        if(space <= 10)
        {
            //Check adjacent spots open
            //and place domino
            if(p.second & Right)
            {
                auto& toRight = board[i][j+1];
                if(toRight <= 10)
                {
                    //cerr << "Place " << (int)get<0>(here) << " at " << i << ", " << j << " >" << endl;
                    space = '[';
                    toRight = ']';
                    backtrack(used, placements, count);
                    //cerr << "Remove " << (int)get<0>(here) << " at " << i << ", " << j << " >" << endl;
                    toRight = 10;
                }
            }

            if(p.second & Down)
            {
                auto& below = board[i+1][j];
                if(below <= 10)
                {
                    //cerr << "Place " << (int)get<0>(here) << " at " << i << ", " << j << " v" << endl;
                    space = 'n';
                    below = 'u';
                    backtrack(used, placements, count);
                    //cerr << "Remove " << (int)get<0>(here) << " at " << i << ", " << j << " v" << endl;
                    below = 10;
                }
            }

            //mark board open again
            space = 10;
        }
    }
    used[minInd] = false;
}

bool doCase()
{
    int k;
    cin >> m >> n >> k;
    if(m == 0) return false;

    vector<domino> tiles(k);
    unordered_map<uint8_t, vector<uint8_t>> numToDomino;

    uint16_t i = 0, j = 0;
    //Read dominoes, keeping track
    //of which dominoes have which numbers
    for(domino& d : tiles)
    {
        cin >> d.first >> d.second;
        d.first -= '0';
        d.second -= '0';
        //cerr << (int)d.first << " " << (int)d.second << "    ";
        numToDomino[d.first].push_back(i);
        if(d.first != d.second)
            numToDomino[d.second].push_back(i);
        ++i;
    }
    //cerr << endl;

    //cerr << endl;
    /*
    for(int i=0; i<=9; i++)
    {
        //cerr << i << ": ";
        if(numToDomino.count(i))
        for(uint8_t d : numToDomino[i])
            //cerr << (int)d << " ";
        //cerr << endl;
    }*/

    //Read the board
    i=0;
    for(auto itm = board.begin(); i < m; itm++, i++)
    {
        j=0;
        for(auto itn = itm->begin(); j < n; itn++, j++)
        {
            cin >> *itn;
            if(*itn != 'X') *itn -= '0';
        }
    }

    //cerr << "Board: " << endl;
    /*
    for(i=0; i <m; i++)
    {
        for(j=0; j<n; j++)
            //cerr << (int)board[i][j] << " ";
        //cerr << endl;
    }*/
            

    //Map dominoes to where they can be on the board
    //and places on the board to which dominoes can go there
    vector<tuple<uint8_t, uint64_t, unordered_map<location, orientation>>> dominoToPlacement(k); //Where can domino i go
    unordered_map<location, vector<placement>> boardToPlacement;  //What dominoes can go on board i
    for(i = 0; i<k; ++i)
    {
        get<0>(dominoToPlacement[i]) = i;
        get<1>(dominoToPlacement[i]) = 0;
    }

    i=0;
    for(auto itm = board.begin(); i < m; ++itm, ++i)
    {
        j=0;
        for(auto itn = itm->begin(); j < n; ++itn, ++j)
        {
            //cerr << "Match " << i << ", " << j << endl;
            for(const uint8_t& ntd : numToDomino[*itn])
            {
                //Resulting valid placements
                orientation orient = 0;
                int orients = 0;

                //Check if we need to flip the domino to put it here
                const domino& d = tiles[ntd];
                uint8_t left = d.first, right = d.second;
                if(*itn != left) {swap(left, right);}

                //cerr << "  Check domino " << (int)left << " " << (int)right << endl;
                //if(i < m-1)
                    //cerr << "    vs " << (int)(*((itm + 1)->begin() + j)) << endl;
                //if(j < n-1)
                    //cerr << "    vs " << (int)(*(itn + 1)) << endl;

                //Check going down
                if(i < m-1 && *((itm + 1)->begin() + j) == right)
                {
                    //Mark local orientation
                    orient |= Down;

                    //Mark for neighbor below
                    boardToPlacement[locate(i+1, j)].emplace_back(ntd, locate(i, j), Down);

                    orients++;
                }

                //Check going right
                if(j < n-1 && *(itn + 1) == right)
                {
                    //Mark local orientation
                    orient |= Right;

                    //Mark for neighbor to right
                    boardToPlacement[locate(i, j+1)].emplace_back(ntd, locate(i, j), Right);

                    orients++;
                }

                if(orients)
                {
                    //cerr << "match " << orients << endl;
                    boardToPlacement[locate(i, j)].emplace_back(ntd, locate(i, j), orient);
                    get<1>(dominoToPlacement[ntd])+=orients;
                    get<2>(dominoToPlacement[ntd])[locate(i, j)] = orient;
                }
            }
        }
    }

    //cerr << "Board to Dominoes: " << endl;
    /*
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            //cerr << i << ", " << j << ": ";
            if(boardToPlacement.count(locate(i, j)))
            for(placement p : boardToPlacement[locate(i, j)])
                //if(get<1>(p) == locate(i, j))
                    //cerr << (int)get<0>(p) << ";" << (int)get<2>(p) << " ";
            //cerr << endl;
        }
    }

    //cerr << "Dominoes to Board: " << endl;
    for(int i=0; i<k; i++)
    {
        //cerr << i << ";" << get<1>(dominoToPlacement[i]) << endl;
        for(pair<location, orientation> p : get<2>(dominoToPlacement[i]))
            //cerr << "\t" << (p.first & 0xff) << ", " << (p.first >> 8) << " : " << (int)p.second << endl;
    }*/

    //---------------------------------------------------------------------------
    //This next section trims a lot of the possible plays before the backtracking
    //starts. It consists of a number of functions that all call each other
    //to remove plays that are not helpful.
    //
    //Each of the functions takes a domino index, and will only
    //cause cascading updates to dominos with a lower index
    //This cuts down the number of tests required, as there
    //is a loop at the bottom, testing all dominos in increasing index order
    //---------------------------------------------------------------------------

    //Keep track of which squares and dominoes we've
    //already cascaded through so that
    //we don't update the same spots a bunch of times
    unordered_set<location> forcedSquares;
    unordered_set<uint8_t> forcedDominoes;

    //Removes all placement rules that would contradict
    //the given one. It's possible that this will cause
    //neighboring squares or other dominoes
    //to only have a single placement. Almost like a.....domino effect (⌐■_■)
    function<bool(placement, bool)> forcePlacement;

    //Removes a single placement from a square and optionally its neighbors
    function<bool(placement, location, bool)> removePlacement;

    //Tests if a domino can only cover 1 tile
    //If so, force that domino placement and update adjacent
    //squares
    function<bool(uint8_t)> testDomino;

    //Tests if a square can only be covered by a single domino
    //if this is the case, that domino is updated to 
    //have this space as its only valid placement
    function<bool(location)> testSquare =
    [&](location l)
    {
        //If no dominoes can cover a square, we're done
        if(boardToPlacement[l].size() == 0 && board[l & 0xff][l >> 8] != 'X')
        {
            //cerr << "No way to fill space " << i << ", " << j << endl;
            return false;
        }

        //If only 1 domino can cover a location
        //We force it to go there
        if(boardToPlacement[l].size() == 1 && !forcedSquares.count(l))
        {
            //cerr << "Space has 1 placement" << endl;
            if(!forcePlacement(boardToPlacement[l][0], false)) return false;
        }
        return true;
    };

    testDomino =
    [&](uint8_t i){
        auto& places = get<2>(dominoToPlacement[i]);

        //If a domino cannot be placed anywhere, we are done
        if(places.size() == 0)
        {
            //cerr << "No way to place domino " << (int)i << endl;
            return false;
        }

        if(places.size() == 1 && !forcedDominoes.count(i))
        {
            location there = places.begin()->first;
            placement p(i, there, places.begin()->second);

            //cerr << "Domino has 1 placement" << endl;
            if(!forcePlacement(p, false)) return false;   
        }

        return true;
    };

    removePlacement =
    [&](placement p, location loc, bool cascade)
    {
        //cerr << "Remove placement " << (int)get<0>(p) << " : " << (int)get<2>(p) << " at " <<
            //(int)(get<1>(p) & 0xff) << ", " << (int)(get<1>(p) >> 8) << " from " << 
            //(int)(loc & 0xff) << ", " << (int)(loc >> 8) << endl;

        auto& otherPlaces = boardToPlacement[loc];
        auto it = otherPlaces.begin();
        for(; it != otherPlaces.end(); it++)
            if(get<0>(*it) == get<0>(p) && get<1>(*it) == get<1>(p)) break;

        if(it != otherPlaces.end())
        {
            get<2>(*it) &= ~get<2>(p);
            //cerr << "\tSquare has orientation " << (int)get<2>(*it) << " remaining for domino " << (int)get<0>(p) << endl;
            if(get<2>(*it) == 0)
                otherPlaces.erase(it);

            //Check if we forced the other square to only have one placement
            if(!testSquare(get<1>(p))) return false;
        }
        else
            //cerr << "\t=====================Remove Placement where it doesn't exist========================" << endl;

        if(cascade)
        {
            if(get<2>(p) & Down)
            {
                //cerr << "Move down" << endl;
                if(!removePlacement(p, get<1>(p) + 1, false)) return false;
            }
            if(get<2>(p) & Right)
            {
                //cerr << "Move right" << endl;
                if(!removePlacement(p, get<1>(p) + (1<<8), false)) return false;
            }
        }

        return true;
    };

    forcePlacement =
    [&](placement place, bool isAdjacent)
    {
        uint8_t domino = get<0>(place);
        location loc = get<1>(place);
        orientation orient = get<2>(place);

        //Mark that we're forcing this square to be covered
        forcedSquares.insert(loc);

        //We're only forcing placement of this domino
        //if there's only one orientation
        if(orient != (Down | Right))
            forcedDominoes.insert(domino);

        if(isAdjacent)
        {
            if(orient == Right)
                loc += (1<<8);
            else if(orient == Down)
                loc += 1;
            //else
                //cerr << "=====================Force Adjacent Placement with ambiguous orientation========================" << endl;
        }

        //cerr << "Force placement " << (int)domino << " : " << (int)orient << " at " << (isAdjacent?"adjacent ":"") << (int)(loc & 0xff) << ", " << (int)(loc >> 8) << endl;
    
        //Have to store up remove calls during function to avoid
        //breaking iterators
        vector<tuple<placement, location, bool>> removals;
        vector<uint8_t> dTests;

        //Clear out all other placements at the same space
        //from the associated dominoes and adjacent spaces
        //cerr << "\tRemove other placements which cover here" << endl;
        for(placement p : boardToPlacement[loc])
        {
            if(p != place)
            {
                if(get<0>(p) == get<0>(place) && get<1>(p) == get<1>(place))
                    get<2>(p) = get<2>(place) ^ (Down | Right);

                int orientations = ((get<2>(p) & Down) ? 1:0) + ((get<2>(p) & Right) ? 1:0);

                //cerr << "\tRemove placement " << (int)get<0>(p) << " : " << (int)get<2>(p) << " at " << (int)(get<1>(p) & 0xff) << ", " << (int)(get<1>(p) >> 8) << endl;

                //If the play originates at a different location,
                //Remove it from that location
                if(get<1>(p) != loc)
                    removals.emplace_back(p, get<1>(p), false);
                //Else, if play originates here, remove it from neighbors
                else
                    if(get<2>(p) & Right)
                        removals.emplace_back(p, get<1>(p) + (1<<8), false);
                    if(get<2>(p) & Down)
                        removals.emplace_back(p, get<1>(p) + 1, false);

                //Remove the place from the associated domino
                auto& dominoDat = dominoToPlacement[get<0>(p)];
                get<1>(dominoDat) -= orientations;
                get<2>(dominoDat)[get<1>(p)] &= ~get<2>(p);
                //cerr << "\tDomino " << (int)get<0>(p) << " has orientation " << (int)get<2>(dominoDat)[get<1>(p)] << " remaining at " << (int)(get<1>(p) & 0xff) << ", " << (int)(get<1>(p) >> 8) << endl;
                if(get<2>(dominoDat)[get<1>(p)] == 0)
                    get<2>(dominoDat).erase(get<1>(p));

                //Check if we forced that domino to only have one place
                dTests.emplace_back(get<0>(p));
            }
        }

        //cerr << "\tRemove other placements of domino" << endl;
        //Remove this domino from all other tiles
        auto& dominoDat = dominoToPlacement[domino];
        for(const pair<location, orientation>& p : get<2>(dominoDat))
            if(p.first != get<1>(place))
                removals.emplace_back(placement(domino, p.first, p.second), p.first, true);

        //Set the target placement as the only one for the associated space
        boardToPlacement[loc].clear();
        boardToPlacement[loc].push_back(place);

        //cerr << "Domino " << get<0>(valid) << " forced onto " << i << ", " << j << endl;

        //Set this placement as the only valid one for the associated domino
        get<1>(dominoDat) = ((orient & Down) ? 1:0) + ((orient & Right) ? 1:0);
        get<2>(dominoDat).clear();
        get<2>(dominoDat)[get<1>(place)] = orient;

        //If we're updating the origin space of the placement
        //and the placement is a single orientation
        //then force the adjacent space covered
        if(!isAdjacent)
        {
            //cerr << "\tCheck for definite orientation" << endl;
            if(get<2>(place) == Right)
            {
                if(!forcePlacement(place, true)) return false;
            }
            else if(get<2>(place) == Down)
            {
                if(!forcePlacement(place, true)) return false;
            }
        }

        for(auto& t : removals)
            if(!removePlacement(get<0>(t), get<1>(t), get<2>(t))) return false;

        for(auto& d : dTests)
            if(!testDomino(d)) return false;

        return true;
    };

    cerr << "Solving..." << endl;
    //Look for board spaces that can only be covered by one domino
    //But don't do any cascading updates
    for(i=0; i < m; ++i)
        for(j=0; j < n; ++j)
            if(!testSquare(locate(i, j)))
            {
                cout << "impossible" << endl;
                return true;
            }

    //Look for dominoes that can only be placed in one spot
    for(i=0; i<tiles.size(); i++)
        if(!testDomino(i))
        {
            cout << "impossible" << endl;
            //cerr << "Domino " << i << " has no location" << endl;
            return true;
        }
    
    //Backtracking based on where dominoes go is tooooo slloooowww
    //so instead, build a list of which dominoes go on which squares and backtrack
    //on that

    //Sort the dominos in order of least placements to most
    sort(dominoToPlacement.begin(), dominoToPlacement.end(),
    [](const tuple<uint8_t, uint64_t, unordered_map<location, orientation>>& left, const tuple<uint8_t, uint64_t, unordered_map<location, orientation>>& right)
    {
        return get<1>(left) < get<1>(right);
    });

    //cerr << "Placement order: " << endl;
    //for(auto& t : dominoToPlacement)
        //cerr << (int)get<0>(t) << " ";
    //cerr << endl;

    uint64_t ways = 0;
    vector<bool> used(k);
    cerr << "Backtracking..." << endl;
    backtrack(used, dominoToPlacement, ways);

    if(ways == 0)
    {
        cout << "impossible" << endl;
        //cerr << "Unable to fill board" << endl;
    }
    else
        cout << ways-1 << endl;

    return true;
}

int main()
{
    while(doCase()) cout << endl;
    return 0;
}