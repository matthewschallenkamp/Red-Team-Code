#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

struct point
{
    int32_t x, y;

    point(int32_t x_=0, int32_t y_=0) : x(x_), y(y_){}

    // Only < operator is unusual behavior
    // the rest can probably all be memorized
    bool operator <(const point& other) const
    {
        return (x < other.x ? true : (x == other.x && y < other.y));
    }

    bool operator == (const point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator != (const point& other) const
    {
        return x != other.x || y != other.y;
    }

    point operator +(const point& r) const
    {
        return point(x+r.x, y+r.y);
    }

    point operator -(const point& r) const
    {
        return point(x-r.x, y-r.y);
    }

    point operator /(const int32_t& r) const
    {
        return point(x/r, y/r);
    }
};

int64_t gcd(const int64_t& a, const int64_t b)
{
    return b == 0 ? a : gcd(b, a % b);
}

struct Line
{
    int64_t a, b, c;

    void normalize()
    {
        if (a < 0 || (a == 0 && b < 0)) {
            a *= -1;
            b *= -1;
            c *= -1;
        }    

        int64_t g = gcd(gcd(a, b), c);

        a /= g;
        b /= g;
        c /= g;
    }

    Line(const point& p1, const point& p2) : a(p1.y-p2.y), b(p2.x-p1.x), c(-(a*p1.x+b*p1.y)){normalize();}
    Line(const point& p1, const point& p2, bool dummy)
    {
        point mid = (p1 + p2) / 2;

        a = p2.x - p1.x;
        b = p2.y - p1.y;
        c = -(a * mid.x + b * mid.y);
        normalize();
    }
    bool operator == (const Line& other) const
    { return a == other.a && b == other.b && c == other.c; }

    bool operator < (const Line& other) const
    { return a == other.a ? (b == other.b ? c < other.c : b < other.b) : a < other.a;}
};

ostream& operator << (ostream& out, const point& p)
{
    return out << "(" << p.x << ", " << p.y << ")";
}

ostream& operator << (ostream& out, const Line& l)
{
    return out << "(" << l.a << ", " << l.b << ", " << l.c << ")";
}

uint64_t intRoot(const uint64_t n)
{
    uint64_t root = sqrt(n);
    while(root * root < n) root++;
    while(root * root > n) root--;
    return root;
}

//Squared magnitude of point vector
uint64_t sqmag(const point& p1)
{
    return (int64_t)p1.x*(int64_t)p1.x + (int64_t)p1.y*(int64_t)p1.y;
}

int main()
{
    uint64_t n;
    cin >> n;

    struct inf {uint64_t reflect = 0, colin = 0;};
    map<Line, inf> lineBuckets;     
    map<point, uint64_t> pointBuckets;
    vector<point> pts;

    //Keep track of our best bucket size
    uint64_t bestCount = 0;

     //cerr << "Check segments..." << endl;
    //For each pair of points
    point p1;
    for(uint64_t i=0; i<n; i++)
    {
        cin >> p1.x >> p1.y;
        p1.x*=2;
        p1.y*=2;
        
        for(auto it2 = pts.begin(); it2 != pts.end(); it2++)
        {
            auto& p2 = *it2;

            //Find the mid point and inverse slope (The perpendicular slope going through their midpoint)
            point mid = (p1 + p2) / 2;
            auto sl = Line(p1, p2);
            auto inv = Line(p1, p2, true);

            //Add 2 to the bucket for this pair
            lineBuckets[sl].colin+=1;
             //cerr << "\tColin bucket [" << sl << "]+=2 : " << colinearBuckets[sl] << endl;

            bestCount = max(bestCount, lineBuckets[inv].reflect+=2);
             //cerr << "\tLin bucket [" << inv << "]+=2 : " << lineBuckets[inv] << endl;

            //Add 2 to the rotational bucket for the midpoint
            bestCount = max(bestCount, pointBuckets[mid]+=2);
             //cerr << "\tRot bucket [" << mid << "]+=2 : " << pointBuckets[mid] << endl;
        }

        //Add 1 to the rotational bucket for point 1
        bestCount = max(bestCount, pointBuckets[{p1.x, p1.y}]+=1);
         //cerr << "\tRot bucket [" << p1.x << " " << p1.y << "]++ : " << pointBuckets[{p1.x, p1.y}] << endl;

        pts.push_back(p1);
    }

    //For each line that had colinear points on it
    //determine how many points were colinear on it
    //and add that many points to covered by that line
    //cerr << "Check colinear points" << endl;
    for(auto& l : lineBuckets)
    {
        if(l.second.colin)
        {
            //cerr << "Solve choose 2 " << l.second << endl;
            uint64_t colinearPoints = (1 + intRoot(1+8*l.second.colin)) / 2;
            bestCount = max(bestCount, l.second.reflect += colinearPoints);
            //cerr << "\tLin bucket [" << l.first << "]+=" << colinearPoints << " : " << lineBuckets[l.first] << endl;
        }
    }

    //For every pair of points, map each point
    //to the other at their distance away
    //cerr << "Compute distances..." << endl;
    bool finished = false;
    for(auto it1 = pts.begin(); !finished && it1 != pts.end(); it1++)
    {
        vector<pair<uint64_t, point>> dists;
        for(auto it2 = it1+1; it2 != pts.end(); it2++)
            dists.emplace_back(sqmag(*it1-*it2), *it2);

        sort(dists.begin(), dists.end());
        
        //cerr << "Match distances" << endl;
        //For every group of points that are the same distance
        //from some other point
        //We can construct a line between them that that third point
        //is on. If that line has no colinear points on it, then 
        //we can take it's total + 1 and see if that's a better best score
        //if so, we are done; nothing can beat it
        //cerr << "Mark distances..." << endl;
        for(auto dit1 = dists.begin(); !finished && dit1 != dists.end(); dit1++)
            for(auto dit2 = dit1 + 1; !finished && dit2 != dists.end() && dit2->first == dit1->first; dit2++)
            {
                auto inv = Line(dit1->second, dit2->second, true);
                const auto& line = lineBuckets[inv];
                //cerr << "\t\tInv Slope " << *it1 << " -> " << *it2 << endl;
                if(!line.colin)
                {
                    auto oldBest = bestCount;
                    bestCount = max(bestCount, line.reflect + 1);
                     //cerr << "\t\tLin bucket [" << inv << "]++ : " << lineBuckets[inv] << endl;
                    if(oldBest != bestCount)
                    {
                         //cerr << "Found single point on line!" << endl;
                        finished = true;
                    }
                }
            }
    }

     //cerr << n << endl << bestCount << endl;
    cout << n - bestCount << endl;
}