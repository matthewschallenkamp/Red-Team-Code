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
    double x, y;

    int owningPoly; //Index of subpoly owning this point
    int matchingPoly; //Index of subpoly owning matched point
    int matchingIndex = -1; //Index of matched point in other sub-poly; > -1 -> match exists

    bool marked = false;
    point(double x_=0, double y_=0) : x(x_), y(y_){}

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

    point operator *(const double& d) const
    {
        return point(x*d, y*d);
    }

    point operator /(const double& d) const
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



struct segment
{
    point p1, p2;
};

constexpr int Colinear = -1;
constexpr int NoIntersect = 0;
constexpr int Intersect = 1;
constexpr int CW = 2;
constexpr int CCW = 3;

double dot(const point& a, const point& b)
{
    return a.x*b.x + a.y*b.y;
}

double cross(const point& A, const point& B, const point& C)
{
    point AB, AC;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    return(AB.x * AC.y - AB.y * AC.x);
}

int orientation(const point& p, const point& q, const point& r)
{
    double val = cross(p, q, r);
    if(abs(val) < 0.0001) return Colinear;
    return (val > 0) ? CW : CCW;
}

int intersect(const segment& s1, const segment& s2, point& intersection)
{
    point a = s1.p1, b = s1.p2, c = s2.p1, d = s2.p2;

    if(orientation(a, b, c) == 0 && orientation(a, b, d) == 0 &&
        orientation(c, d, a) == 0 && orientation(c, d, b) == 0)
    {
        point min_s1 = min(a, b), max_s1 = max(a, b);
        point min_s2 = min(c, d), max_s2 = max(c, d);

        if(min_s1 < min_s2)
        {
            if(max_s1 < min_s2) return NoIntersect;
        }
        else if(min_s2 < min_s1 && max_s2 < min_s1) return NoIntersect;

        point start = max(min_s1, min_s2), end = min(max_s1, max_s2);
        if(start == end)
        {
            intersection = start;
            return Intersect;
        }
        else return Colinear;
    }

    double x1 = (b.x - a.x), y1 = (b.y - a.y), x2 = (d.x - c.x), y2 = (d.y - c.y);
    double u1 = (-y1 * (a.x - c.x) + x1 * (a.y - c.y)) / (-x2 * y1 + x1 * y2);
    double u2 = (x2 * (a.y - c.y) - y2 * (a.x - c.x)) / (-x2 * y1 + x1 * y2);

    if(u1 >= 0 && u1 <= 1 && u2 >= 0 && u2 <= 1)
    {
        intersection = point(a.x + u2 * x1, a.y + u2 * y1);
        return Intersect;
    }
    return NoIntersect;
}


ostream& operator <<(ostream& out, const point& p)
{
    return out << "(" << p.x << ", " << p.y << ": " << p.matchingPoly << ", " << p.matchingIndex << ")";
}

double sqmag(const point& p1)
{
    return p1.x*p1.x + p1.y*p1.y;
}

double mag(const point& p1)
{
    return sqrt(sqmag(p1));
}

//Returns positive area if points are counterclockwise,
//negative area if clockwise
double polyArea(const vector<point>& points)
{
    double result = 0;
    for(int i=0, j=1; i<points.size(); i++, j=(j+1)%points.size())
    {
        result += points[i].x * points[j].y;
        result -= points[i].y * points[j].x;
    }
    return result/2;
}

void reverseIfCounterClockwise(vector<point>& points)
{
    if(polyArea(points) > 0)
    {
        //cerr << "Points given counterclockwise" << endl;
        reverse(points.begin(), points.end());
    }
}

void splitPoints(const vector<point>& fullShape, vector<point>& leftShape, vector<point>& rightShape, const point& linea,const point& lineb)
{
    bool left=false;
    bool right = false;
    for(const point& p : fullShape)
    {
        double cProd = cross(linea, p, lineb);
        if(abs(cProd) < 0.001)
        {
            leftShape.push_back(p);
            rightShape.push_back(p);
        }
        else if(cProd < 0)
        {
            left = true;
            leftShape.push_back(p);
        }
        else
        {
            right = true;
            rightShape.push_back(p);
        }
    }

    //If we never insert a point that is wholly on one side of the line
    //we don't return any points for that side; the answer is 0
    if(!left) leftShape.clear();
    if(!right) rightShape.clear();
}

