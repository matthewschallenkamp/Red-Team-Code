#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>
#include <iomanip>

using namespace std;
using std::cout; //Solves complaint from intellisense

//Double compare epsilon
const double EPSILON = 0.0000001;

int main()
{
    bool debug = false;
    int n;
    cin >> n;
    if(debug) cout << n << " ";

    vector<double> l(n);
    for(auto &i : l) {
        cin >> i;
        if(debug) cout << i << " ";        
    }
    double x, y;
    cin >> x >> y;
    if(debug) cout << x << " " << y << endl;

    //min, max
    vector<pair<int, int>> ranges(n);

    //Range of first link is it's own length
    ranges[0] = make_pair(l[0], l[0]);
    for(int i=1; i<n; i++)
    {
        //Every other range extends from the min/max of previous
        //If the range has a positive an negative, then the min is actually just 0; the origin
        ranges[i] = make_pair(min(abs(ranges[i-1].first - l[i]), abs(ranges[i-1].second - l[i])),
                                  abs(ranges[i-1].second + l[i]));
        if((ranges[i-1].first - l[i] < -EPSILON) != (ranges[i-1].second - l[i] < EPSILON))
            ranges[i].first = 0;
            
        //cerr << "Link " << i << " range: " << ranges[i].first << " -> " << ranges[i].second << endl;
    }

    //Find distance to target and unit vector
    //towards it
    double tDist = sqrt(x*x + y*y);
    double xU = 0, yU = 1;
    if(tDist > EPSILON)
    {
      xU = x/tDist;
      yU = y/tDist;
    }

    //Check if target is outside range of final link
    if(tDist < ranges[n-1].first || tDist > ranges[n-1].second)
    {
        //cerr << "outside final link " << endl;
        //cerr << "Distance to target: " << tDist << endl;
        //cerr << "Vector to target: " << xU << " " << yU << endl;
        //Check if it's closer to the min or max of final link range
        //And project target onto which ever range is closer
        if(abs(ranges[n-1].first - tDist) < abs(ranges[n-1].second - tDist))
        {
            x = xU * ranges[n-1].first;
            y = yU * ranges[n-1].first;
        }
        else
        {
            x = xU * ranges[n-1].second;
            y = yU * ranges[n-1].second;
        }
        //cerr << "Adjusted point to " << x << " " << y << endl;
    }

    //Storage for path
    stack<pair<double, double>> path;

    //Go backwards from outer link to origin
    //finding where the links end up
    for(int i=n-1; i>0; i--)
    {
        path.push(make_pair(x, y));
        tDist = sqrt(x*x + y*y);

        bool onEdge = abs(tDist - ranges[i].second) < EPSILON;
        //cerr << "---------------------" << endl;
        //cerr << "At point " << x << " " << y << endl;
        //cerr << "Distance from origin: " << tDist << endl;
        //cerr << "On edge? " << onEdge << endl;
        //cerr << "Link size: " << l[i] << endl;
        //cerr << "Target edge: " << ranges[i].second << endl;

        //If we can get from point to max range of previous link, do so
        //otherwise travel directly towards max range of previous link
        if(tDist + l[i] - ranges[i-1].second > EPSILON && !onEdge)
        {
            //cerr << "special edge at " << i << endl;
            //Find interesting point
            double cosA = ((l[i] * l[i])
              - (ranges[i-1].second * ranges[i-1].second)
              - (tDist * tDist)) 
              / (-2.0 * ranges[i-1].second * tDist);
            //cerr << "CosA " << cosA << endl;
            //cosA is cos of angle from origin
            //now to get new point, we need to move to ranges[i] at the new angle
            double currAngle = atan2(y, x);
            currAngle += acos(cosA);
            y = sin(currAngle) * ranges[i-1].second;
            x = cos(currAngle) * ranges[i-1].second;
            //cerr << "New target: " << x << " " << y << endl;
            
            //Adjust unit vector to point at it
            tDist = sqrt(x*x + y*y);
            xU = x/tDist, yU = y/tDist;
        }
        else
        {
            //If we're directly on a max range, then travel
            //towards the origin
            if(onEdge)
            {
                x = xU * (tDist - l[i]);
                y = yU * (tDist - l[i]);

                //If length of link longer than distance to origin
                //go through the origin
                if(l[i] > tDist)
                {
                    xU = -xU;
                    yU = -yU;
                }
            }
            //Otherwise, travel directly away from the origin
            else
            {
                x = xU * (tDist + l[i]);
                y = yU * (tDist + l[i]);
            }
        }
    }

    cout << setprecision(5) << fixed << x << " " << y << endl;
    while(path.size())
    {
        cout << path.top().first << " " << path.top().second << endl;
        path.pop();
    }
    return 0;
}