#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <map>
#include <limits>
#include <stack>

using namespace std;

struct point
{
    long double x, y;

    point(long double x_=0, long double y_=0) : x(x_), y(y_){}

    // Only < operator is unusual behavior
    // the rest can probably all be memorized
    bool operator <(const point& other) const
    {
        return (x < other.x ? true : (x == other.x && y < other.y));
    }

    bool operator == (const point& other) const
    {
        return abs(other.x - x) < 0.0001 && abs(other.y - y) < 0.0001;
    }

    bool operator != (const point& other) const
    {
        return abs(other.x - x) > 0.0001 || abs(other.y - y) > 0.0001;
    }

    point operator *(const long double& d) const
    {
        return point(x*d, y*d);
    }

    point operator /(const long double& d) const
    {
        return point(x/d, y/d);
    }

    point operator +(const point& r) const
    {
        return point(x+r.x, y+r.y);
    }

    point operator -(const point& r) const
    {
        return point(x-r.x, y-r.y);
    }

    point operator -() const
    {
        return point(-x, -y);
    }
};

ostream& operator << (ostream& out, const point& p)
{
    return out << "(" << p.x << ", " << p.y << ")";
}

//Container for line segment
struct segment
{
    point p1, p2;
};

//Constant values to be returned
constexpr int Colinear = -1;
constexpr int NoIntersect = 0;
constexpr int Intersect = 1;
constexpr int CW = 2;
constexpr int CCW = 3;
constexpr int Inside = 4;
constexpr int Outside = 5;
constexpr int OnEdge = 6;

//Dot product ab.bc
double dot(const point& a, const point& b, const point& c)
{
    point AB = b - a;
    point BC = c - b;
    return AB.x*BC.x + AB.y*BC.y;
}

//Cross product
//AB X AC
double cross(const point& A, const point& B, const point& C)
{
    point AB = B - A, AC = C - A;
    return(AB.x * AC.y - AB.y * AC.x);
}

//Finds orientation of triplet of points p, q, r
//Returns Colinear, CW, or CCW
int orientation(const point& p, const point& q, const point& r)
{
    double val = cross(p, q, r);
    if(abs(val) < 0.0001) return Colinear;
    return (val > 0) ? CW : CCW;
}

//Checks if point p is possibly on the segment s
//but doesn't guarantee it is
bool onSegment(const point& p, const segment& s)
{
    bool x = (abs(s.p1.x - s.p2.x) < 0.001 && abs(p.x - s.p2.x) < 0.001) || (p.x <= max(s.p1.x, s.p2.x) && p.x >= min(s.p1.x, s.p2.x));
    bool y = (abs(s.p1.y - s.p2.y) < 0.001 && abs(p.y - s.p2.y) < 0.001) || (p.y <= max(s.p1.y, s.p2.y) && p.y >= min(s.p1.y, s.p2.y));
    return x && y;
}

//Returns of list of intersection points between segments s1, and s2
//If they do not intersect, the result is an empty vector
//If they intersect at exactly 1 point, the result contains that point
//If they overlap for non-0 distance, the left and right points of that intersection
//  are returned
vector<point> intersect(const segment& s1, const segment& s2)
{
/*
    cout << "Intersect:" << endl;
    cout << s1.p1 << " -> " << s1.p2 << endl;
    cout << s2.p1 << " -> " << s2.p2 << endl;
*/
    point a = s1.p1, b = s1.p2, c = s2.p1, d = s2.p2;

    if(orientation(a, b, c) == Colinear && orientation(a, b, d) == Colinear &&
        orientation(c, d, a) == Colinear && orientation(c, d, b) == Colinear)
    {
        point min_s1 = min(a, b), max_s1 = max(a, b);
        point min_s2 = min(c, d), max_s2 = max(c, d);
/*
        cout << "Colinear" << endl;
        cout << min_s1 << " -> " << max_s1 << endl;
        cout << min_s2 << " -> " << max_s2 << endl;
*/
        if(max_s1 < min_s2 || max_s2 < min_s1) return {};

        point start = max(min_s1, min_s2), end = min(max_s1, max_s2);
        if(start == end)
            return {start};
        else 
            return {min(start, end), max(start, end)};
    }

    long double a1 = b.y - a.y, a2 = d.y - c.y;
    long double b1 = a.x - b.x, b2 = c.x - d.x;
    long double c1 = a1*a.x + b1*a.y, c2 = a2*c.x + b2*c.y;
    long double det = a1*b2 - a2*b1;
    if(abs(det) > 0.001)
    {
        point inter((b2*c1 - b1*c2)/det, (a1*c2 - a2*c1)/det);
        //cout << "Checking " << inter << " vs segments" << endl;
        //cout << onSegment(inter, s1) << " " << onSegment(inter, s2) << endl;
        if(onSegment(inter, s1) && onSegment(inter, s2))
            return {inter};
    }
    return {};
}

