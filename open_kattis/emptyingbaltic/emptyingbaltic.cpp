#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>

using namespace std;

//y, x
typedef pair<int64_t, int64_t> loc;

//Depth, neighbor count
struct node
{
  int64_t drain = 0;
  int64_t depth;
  vector<loc> neighbors;
};

void dijkstra(vector<vector<node>>& grid, int y, int x)
{
  priority_queue<pair<int64_t, loc>> q;
  q.push(make_pair(-grid[y][x].depth, loc(y, x)));
  grid[y][x].drain = -grid[y][x].depth;

  while(q.size())
  {
    auto curr = q.top();
    q.pop();
    node& here = grid[curr.second.first][curr.second.second];

    for(auto& p : here.neighbors)
    {
      node& there = grid[p.first][p.second];

      int64_t minDrain = min(-there.depth, here.drain);
      if(minDrain > there.drain)
      {
        there.drain = minDrain;
        q.push(make_pair(there.drain, p));
      }
    }
  }
}

int main()
{
  int h, w;
  cin >> h >> w;
  
  //Read map
  vector<vector<node>> grid(h, vector<node>(w));
  for(auto& v : grid)
  {
    for(auto& n : v)
      cin >> n.depth;
  }
  
  //Drain location
  int di, dj;
  cin >> di >> dj;
  di--; dj--;
  
  //For each cell
  for(int i=0; i<h; i++)
    for(int j=0; j<w; j++)
    {
      //Look at the 8 surrounding cells
      for(int i_=-1; i_<=1; i_++)
        for(int j_=-1; j_<=1; j_++)
          if(i_ !=0 || j_ != 0)
          {
            int ni = i+i_, nj = j+j_;

            //If cell is on grid, and under water, add to neighbor list
            if(ni >= 0 && ni < h && nj >= 0 && nj < w)
              if(grid[ni][nj].depth < 0)
                grid[i][j].neighbors.push_back(loc(ni, nj));
          }
    }

  dijkstra(grid, di, dj);

  uint64_t result = 0;
  for(auto& v : grid)
    for(auto& n : v)
      result += n.drain;

  cout << result << endl;
}