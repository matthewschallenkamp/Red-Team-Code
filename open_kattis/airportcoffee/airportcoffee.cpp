#include <iostream>
#include <vector>
#include <array>
#include <limits>

using namespace std;

struct stand
{
    uint64_t dist;
    uint32_t next;
    double time;
};

//Calculate time to walk a specific distance
//starting at a coffee cart
double walkTime(const uint64_t& dist, const uint32_t& speedNoCoffee, const uint32_t& speedCoffee,
                  const uint32_t& coffeeWait, const uint32_t& coffeeDrink)
{
    uint64_t distWait = speedNoCoffee * coffeeWait;
    uint64_t distSpeed = speedCoffee * coffeeDrink;
    
    //cerr << "Slow dist: " << distWait << endl;
    //cerr << "Fast dist: " << distSpeed << endl;

    if(dist <= distWait)
    {
        //cerr << "A" << endl;
        return dist/(double)speedNoCoffee;
    }
    else if(dist <= distWait + distSpeed)
    {
        //cerr << "B" << endl;
        return  coffeeWait + (dist-distWait)/(double)speedCoffee;
    }
    else
    {
        //cerr << "C" << endl;
        return coffeeWait + coffeeDrink + (dist - distWait - distSpeed)/(double)speedNoCoffee;
    }
}

//Finds the next bigger value in the array
//or the end, whichever is first
uint32_t search(const uint64_t& dist, const vector<stand>& standDists)
{
    uint32_t left = 0, right = standDists.size()-1, mid;
    do
    {
        mid = (left + right)/2;

        //cout << left << " " << mid << " " << right << endl;
        
        if(standDists[mid].dist == dist) return mid;
        if(standDists[mid].dist < dist)
            left = mid+1;
        else
            right = mid-1;
    }while(left <= right);
    return standDists[mid].dist < dist ? mid + 1 : mid;
}

int main()
{
    uint64_t dist;
    uint32_t speedNoCoffee, speedCoffee;
    uint32_t coffeeWait;
    uint32_t coffeeDrink;

    cin >> dist >> speedNoCoffee >> speedCoffee >> coffeeWait >> coffeeDrink;

    uint32_t stands;
    cin >> stands;

    vector<stand> standData(stands);
    for(stand& s : standData)
        cin >> s.dist;

    if(stands == 0)
    {
        cout << 0 << endl;
        return 0;
    }

    //Add 'coffee cart' at target location
    if(standData.back().dist != dist)
        standData.push_back({dist, standData.size()+1, 0});

    uint64_t distWait = speedNoCoffee * coffeeWait;
    uint64_t distSpeed = speedCoffee * coffeeDrink;

    //Calculate build dynamic table
    //backwards
    //cerr << "Fill table" << endl;
    for(int32_t i = standData.size()-2; i>=0; i--)
    {
        uint32_t found = search(standData[i].dist + distWait + distSpeed, standData);
        array<uint32_t, 3> checks{i+1, found, found-1};
        //cerr << "Checks for table " << i << " : " << i+1 << " " << found-1 << " " << found << endl;

        standData[i].time = numeric_limits<double>::max();
        for(int32_t j=0; j<3; j++)
        {
            if(checks[j] != i && checks[j] < standData.size())
            {
                uint64_t dist = standData[checks[j]].dist - standData[i].dist;
                double t = standData[checks[j]].time + walkTime(dist, speedNoCoffee, speedCoffee, coffeeWait, coffeeDrink);
                //cerr << t << " time to walk from " << i << " to " << checks[j] << " : " << dist << " distance" << endl;
                if(t < standData[i].time)
                {
                    standData[i].time = t;
                    standData[i].next = checks[j];
                }
            }
        }
    }

    //Follow solution forwards and output
    //cerr << "Build answer" << endl;
    string out = "";
    uint32_t count = 0;
    uint32_t curr = 0;
    do
    {
        count++;
        out += to_string(curr) + " ";
        curr = standData[curr].next;
    }while(curr < standData.size()-1);
    cout << count << endl << out << endl;
}