//Squared magnitude of point vector
double sqmag(const point& p1)
{
    return p1.x*p1.x + p1.y*p1.y;
}

//Magnitude of point vector
double mag(const point& p1)
{
    return sqrt(sqmag(p1));
}

//Checks if two segments straddle each other
bool straddle(const segment& s1, const segment& s2)
{
    long double cross1 = cross(s1.p1, s1.p2, s2.p1);
    long double cross2 = cross(s1.p1, s1.p2, s2.p2);

    if((cross1 > 0 && cross2 > 0) || 
       (cross1 < 0 && cross2 < 0)) return false;

    if(abs(cross1) < 0.001 && abs(cross2) < 0.001 &&
       orientation(s1.p2, s2.p1, s2.p2) != Colinear)
       return false;
    
    return true;
}

//Returns distance from line (or segment) to point
long double linePointDist(const segment& s, const point& p, bool isSegment=false)
{
    if(s.p1 == s.p2)
    {
        if(p == s.p1) return 0;
        return mag(p - s.p1);
    }

    if(isSegment)
    {
        if(dot(s.p1, s.p2, p) > 0) return mag(s.p2 - p);
        if(dot(s.p2, s.p1, p) > 0) return mag(s.p1 - p);
    }
    return abs(cross(s.p1, s.p2, p) / mag(s.p1 - s.p2));
}

//Returns positive area if points are counterclockwise,
//negative area if clockwise
long double polyArea(const vector<point>& points)
{
    long double result = 0;
    for(int i=0, j=1; i<points.size(); i++, j=(j+1)%points.size())
    {
        result += points[i].x * points[j].y;
        result -= points[i].y * points[j].x;
    }
    return result/2;
}

//Checks if point p is inside the polygon
//Returns Inside, Outside, or OnEdge
int pointInPoly(const vector<point>& poly, const point& p)
{
    //cout << "Point in poly " << p.x << " " << p.y << endl;
    bool inside = false;

    long double maxX = numeric_limits<long double>::lowest();
    for(const point& p : poly)
        maxX = max(maxX, p.x);

    //Create point definitely outside polygon
    point outside(maxX+1, p.y);

    vector<point> intersection;

    for(int i=0, j = poly.size()-1; i < poly.size(); i++, j=i-1)
    { 
        if(p == poly[i] || p == poly[j]) return OnEdge;
        if(orientation(p, poly[i], poly[j]) == Colinear &&
           onSegment(p, segment{poly[i], poly[j]})) return OnEdge;

        intersection = intersect(segment{p, outside}, segment{poly[i], poly[j]});
        //cout << intersection.size() << " intersections with " << poly[i].x << ", " << poly[i].y << " -> " << poly[j].x << ", " << poly[j].y << endl;
        if(intersection.size() == 1)
        {
            if(poly[i] == intersection[0] && poly[j].y <= p.y) continue;
            if(poly[j] == intersection[0] && poly[i].y <= p.y) continue;
            //cout << intersection[0].x << " " << intersection[0].y << endl;
            inside = !inside;
        }
    }   

    //cout << "Is inside? " << inside << endl;
    return (inside ? Inside : Outside);    
}

//Computes the convex hull of a set of points
//Using the graham scan algorithm
vector<point> convexHull(vector<point> points)
{
    if(points.size() < 4) return points;

    point lowestPoint = points[0];

    //Find lowest, leftmost point
    for(int i=0; i<points.size(); i++)
        if(points[i].y < lowestPoint.y ||
          (points[i].y == lowestPoint.y && 
           points[i].x < lowestPoint.x))
                lowestPoint = points[i];

    //Sort points by polar angle to
    //lowest point
    point horiz = lowestPoint + point(1, 0);
    sort(points.begin(), points.end(),
    [=](const point& l, const point& r)
    {
        if(l == lowestPoint) return true;

        double scoreL = dot(horiz, lowestPoint, l) / (mag(horiz-lowestPoint)*mag(lowestPoint-l));
        double scoreR = dot(horiz, lowestPoint, r) / (mag(horiz-lowestPoint)*mag(lowestPoint-r));
        
        return scoreL < scoreR;
    });

    //Copy last point to first; it's guaranteed
    //part of the hull
    points.insert(points.begin(), points.back());

    //M = number of points to return 
    uint32_t m = 1;

    //Walk around points, adding them to hull
    for(int i=2; i<points.size()-1; i++)
    {
        //Walk backwards removing points as long
        //as there is a 'right turn'
        while(cross(points[m-1], points[m], points[i]) <= 0)
        {
            if(m > 1)
            {
                m--;
                continue;
            }
            //Degenerate case, all points are a line
            else if(i == points.size()-1)
                break;
            else
                i++;
        }

        m++;
        swap(points[m], points[i]);
    }
    points.resize(m+1);
    if(points.front() == points.back()) points.pop_back();

    return points;
}

