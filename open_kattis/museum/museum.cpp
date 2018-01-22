
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

pair<int, bool> findLeftEdge(vector<point>& pts, vector<point>& treasure, point origin, int startInd)
{
  ////cerr << "Find left edge" << endl;

  int currInd = startInd;
  int crosses;
  do
  {
    if(pts[currInd] == origin) continue;
    crosses = 0;

    //Check if any point of the treasure is to the left of the line
    for(point& p : treasure)
    {
      ////cerr << origin << " -> " << pts[currInd] << " -> " << p << " = " << crossProduct(origin, pts[currInd], p) << endl;
      if(crossProduct(origin, pts[currInd], p) > 0)
      {
        crosses++;
      }
    }     
    if(crosses > 0) return make_pair(currInd, crosses!=4);
  }while((currInd = ((currInd+1) % pts.size())) != startInd);

  throw logic_error("Unable to find left edge");
}

int findRightEdge(vector<point>& pts, vector<point>& treasure, point origin, int startInd)
{
  ////cerr << "Find right edge" << endl;

  int currInd = startInd;
  bool done;
  do
  {
    if(pts[currInd] == origin) continue;
    done = true;
    
    //Check if all points of the treasure are to the left of the line
    for(point& p : treasure)
    {
      ////cerr << origin << " -> " << pts[currInd] << " -> " << p << " = " << crossProduct(origin, pts[currInd], p) << endl;
      if(crossProduct(origin, pts[currInd], p) < 0)
      {
        //If not, keep searching
        done = false;
        break;
      }
    }     
    //Find the last point that intersects
    if(done) return currInd;
  }while((currInd = ((currInd+1) % pts.size())) != startInd);

  throw logic_error("Unable to find right edge");
}

int union_size(vector<pair<int, int>>& ranges1, vector<pair<int, int>>& ranges2)
{
  if(!ranges1.size() && !ranges2.size()) return 0;

  vector<int> starts;
  vector<int> ends;
  int startInd = 0, endInd = 0;

  for(auto& p : ranges1)
  {
    starts.push_back(p.first);
    ends.push_back(p.second);
  }

  for(auto& p : ranges2)
  {
    starts.push_back(p.first);
    ends.push_back(p.second);
  }

  sort(starts.begin(), starts.end());
  sort(ends.begin(), ends.end());

  int sum = 1;
  int level = 1;
  int curr = starts[startInd++];
  int last = curr;
  while(curr != ends.back())
  {
    if(starts[startInd] <= ends[endInd])
    {
      if(level == 0)
        sum++;
      else
        sum += starts[startInd] - curr;

      level++;
      curr = starts[startInd++];
    }
    else
    {
      sum += ends[endInd] - curr;

      level--;
      curr = ends[endInd++];
    }
  }

  return sum;
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
  treasure.emplace_back(y, L-x);
  treasure.emplace_back(y+w, L-x);
  treasure.emplace_back(y+w, L-x-w);
  treasure.emplace_back(y, L-x-w);
  
  //Find range of points that cause intersections for point 0
  pair<int, bool> b_begin = findLeftEdge(pts, treasure, pts[0], 1);
  int b_end = findRightEdge(pts, treasure, pts[0], b_begin.first);
  
  //Walk around the outside and find that range (inclusive on both sides) for each point
  //Use previous range to find new range in O(1) time amortized
  vector<vector<pair<int, int>>> ranges(pts.size());
  for(int i=0; i<pts.size(); i++)
  {      
      if(i == b_begin.first % pts.size()) b_begin.first++;

      //Wasted test on first time, but should be harmless      
      b_begin = findLeftEdge(pts, treasure, pts[i], b_begin.first);
      b_end = findRightEdge(pts, treasure, pts[i], b_end);

      //Currently begin, end is inclusive/exclusive
      //it's a little easier to work with inclusive/inclusive
      //but we have to make sure there is a range first
      if(b_begin.second)
      {
        b_end = (b_end == 0 ? pts.size() - 1 : b_end - 1);

        //Check if range looped around point 0, and if so
        //split it into two ranges
        //cerr << "Range " << i << ": " << b_begin.first << " -> " << b_end << endl;
        if(b_end < b_begin.first)
        {
          //cerr << "Split " << b_begin.first << " -> " << pts.size()-1;
          //cerr << " : " << 0 << " -> " << b_end << endl;
          ranges[i].emplace_back(b_begin.first, pts.size()-1);
          ranges[i].emplace_back(0, b_end);
        }
        else
        {
          ranges[i].emplace_back(b_begin.first, b_end);
        }
      }
      else
      {
        //cerr << "Range " << i << ": None" << endl;
      }
  }

  //Total triangles found
  ll triangles = 0;

  //Now check each point to see how many triangles it's part of with
  //0 intersections on the box using the ranges of itself and the points
  //it can safely connect to
  //O(n^2)
  for(int i=0; i<pts.size(); i++)
  {
    vector<pair<int, int>>& range1 = ranges[i];

    int checkStart = 0, checkEnd = 0;
    bool noRange = true;
    if(range1.size())
    {
      checkStart = (range1.back().second + 1) % pts.size();
      checkEnd = range1.front().first;
      noRange = false;
    }

    //Check point i with each point outside it's intersection range
    //and find points outside both intersection ranges to make a triangle
    for(int j=checkStart; j != checkEnd || noRange; j = (j + 1) % pts.size())
    {
      noRange = false;
      if(j == i) continue;

      vector<pair<int, int>>& range2 = ranges[j]; 

      //Find how many points are invalidated because they are in
      //either range by comparing each part of the first
      //range to each part of the second
      int invalidPoints = 0;

      invalidPoints += union_size(range1, range2);
      
      //cerr << "Pts " << i << ", " << j << endl;
      //cerr << invalidPoints << " unsafe points" << endl;
      
      //Minus 2 so we don't count the points we're currently
      //looking at
      ll safeTriangles = tot_pts - invalidPoints - 2;
      //cerr << safeTriangles << " safe triangles" << endl;

      triangles += safeTriangles;
    }
  }

  triangles /= 6;
  //Remove 'triangles' that have three points on the same wall
  triangles -= 2*chooseNK(W+1, 3);
  triangles -= 2*chooseNK(L+1, 3);

  //Every triangle is counted once for each
  //side of each line; so 6 times
  cout << triangles << endl;
  //cerr << "---------------" << endl;
}

int main()
{
  int cases;
  cin >> cases;
  for(int i=0; i<cases; i++)
    doCase();
    
  return 0;
}