void reflect(vector<point>& points, const point& linea, const point& lineb)
{
    point vline = linea-lineb;
    double vlinesqmag = sqmag(vline);
    for(point& p : points)
    {
        point pline = p - lineb;
        point project = lineb + (vline * dot(pline, vline))/vlinesqmag;
        p = p + (project-p)*2;
    }

    reverse(points.begin(), points.end());
}


void insertIntersections(vector<point>& points, const point& linea, const point& lineb)
{
    segment bigLine = {linea, lineb};
    point intersection;

    for(int i=0, j=1; i < points.size(); i++, j=(j+1)%points.size())
    {
        int doesIntersect = intersect(bigLine, segment{points[i], points[j]}, intersection);
    
        if(intersection == point(46.7975, -79.9488))
        {
            cerr << "Found point: " << doesIntersect << endl;
            cerr << linea << " -> " << lineb << endl;
            cerr << points[i] << " -> " << points[j] << endl;
        }

        if(doesIntersect == Intersect
           && intersection != points[i] && intersection != points[j])
        {
            if(j != 0)
            {
                points.insert(points.begin()+j, intersection);
            }
            else
            {
                points.push_back(intersection);
                j = points.size()-1;
            }
        }
    }
}

void insertPoints(vector<point>& set, vector<vector<point>>& points)
{
    vector<point> result;
    for(int i=0; i<set.size(); i++)
    {
        point& p1 = set[i];

        //Sort points inserted after point i in order
        //of furthest to closest to point i
        sort(points[i].begin(), points[i].end(), 
        [p1](const point& left, const point& right)
        {
            return sqmag(left-p1) < sqmag(right-p1);
        });

        result.push_back(p1);
        for(point& p : points[i])
        {  
            //Avoid duplicate points
            if(result.back() != p)
                result.push_back(p);
        }
    }

    set = result;
}


void insertIntersections(vector<point>& set1, vector<point>& set2)
{
    //Tally of points to insert into the sets,
    //each index represents all the points inserted
    //immediately after that point on the corresponding poly
    //they can be sorted to be inserted in the correct order
    //after they are all found
    vector<vector<point>> s1Inserts(set1.size());
    vector<vector<point>> s2Inserts(set2.size());

    for(int i=0, j=set1.size()-1; i < set1.size(); i++, j=i-1)
    {
        point& p1 = set1[i], &p2 = set1[j];
        segment s1{p1, p2};

        for(int k=0, l=set2.size()-1; k < set2.size(); k++, l=k-1)
        {
            point& p3 = set2[k], &p4 = set2[l];

            point inter;

            //Check if there's an intersection
            if(intersect(s1, segment{p3, p4}, inter) == Intersect)
            {
                cerr << p1 << " -> " << p2 << " : " << p3 << " -> " << p4 << " = " << inter << endl;

                //Check if it's a new point to poly 1
                if(inter != p1 && inter != p2)
                    s1Inserts[j].push_back(inter);

                //Check if it's a new point to poly 2
                if(inter != p3 && inter != p4)
                    s2Inserts[l].push_back(inter);
            }
        }
    }

    insertPoints(set1, s1Inserts);
    insertPoints(set2, s2Inserts);
}

