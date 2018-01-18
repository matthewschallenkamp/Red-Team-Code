
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <string>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;
typedef long double ld;

ll chooseNK(ll n, ll k)
{
  static ll table[500][500] = { 0 };
  if(table[n][k] > 0)
    return table[n][k];
  
  if(k < 0 || k > n || n < 0) return 0;
  
  if(k == 0 || k == n)
  {
    table[n][k] = 1;
    return table[n][k];
  }
  
  table[n][k] = chooseNK(n-1, k-1) + chooseNK(n-1, k);
  return table[n][k];
}

struct point
{
    ld x=0, y=0;
    
    point(ld xloc, ld yloc) : x(xloc), y(yloc){}
    point(){}
    point& operator = (const point& other)
    {
      x = other.x; y = other.y;
      return *this;
    }
    
    bool operator == (const point& other) const
    {
      return (other.x == x && other.y == y);
    }
    
    bool operator != (const point& other) const
    {
      return other.x != x || other.y != y;
    }
    
    bool operator < (const point& other) const
    {
      return (x < other.x ? true : (x == other.x && y < other.y));
    }
};

ostream& operator << (ostream& out, point& p)
{
  return out << "(" << p.x << ", " << p.y << ")";
}

struct vect { ld i, j; };
struct segment
{
  point p1, p2;
  segment(point a, point b) : p1(a), p2(b){}
  segment(){}
};

ld crossProduct(point A, point B, point C)
{
  vect AB, AC;
  AB.i = B.x - A.x;
  AB.j = B.y - A.y;
  AC.i = C.x - A.x;
  AC.j = C.y - A.y;
  return (AB.i * AC.j - AB.j * AC.i);
}

int findLeftEdge(vector<point>& pts, vector<point>& treasure, point origin, int startInd)
{
  ////cerr << "Find left edge" << endl;

  int currInd = startInd;
  do
  {
    //Check if any point of the treasure is to the left of the line
    for(point& p : treasure)
    {
      ////cerr << origin << " -> " << pts[currInd % pts.size()] << " / " << p << " : " << crossProduct(pts[currInd % pts.size()], origin, p) << endl;
      if(crossProduct(origin, pts[currInd % pts.size()], p) > 0)
      {
        //If so, move back one
        return (currInd-1) % pts.size();
      }
    }     
  }while((++currInd) % pts.size() != startInd);

  throw logic_error("Unable to find left edge");
}

int findRightEdge(vector<point>& pts, vector<point>& treasure, point origin, int startInd)
{
  int currInd = startInd;
  bool done;
  ////cerr << "Find right edge" << endl;
  do
  {
    done = true;
    
    //Check if all points of the treasure are to the left of the line
    for(point& p : treasure)
    {
      ////cerr << origin << " -> " << pts[currInd % pts.size()] << " / " << p << " : " << crossProduct(pts[currInd % pts.size()], origin, p) << endl;
      if(crossProduct(origin, pts[currInd % pts.size()], p) < 0)
      {
        //If not, keep searching
        done = false;
        break;
      }
    }     
    if(done) return currInd % pts.size();
  }while((++currInd) % pts.size() != startInd);

  throw logic_error("Unable to find right edge");
}

