#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

const double EPSILON = 0.000000000001;

struct point
{
    long double x, y;

    point(long double x_=0, long double y_=0) : x(x_), y(y_){}
    bool operator <(const point& other) const
    { return (x < other.x ? true : (x == other.x && y < other.y)); }

    bool operator == (const point& other) const
    { return abs(other.x - x) < EPSILON && abs(other.y - y) < EPSILON; }

    bool operator != (const point& other) const
    { return abs(other.x - x) > EPSILON || abs(other.y - y) > EPSILON; }

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
    long double val = cross(p, q, r);
    if(abs(val) < EPSILON) return Colinear;
    return (val > 0) ? CW : CCW;
}

bool onSegment(const point& p, const segment& s) {
    bool x = (abs(s.p1.x - s.p2.x) < EPSILON && abs(p.x - s.p2.x) < EPSILON) || (p.x <= max(s.p1.x, s.p2.x) && p.x >= min(s.p1.x, s.p2.x));
    bool y = (abs(s.p1.y - s.p2.y) < EPSILON && abs(p.y - s.p2.y) < EPSILON) || (p.y <= max(s.p1.y, s.p2.y) && p.y >= min(s.p1.y, s.p2.y));
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
    if(abs(det) > EPSILON) {
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

    if(abs(cross1) < EPSILON && abs(cross2) < EPSILON &&
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

    //Don't just use point < operator because that checks
    //x then y; we need y then x
    for(int i=0; i<points.size(); i++)
        if(points[i].y < lowestPoint.y || 
          (abs(points[i].y - lowestPoint.y) < EPSILON && points[i].x < lowestPoint.x))
                lowestPoint = points[i];

    point horiz = lowestPoint + point(1, 0);
    sort(points.begin(), points.end(),
    [=](const point& l, const point& r) {
        if(l == lowestPoint) return true;

        long double scoreL = dot(horiz, lowestPoint, l) / (mag(horiz-lowestPoint)*mag(lowestPoint-l));
        long double scoreR = dot(horiz, lowestPoint, r) / (mag(horiz-lowestPoint)*mag(lowestPoint-r));
        
        return scoreL < scoreR;
    });
    points.insert(points.begin(), points.back());

    /*
    cerr << "Hull check points" << endl;
    for(point& p : points)
        cerr << p << " ";
    cerr << endl;
    */

    uint32_t m = 1;
    for(int i=2; i<points.size(); i++) {
        while(cross(points[m-1], points[m], points[i]) <= -EPSILON) {
            if(m > 1) { /*cerr << "Remove " << points[m];*/ m--; continue; }
            else if(i == points.size()-1) 
            {
                //cerr << "Colinear" << endl;
                break;
            }
            else i++;
        }

        //cerr << "Add " << points[i] << endl;
        m++;
        std::swap(points[m], points[i]);
    }
    points.resize(m);
    if(points.front() == points.back()) points.pop_back();

    return points;
}

typedef unsigned long long ull;
int main()
{
    ull n, r;
    cin >> n >> r;
    
    vector<point> candles(n);
    for(point& c : candles)
        cin >> c.x >> c.y;

    if(n < 3)
    {
        cout << 0 << endl;
        return 0;
    }

    vector<point> hull = convexHull(candles);

/*
    cerr << "Hull: " << endl;
    for(point& p : hull)
        cerr << p << " ";
    cerr << endl;
*/
    long double minDist = numeric_limits<long double>::max();
    long double currDist = 0;
    int wInd = 0;
    for(int i=0, j=hull.size()-1; i < hull.size(); i++, j=i-1)
    {
        segment s{hull[i], hull[j]};
        //cerr << "Check segment " << hull[i] << ", " << hull[j] << endl;

        currDist = linePointDist(s, hull[(wInd)%hull.size()]);
        long double dist;
        int oldWind = wInd;
        bool moved = false;
        while((dist = linePointDist(s, hull[(wInd+1)%hull.size()])) >= (currDist - EPSILON))
        {
            //cerr << "Move past point " << (hull[(wInd)%hull.size()]) << " -> " << linePointDist(s, hull[(wInd)%hull.size()]) << endl;
            currDist = dist;
            wInd = (wInd + 1) % hull.size();
            moved = true;
            if(wInd == oldWind) break;
        }
        if(wInd == oldWind && moved)
        {
            //cerr << "Colinear!" << endl;
            minDist = 0;
            break;
        }

        minDist = min(minDist, currDist);
        //cerr << "Min: " << minDist << endl;
    }
 
    cout << setprecision(10) << fixed << minDist << endl;
}