vector<vector<point>> splitPoly(const vector<point>& points)
{
    //cerr << "Split polygon" << endl;

    map<pair<point, point>, uint64_t> edges;
    vector<vector<point>> out;
    stack<point> travel;

    for(int i=0, j=points.size()-1; i < points.size(); i++, j=i-1)
    {
        //cerr << "Check edge " << points[j] << " " << points[i] << endl;
        pair<point, point> edge = make_pair(points[j], points[i]);
        auto iter = edges.find(edge);

        //If edge already exists, add it as a delimiter
        //for both times it exists
        if(iter != edges.end())
        {
            //cerr << "Duplicate edge: " << points[j] << " " << points[i] << " at indecies " << iter->second << ", " << j << endl;

            vector<point> loop;

            point match = travel.top();
            travel.pop();
            //cerr << "Loop start at " << match << endl;

            point back;
            do
            {
                back = travel.top();
                back.owningPoly = out.size();
                travel.pop();
                //cerr << "Backtrack " << back << endl;

                //Push front to preserve winding
                loop.insert(loop.begin(), back);
            }while(back != match);

            if(loop.size() > 2)
                out.push_back(loop);
        }
        else
        {
            //Insert opposite direction edge; because if it's there twice,
            //that's what'll match the second time
            //Store the index of the first point on the edge
            edges[make_pair(points[i], points[j])] = j;

            travel.push(points[i]);
        }
    }

    vector<point> loop;
    while(travel.size())
    {
        point back = travel.top();
        //cerr << "Backtrack " << back << endl;

        back.owningPoly = out.size();
        loop.insert(loop.begin(), back);
        travel.pop();
    }

    if(loop.size() > 2)
        out.push_back(loop);

    return out;
}

void matchIntersections(vector<vector<point>>& poly1, vector<vector<point>>& poly2)
{
    //For every point in the first group
    for(int i=0; i<poly1.size(); i++)
        for(int j=0; j<poly1[i].size(); j++)
        {
            bool matched = false;

            //Find the point in the second group that is the same point
            for(int k=0; k<poly2.size() && !matched; k++)
                for(int l=0; l<poly2[k].size() && !matched; l++)
                    //Link the points together
                    if(poly1[i][j] == poly2[k][l])
                    {
                        poly1[i][j].matchingPoly = k;
                        poly1[i][j].matchingIndex = l;
                        poly2[k][l].matchingPoly = i;
                        poly2[k][l].matchingIndex = j;

                        matched = true;
                    }
        }

}

double sumAreas(vector<vector<point>>& set1, vector<vector<point>>& set2)
{
    double result = 0;

    for(int i=0; i < set1.size(); i++)
        for(int j=0; j < set1[i].size(); j++)
        {
            //cerr << "Loop poly 1: " << i << ", " << j << " : " << set1[i][j] << endl;

            //Find an intersect point that we haven't used yet
            if(set1[i][j].matchingIndex >= 0 && !set1[i][j].marked)
            {
                //cerr << "New intersection!" << endl;
                vector<point> polygon;

                //Polygon and index looked at in current set
                int ind = j;
                vector<point>* poly = &set1[i];

                //Sets available
                vector<vector<point>>* curr = &set1;
                vector<vector<point>>* other = &set2;

                //At start of loop:
                //ind and poly are current intersection point in
                //set curr
                bool done = false;
                while(!done)
                {
                    //Add intersecting point to polygon
                    point& toAdd = (*poly)[ind];
                    toAdd.marked = true;

                    vector<point>* otherPoly = &(*other)[toAdd.matchingPoly];
                    (*otherPoly)[toAdd.matchingIndex].marked = true;

                    //cerr << "Internal loop: " << toAdd << endl;
                    
                    //If it matches the first point; we've finished the loop
                    if(polygon.size() && toAdd == polygon[0])
                    {
                        done = true;
                        break;
                    }
                    polygon.push_back(toAdd);

                    //Check if switching to other polygon yields a right turn
                    point currNext = (*poly)[(ind+1 % poly->size())];
                    point otherNext = (*otherPoly)[(toAdd.matchingIndex + 1) % otherPoly->size()];

                    double c = cross(toAdd, currNext, otherNext);
                    //cerr << "Internal loop: cross " << toAdd << ", " << currNext << ", " << otherNext << endl;
                    //cerr << "Internal loop: " << c << endl;

                    //If right turn, switch polygons
                    if(c < 0)
                    {
                        //cerr << "Internal loop: switch polygons" << endl;
                        swap(curr, other);

                        poly = &(*curr)[toAdd.matchingPoly];
                        ind = toAdd.matchingIndex;
                    }

                    //Continue until next intersection
                    do
                    {
                        //Step to nxt point
                        ind = (ind+1) % poly->size();
                        point& pt = (*poly)[ind];

                        //cerr << "Walk to: " << pt << endl;

                        //Check if it has an intersection
                        if(pt.matchingIndex < 0)
                        {
                            //If it matches the first point; we've finished the loop
                            //Not sure if this is possible during this loop
                            if(polygon.size() && pt == polygon[0])
                            {
                                done = true;
                                break;
                            }
                            polygon.push_back(pt);
                        }
                    }while((*poly)[ind].matchingIndex < 0);
                }
                //cerr << "Find area of " << polygon.size() << " points" << endl;
                result += abs(polyArea(polygon));
                //cerr << endl;
            }
        }

    return result;
}