void doCase()
{
  int L, W;
  cin >> L >> W;

  ld x, y, w;
  cin >> x >> y >> w;
  ll tot_pts = 2*L + 2*W;
  
  //Add all points around the room to a vector
  //left wall, then top wall, then right wall, then bottom wall
  vector<point> pts;
  for(int i = 0; i < L; i++)
  {
    pts.emplace_back(0, i);
  }
  for(int i = 0; i < W; i++)
  {
    pts.emplace_back(i, L);
  }
  for(int i = L; i > 0; i--)
  {
    pts.emplace_back(W, i);
  }
  for(int i = W; i > 0; i--)
  {
    pts.emplace_back(i, 0);
  }
  
  //Add treasure points to vector
  vector<point> treasure;
  treasure.emplace_back(x, L-y);
  treasure.emplace_back(x+w, L-y);
  treasure.emplace_back(x+w, L-y-w);
  treasure.emplace_back(x, L-y-w);
  
  //Find range of points that cause intersections for point 0
  int b_begin = findLeftEdge(pts, treasure, pts[0], 1);
  int b_end = findRightEdge(pts, treasure, pts[0], b_begin+1);
  
  //Walk around the outside and find that range for each point
  //Use previous range to find new range in O(1) time amortized
  vector<pair<int, int>> ranges(pts.size());
  for(int i=0; i<pts.size(); i++)
  {      
      if(i == b_begin % pts.size()) b_begin++;

      //Wasted test on first time, but should be harmless      
      b_begin = findLeftEdge(pts, treasure, pts[i], b_begin);
      b_end = findRightEdge(pts, treasure, pts[i], b_end);

      if(i == b_end % pts.size()) b_end--;

      ranges[i] = make_pair(b_begin, b_end);
      //cerr << "Range " << i << ": " << b_begin << " -> " << b_end << endl;
  }

  //Counters for number of triangles that intersect
  //the treasure 1, 2, or 3 times
  ll naive_single = 0;
  ll naive_double = 0;
  ll naive_triple = 0;

  //Now check each point to see how many triangles it's part of with
  //1 or 2 intersections on the box, and use the collected ranges
  //to find how many it's part of with 3 intersections
  //O(n^2)
  for(int i=0; i<pts.size(); i++)
  {
    pair<int, int>& range1 = ranges[i];

    ll intersectsR1 = range1.second - range1.first - 1;

    bool range1Crossed = false;
    if(range1.second < range1.first)
    {
      intersectsR1 = (range1.second + pts.size()) - range1.first - 1;
      range1Crossed = true;
    }

    //Count each non-intersecting point as a vertex of a triangle
    //with each point that does intersect
    ll singles = 0;
    ll doubles = 0;
    ll triples = 0;

    //If at least 2 points are in the are of intersections, count 
    //How many trianges can use the origin and two points in the intersection area
    if(intersectsR1 != 0)
    {
      //For each intersect point, check how many of it's intersecting points
      //fall in both ranges; those cause a triple intersection
      for(int j=(range1.first + 1) % pts.size(); j != range1.second; j = (j + 1) % pts.size())
      {
        pair<int, int>& range2 = ranges[j]; 
        
        ll intersectsR2 = range2.second - range2.first - 1;

        bool range2Crossed = false;
        if(range2.second < range2.first)
        {
          intersectsR2 = (range2.second + pts.size()) - range2.first - 1;
          range2Crossed = true;
        }

        //cerr << "Pts " << i << ", " << j << endl;
        //cerr << intersectsR1 << ", " << intersectsR2 << endl;

        //Find the number of points that are...
        //  Not in range 1 or 2
        //  In range 2 but not range 1
        //  In both ranges 1 and 2
        //
        //The current point (in range 1) can be used with points in
        //  the first category to make triangles that intersect 1 time
        //
        //It can be used with points in the second category to
        //  make triangles that intersect 2 times
        //
        //It can be used with points in the third category to 
        //  make trianges that intersect 3 times
        //
        //It is also known that range2.first >= range1.first and
        //range2.second >= range2.second
        ll inRange2;
        ll outRange12;
        ll inRange12;

        //Neither range crosses the 0th pt
        //(easy case)
        //cerr << "Case ";
        if(!range1Crossed && !range2Crossed)
        {
          //cerr << "NN";
          bool start2In1 = range2.first >= range1.first && range2.first < range1.second-1;
          bool end2In1 = range2.second > range1.first + 1 && range2.second <= range1.second;
          //Ranges don't intersect
          if(range2.first >= range1.second - 1 || range2.second <= range1.first + 1)
          {
            //cerr << "0" << endl;
            inRange2 = intersectsR2;
            outRange12 = tot_pts - intersectsR2 - intersectsR1;
            inRange12 = 0;
          }
          else
          {
            //Range2 within range1
            if(start2In1 && end2In1)
            {
              //cerr << "1" << endl;
              inRange2 = 0;
              outRange12 = tot_pts - intersectsR1;
              inRange12 = intersectsR2;
            }
            //Range2 overlaps right side of range1
            else if(start2In1 && !end2In1)
            {
              //cerr << "2" << endl;
              inRange2 = range2.second - range1.second;
              outRange12 = tot_pts - (range2.second - range1.first - 1);
              inRange12 = range1.second - range2.first - 1;
            }
            //Range2 overlaps left side of range1
            else if(!start2In1 && end2In1)
            {
              //cerr << "3" << endl;
              inRange2 = range1.first - range2.first;
              outRange12 = tot_pts - (range1.second - range2.first - 1);
              inRange12 = range2.second - range1.first - 1;
            }
            //Range1 within range2
            else
            {
              //cerr << "4" << endl;
              inRange2 = intersectsR2 - intersectsR1;
              outRange12 = tot_pts - intersectsR2;
              inRange12 = intersectsR1;
            }
          }

          //Don't count point i
          if(i > range2.first && i < range2.second) inRange2--;
          else outRange12--;
        }
        else if(range1Crossed && !range2Crossed)
        {
          //cerr << "YN";
          bool start2In1 = range2.first >= range1.first || range2.first < range1.second-1;
          bool end2In1 = range2.second > range1.first + 1 || range2.second <= range1.second;

          //Ranges don't intersect
          if(range2.first >= range1.second - 1 && range2.second <= range1.first + 1)
          {
            //cerr << "0" << endl;
            inRange2 = intersectsR2;
            outRange12 = tot_pts - intersectsR2 - intersectsR1;
            inRange12 = 0;
          }
          else
          {
            //Range2 within range1
            //Or wraps all the way around
            if(start2In1 && end2In1)
            {
              bool mobiusDoubleReacharound = range2.first < range1.second-1;
              if(mobiusDoubleReacharound)
              {
                //cerr << "1y" << endl;
                inRange2 = tot_pts - intersectsR1;
                inRange12 = intersectsR2 - inRange2;
                outRange12 = 0;
              }
              else
              {
                //cerr << "1n" << endl;
                inRange2 = 0;
                outRange12 = tot_pts - intersectsR1;
                inRange12 = intersectsR2;
              }
            }
            //Range2 overlaps right side of range1
            else if(start2In1 && !end2In1)
            {
              //cerr << "2" << endl;
              inRange2 = range2.second - range1.second;
              outRange12 = tot_pts - (range2.second + pts.size() - range1.first - 1);
              inRange12 = range1.second - range2.first - 1;
            }
            //Range2 overlaps left side of range1
            else if(!start2In1 && end2In1)
            {
              //cerr << "3" << endl;
              inRange2 = range1.first - range2.first;
              outRange12 = tot_pts - (range1.second+pts.size() - range2.first - 1);
              inRange12 = range2.second - range1.first - 1;
            }
            //Range1 within range2
            else
            {
              throw logic_error("Impossible situation 1");
            }
          }

          //Don't count point i
          if(i > range2.first && i < range2.second) inRange2--;
          else outRange12--;
        }
        else if(!range1Crossed && range2Crossed)
        {
          //cerr << "NY";
          bool start2In1 = range2.first >= range1.first && range2.first < range1.second-1;
          bool end2In1 = range2.second > range1.first + 1 && range2.second <= range1.second;

          //Ranges don't intersect
          if(range2.first >= range1.second - 1 && range2.second <= range1.first + 1)
          {
            //cerr << "0" << endl;
            inRange2 = intersectsR2;
            outRange12 = tot_pts - intersectsR2 - intersectsR1;
            inRange12 = 0;
          }
          else
          {
            //Range2 within range1
            //Or wraps all the way around
            if(start2In1 && end2In1)
            {
              bool mobiusDoubleReacharound = range2.first < range1.second-1;
              if(mobiusDoubleReacharound)
              {
                //cerr << "1y" << endl;
                inRange2 = tot_pts - intersectsR1;
                inRange12 = intersectsR2 - inRange2;
                outRange12 = 0;
              }
              else
              {
                throw logic_error("Impossible situation 2");
              }
            }
            //Range2 overlaps right side of range1
            else if(start2In1 && !end2In1)
            {
              //cerr << "2" << endl;
              inRange2 = range2.second+pts.size() - range1.second;
              outRange12 = tot_pts - (range2.second + pts.size() - range1.first - 1);
              inRange12 = range1.second - range2.first - 1;
            }
            //Range2 overlaps left side of range1
            else if(!start2In1 && end2In1)
            {
              //cerr << "3" << endl;
              inRange2 = range1.first+pts.size() - range2.first;
              outRange12 = tot_pts - (range1.second+pts.size() - range2.first - 1);
              inRange12 = range2.second - range1.first - 1;
            }
            //Range1 within range2
            else
            {
              //cerr << "4" << endl;
              inRange2 = intersectsR2 - intersectsR1;
              outRange12 = tot_pts - intersectsR2;
              inRange12 = intersectsR1;
            }
          }

          //Don't count point i
          if(i > range2.first || i < range2.second) inRange2--;
          else outRange12--;
        }
        else if(range1Crossed && range2Crossed)
        {
          //cerr << "YY";
          bool start2In1 = range2.first >= range1.first || range2.first < range1.second - 1;
          bool end2In1 = range2.second <= range1.second || range2.second > range1.first + 1;

          //Ranges don't intersect
          if((range1.second == 0 && range2.first == pts.size()-1) ||
            (range1.first == pts.size() - 1 && range2.second == 0))
          {
            inRange2 = intersectsR2;
            inRange12 = 0;
            outRange12 = tot_pts - intersectsR1 - intersectsR2;
          }
          else
          {
            //Range2 within range1
            //Or wraps all the way around
            if(start2In1 && end2In1)
            {
              bool mobiusDoubleReacharound = ((range2.first < range1.second) == (range2.second < range1.second));
              if(mobiusDoubleReacharound)
              {
                //cerr << "1y" << endl;
                inRange2 = tot_pts - intersectsR1;
                inRange12 = intersectsR1 - (tot_pts - intersectsR2);
                outRange12 = 0;
              }
              else
              {
                //cerr << "1n" << endl;
                inRange2 = 0;
                inRange12 = intersectsR2;
                outRange12 = tot_pts - intersectsR1;
              }
            }
            //Range2 overlaps right side of range1
            else if(start2In1 && !end2In1)
            {
              //cerr << "2" << endl;
              inRange2 = range2.second - range1.second;
              outRange12 = tot_pts - (range2.second + pts.size() - range1.first - 1);
              inRange12 = range1.second + pts.size() - range2.first - 1;
            }
            //Range2 overlaps left side of range1
            else if(!start2In1 && end2In1)
            {
              //cerr << "2" << endl;
              inRange2 = range1.first - range2.first;
              outRange12 = tot_pts - (range1.second+pts.size() - range2.first - 1);
              inRange12 = range2.second + pts.size() - range1.first - 1;
            }
            //Range1 within range2
            else
            {
              //cerr << "3" << endl;
              inRange2 = intersectsR2 - intersectsR1;
              outRange12 = tot_pts - intersectsR2;
              inRange12 = intersectsR1;
            }
          }

          //Don't count point i
          if(i > range2.first || i < range2.second) inRange2--;
          else outRange12--;
        }

        //cerr << "In neither range: " << outRange12 << endl;
        //cerr << "In only range 2: " << inRange2 << endl;
        //cerr << "In both ranges: " << inRange12 << endl;

        singles += outRange12;
        doubles += inRange2;
        triples += inRange12;
      }

      //cerr << "Pt " << i << endl;
      //cerr << "Single intersectsR1: " << singles << endl;
      //cerr << "Double intersectsR1: " << doubles << endl;
      //cerr << "Triple intersectsR1: " << triples << endl;
    }
    naive_single += singles;
    naive_double += doubles;
    naive_triple += triples;
  }

  //Total possible triangles in the room
  //Total triangles - all triangles that have 3 points on the same wall
  ll naive_total = chooseNK(tot_pts, 3) - 2 * chooseNK(W + 1, 3) - 2 * chooseNK(L + 1, 3);
  
  if((naive_single % 2) || (naive_double % 2) || (naive_triple % 6))
    throw logic_error("Answer not even");

  //cerr << naive_total << " total triangles" << endl;
  //cerr << naive_single << " -> " << naive_single/2 << " single intersect triangles" << endl;
  //cerr << naive_double << " -> " << naive_double/2 << " double intersect triangles" << endl;
  //cerr << naive_triple << " -> " << naive_triple/6 << " triple intersect triangles" << endl;
  //cerr << string(20, '-') << endl;

  cout << naive_total - naive_single/2 - naive_double/2 - naive_triple/6 << endl;
}

int main()
{
  int cases;
  cin >> cases;
  for(int i=0; i<cases; i++)
    doCase();
    
  return 0;
}