#include <array>
int main()
{
    /*
    vector<point> test
    { point(0, 0),
      point(1, 1),
      point(-1, -1),
      point(2, 0),
      point(-1, 1),
      point(-2, 1),
      point(2, 1),
      point(2, -1),
      point(0, 1),
      point(-1, 2)};
    */

    /*
    vector<point> test
    { point(-1, -1),
      point(-2, 1),
      point(2, 1),
      point(2, -1),
      point(-1, 2)};
    */

    /*
    vector<point> test
    { point(0, 0),
      point(0, 0),
      point(0, 0),
      point(0, 0),
      point(0, 0)};
    */

    /*
    vector<point> test
    { point(0, 0),
      point(0, 1),
      point(0, 2),
      point(0, 3),
      point(0, 4)};
    */

    vector<point> hull
    {
        point(0, 0),
        point(1, 0),
        point(1, 1),
        point(0, 1),
        point(0, 2),
        point(-1, 2),
        point(-1, 0)
    };
/*
    vector<pair<point, int>> test
    {
      {point(-2, 1),Outside},
      {point(-2, 0),Outside}};

*/

    vector<pair<point, int>> test
    { {point(0, 0), OnEdge},
      {point(0.5, 0),OnEdge},
      {point(0, 0.5),Inside},
      {point(-0.5, 0.5),Inside},
      {point(-0.5, 2),OnEdge},
      {point(-0.5, 1),Inside},
      {point(-2, 1),Outside},
      {point(-2, 0),Outside},
      {point(-2, -1),Outside},
      {point(0, 2.5),Outside},
      {point(-1.5, 2.5),Outside},
      {point(0.5, 1.5),Outside},
      {point(2, 1), Outside}};

    for(auto& p : test)
        cout << p.first.x << " " << p.first.y << " : " << 
            pointInPoly(hull, p.first) << " vs. " << p.second << endl;
}