int main()
{
    int n;
    cin >> n;
    
    vector<point> points(n);
    for(point& p : points)
        cin >> p.x >> p.y;

    cerr << "Check for winding" << endl;
    //Points not guaranteed to be clockwise or counterclockwise
    //so if counterclockwise, reverse them
    reverseIfCounterClockwise(points);

    //Read points to define line
    //and move them outside the shape guaranteed
    //based on max size of inputs
    point a_, b_;
    cin >> a_.x >> a_.y >> b_.x >> b_.y;

    cerr << "Compute line" << endl;
    point ab = a_-b_;
    point a = b_ + (ab/mag(ab))*1500;
    point b = a_ + (-ab/mag(ab))*1500;

    //cerr << "Point A: " << a_ << " -> " << a << endl;
    //cerr << "Point B: " << b_ << " -> " << b << endl;

    cerr << "Intersect with line" << endl;
    //Add points anywhere the shape intersects the line
    insertIntersections(points, a, b);

    cerr << "Split points" << endl;
    //Split the points based on which side of the line they are on
    vector<point> leftPoints, rightPoints;
    splitPoints(points, leftPoints, rightPoints, a, b);

    for(point& p : leftPoints)
        cerr << p << " ";
    cerr << endl << endl;

    for(point& p : rightPoints)
        cerr << p << " ";
    cerr << endl << endl;

    //If either side of the shape has no points, then
    //the max area is 0
    if(!leftPoints.size() || !rightPoints.size())
    {
        cout << fixed << setprecision(9) << 0 << endl;
        return 0.0;
    }

    cerr << "Reflect" << endl;
    //Reflect all points on one side across the line
    reflect(leftPoints, a, b);

    cerr << "Intersect halves" << endl;
    //Find all intersections of the two half-shapes
    //and insert those points into both sides
    insertIntersections(leftPoints, rightPoints);

    for(point& p : leftPoints)
        cerr << p << " ";
    cerr << endl << endl;

    for(point& p : rightPoints)
        cerr << p << " ";
    cerr << endl << endl;

    cerr << "Split pieces" << endl;
    //Split polygons apart so no edges are used twice
    vector<vector<point>> leftSplit, rightSplit;
    leftSplit = splitPoly(leftPoints);
    rightSplit = splitPoly(rightPoints);

    cerr << "Split into " << leftSplit.size() << " and " << rightSplit.size() << " subpolys" << endl;
    
    for(vector<point>& poly : leftSplit)
    {
        for(point p : poly)
            cerr << p.x << " " << p.y << endl;
        cerr << poly[0].x << " " << poly[0].y << endl;
        cerr << endl;
    }
    cerr << endl;

    for(vector<point>& poly : rightSplit)
    {
        for(point p : poly)
            cerr << p.x << " " << p.y << endl;
        cerr << poly[0].x << " " << poly[0].y << endl;
        cerr << endl;
    }
    cerr << endl;
    

    cerr << "Match intersections" << endl;
    //Match points that are the same on both polygon groups
    matchIntersections(leftSplit, rightSplit);

    //cerr << "Polygons processed:" << endl;
    /*
    for(vector<point>& poly : leftSplit)
    {
        for(point p : poly)
            //cerr << p << " ";
        //cerr << endl;
    }
    //cerr << endl;

    for(vector<point>& poly : rightSplit)
    {
        for(point p : poly)
            //cerr << p << " ";
        //cerr << endl;
    }
    //cerr << endl;
    */

    cerr << "Solve" << endl;
    //Walk around one shape finding shared areas and summing
    //them
    cout << fixed << setprecision(9) << sumAreas(leftSplit, rightSplit) << endl;
}