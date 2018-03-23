#include <iostream>
#include <vector>
#include <map>

using namespace std;


int bx = -1, bxs = -1, by = -1, bys = -1;
int best_sq = 1;


void rem(vector<int> &height, vector<int> &starts, int h, int j, int i, int &start) {
  while(height.size() && height.back() >= h) {
    int sq = min(height.back(), (j - starts.back()));
    // cerr << height.back() << " " << j - starts.back() << " " << sq << endl;
    if(sq > best_sq)
    {
      by = starts.back(); bys = j - starts.back();
      bx = i; bxs = height.back();
    }
    best_sq = max(best_sq , sq);
    height.pop_back();
    start = starts.back();
    starts.pop_back();
  }
}

int main()
{
  int INT_MAX=70000;
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  //each item will become the largest spot after it that it has a line to
  int x, y;
  scanf(" %i %i", &x, &y);
  while(x != 0 && y != 0)
  {
    bx = -1, bxs = -1, by = -1, bys = -1;
    best_sq = 1;

    vector<vector<int> > table(x);
    for(int i = 0; i < x; i++)
    {
      table[i].resize(y, 1);
      for(int j = 0; j < y; j++)
      {
        scanf("%i ", &(table[i][j]));
      }
    }
    vector<vector<int> > heights(x);
    heights[x-1].resize(y, 1);
    if(x > 2)
      heights[x-2].resize(y, 2);
    for(int i = x - 3; i >= 0; i--)
    {
      heights[i].resize(y, 2);
      for(int j = 0; j < y; j++)
      {
        if(table[i+2][j] - table[i+1][j] == table[i+1][j] - table[i][j])
        {
          heights[i][j] = heights[i+1][j] + 1;
        }
      }
    }
    //same algo as tables, but keep max sqr instead
    for(int i = 1; i < x; i++)
    {
      //per histogram
      //keep stack of items with type, start, height
      vector<int> starts, height;
      pair<int, int> ctype = make_pair(INT_MAX, INT_MAX);
      for(int j = 1; j < y; j++)
      {
        //new item for stack
        int bt = table[i-1][j] - table[i][j];
        int sbt = table[i-1][j-1] - table[i][j-1];
        int at = table[i][j-1] - table[i][j];
        int sat = table[i-1][j-1] - table[i-1][j];

        pair<int, int> type = make_pair(at, bt);
        int start = j-1, fs = j-1;
        if((abs(at) != abs(bt) && at != 0 && bt != 0) ||  at != sat || bt != sbt)
        {
          type = make_pair(INT_MAX, INT_MAX);
          start = j;
        }
        int h = min(heights[i-1][j - 1], heights[i-1][j]);
        // cerr << i << " " << j << " " << best_sq << " " << type.first << " " << type.second << " " << h << endl;
        if(type == ctype && type != make_pair(INT_MAX, INT_MAX))
        {
          rem(height, starts, h, j, i-1, start);
        }
        else {
          rem(height, starts, -1, j, i-1, fs);
        }
        ctype = type;
        starts.push_back(start);
        height.push_back(h);
      }
      int start = 0;
      // rem(height, starts, heights[i-1][y-1], y, i-1, start);
      rem(height, starts, -1, y, i-1, start);
    }

    // cerr << bx << " " << by << " " << bxs << " " << bys << endl;
    printf("%i\n", best_sq * best_sq);

    scanf(" %i %i", &x, &y);
  }
  return 0;
}