/*
struct point
{
    long double x, y;

    point(long double x_=0, long double y_=0) : x(x_), y(y_){}
    bool operator <(const point& other) const
    { return (x < other.x ? true : (x == other.x && y < other.y)); }

    bool operator == (const point& other) const
    { return abs(other.x - x) < 0.0001 && abs(other.y - y) < 0.0001; }

    bool operator != (const point& other) const
    { return abs(other.x - x) > 0.0001 || abs(other.y - y) > 0.0001; }

    point operator *(const long double& d) const
    { return point(x*d, y*d); }

    point operator /(const long double& d) const
    { return point(x/d, y/d); }

    point operator +(const point& r) const
    { return point(x+r.x, y+r.y); }

    point operator -(const point& r) const
    { return point(x-r.x, y-r.y); }

    point operator -() const
    { return point(-x, -y); }
};

ostream& operator << (ostream& out, const point& p)
{ return out << "(" << p.x << ", " << p.y << ")"; }

struct segment { point p1, p2; };

constexpr int Colinear = -1;
constexpr int NoIntersect = 0;
constexpr int Intersect = 1;
constexpr int CW = 2;
constexpr int CCW = 3;
constexpr int Inside = 4;
constexpr int Outside = 5;
constexpr int OnEdge = 6;

double dot(const point& a, const point& b, const point& c) {
    point AB = b - a;
    point BC = c - b;
    return AB.x*BC.x + AB.y*BC.y;
}

double cross(const point& A, const point& B, const point& C) {
    point AB = B - A, AC = C - A;
    return(AB.x * AC.y - AB.y * AC.x);
}

int orientation(const point& p, const point& q, const point& r) {
    double val = cross(p, q, r);
    if(abs(val) < 0.0001) return Colinear;
    return (val > 0) ? CW : CCW;
}

bool onSegment(const point& p, const segment& s) {
    bool x = (abs(s.p1.x - s.p2.x) < 0.001 && abs(p.x - s.p2.x) < 0.001) || (p.x <= max(s.p1.x, s.p2.x) && p.x >= min(s.p1.x, s.p2.x));
    bool y = (abs(s.p1.y - s.p2.y) < 0.001 && abs(p.y - s.p2.y) < 0.001) || (p.y <= max(s.p1.y, s.p2.y) && p.y >= min(s.p1.y, s.p2.y));
    return x && y;
}

vector<point> intersect(const segment& s1, const segment& s2) {
    point a = s1.p1, b = s1.p2, c = s2.p1, d = s2.p2;

    if(orientation(a, b, c) == Colinear && orientation(a, b, d) == Colinear &&
        orientation(c, d, a) == Colinear && orientation(c, d, b) == Colinear) {
        point min_s1 = min(a, b), max_s1 = max(a, b);
        point min_s2 = min(c, d), max_s2 = max(c, d);

        if(max_s1 < min_s2 || max_s2 < min_s1) return {};

        point start = max(min_s1, min_s2), end = min(max_s1, max_s2);
        if(start == end)
            return {start};
        else 
            return {min(start, end), max(start, end)};
    }

    long double a1 = b.y - a.y, a2 = d.y - c.y, b1 = a.x - b.x, b2 = c.x - d.x;
    long double c1 = a1*a.x + b1*a.y, c2 = a2*c.x + b2*c.y, det = a1*b2 - a2*b1;
    if(abs(det) > 0.001) {
        point inter((b2*c1 - b1*c2)/det, (a1*c2 - a2*c1)/det);
        if(onSegment(inter, s1) && onSegment(inter, s2))
            return {inter};
    }
    return {};
}

double sqmag(const point& p1)
{ return p1.x*p1.x + p1.y*p1.y; }

double mag(const point& p1)
{ return sqrt(sqmag(p1)); }

bool straddle(const segment& s1, const segment& s2) {
    long double cross1 = cross(s1.p1, s1.p2, s2.p1);
    long double cross2 = cross(s1.p1, s1.p2, s2.p2);

    if((cross1 > 0 && cross2 > 0) || (cross1 < 0 && cross2 < 0)) return false;

    if(abs(cross1) < 0.001 && abs(cross2) < 0.001 &&
       orientation(s1.p2, s2.p1, s2.p2) != Colinear) return false;
    
    return true;
}

long double linePointDist(const segment& s, const point& p, bool isSegment=false) {
    if(s.p1 == s.p2) {
        if(p == s.p1) return 0;
        return mag(p - s.p1);
    }

    if(isSegment) {
        if(dot(s.p1, s.p2, p) > 0) return mag(s.p2 - p);
        if(dot(s.p2, s.p1, p) > 0) return mag(s.p1 - p);
    }
    return abs(cross(s.p1, s.p2, p) / mag(s.p1 - s.p2));
}

long double polyArea(const vector<point>& points) {
    long double result = 0;
    for(int i=0, j=1; i<points.size(); i++, j=(j+1)%points.size()) {
        result += points[i].x * points[j].y;
        result -= points[i].y * points[j].x;
    }
    return result/2;
}

int pointInPoly(const vector<point>& poly, const point& p) {
    bool inside = false;

    long double maxX = numeric_limits<long double>::lowest();
    for(const point& p : poly)
        maxX = max(maxX, p.x);

    point outside(maxX+1, p.y);
    vector<point> intersection;

    for(int i=0, j = poly.size()-1; i < poly.size(); i++, j=i-1) { 
        if(p == poly[i] || p == poly[j]) return OnEdge;
        if(orientation(p, poly[i], poly[j]) == Colinear &&
           onSegment(p, segment{poly[i], poly[j]})) return OnEdge;

        intersection = intersect(segment{p, outside}, segment{poly[i], poly[j]});
        if(intersection.size() == 1) {
            if(poly[i] == intersection[0] && poly[j].y <= p.y) continue;
            if(poly[j] == intersection[0] && poly[i].y <= p.y) continue;
            inside = !inside;
        }
    }   

    return (inside ? Inside : Outside);    
}

vector<point> convexHull(vector<point> points) {
    if(points.size() < 4) return points;
    point lowestPoint = points[0];

    for(int i=0; i<points.size(); i++)
        if(points[i].y < lowestPoint.y ||
          (points[i].y == lowestPoint.y && 
           points[i].x < lowestPoint.x))
                lowestPoint = points[i];

    point horiz = lowestPoint + point(1, 0);
    sort(points.begin(), points.end(),
    [=](const point& l, const point& r) {
        if(l == lowestPoint) return true;

        double scoreL = dot(horiz, lowestPoint, l) / (mag(horiz-lowestPoint)*mag(lowestPoint-l));
        double scoreR = dot(horiz, lowestPoint, r) / (mag(horiz-lowestPoint)*mag(lowestPoint-r));
        
        return scoreL < scoreR;
    });
    points.insert(points.begin(), points.back());

    uint32_t m = 1;
    for(int i=2; i<points.size()-1; i++) {
        while(cross(points[m-1], points[m], points[i]) <= 0) {
            if(m > 1) { m--; continue; }
            else if(i == points.size()-1) break;
            else i++;
        }

        m++;
        swap(points[m], points[i]);
    }
    points.resize(m+1);
    if(points.front() == points.back()) points.pop_back();

    return points;
}
*/