#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>
using namespace std;
typedef long long ll;

vector<vector<ll> > maxs;
vector<string> board;
vector<pair<int, ll> > moves;
vector<ll> xs, ys, ts;


struct dhcomp {
  bool operator()(int l, int r) {
    switch(moves[l].first) {
      case 'u':
        return ts[l] + xs[l] != ts[r] + xs[r] ? ts[l] + xs[l] < ts[r] + xs[r] : moves[l].second - xs[l] < moves[r].second - xs[r];
      case 'r':
        return ts[l] - ys[l] != ts[r] - ys[r] ? ts[l] - ys[l] < ts[r] - ys[r] : moves[l].second + ys[l] < moves[r].second + ys[r];
      case 'd':
        return ts[l] - xs[l] != ts[r] - xs[r] ? ts[l] - xs[l] < ts[r] - xs[r] : moves[l].second + xs[l] < moves[r].second + xs[r];
      case 'l':
        return ts[l] + ys[l] != ts[r] + ys[r] ? ts[l] + ys[l] < ts[r] + ys[r] : moves[l].second - ys[l] < moves[r].second - ys[r];
    }
  }
};

int main()
{
  int h, w, n;
  cin >> h >> w >> n;
  board.resize(h);
  for(int i = 0; i < h; i++)
    cin >> board[i];
  string dir; ll length;
  for(int i = 0; i < n; i++)
  {
    cin >> dir >> length;
    moves.push_back(make_pair(dir[0], length));
  }
  //get starting point for each move
  ll x = h - 1, y = 0, t = 0;
  for(int i = 0; i < n; i++)
  {
    xs.push_back(x);
    ys.push_back(y);
    ts.push_back(t);
    if(moves[i].first == 'u') {
      x -= moves[i].second;
    }
    if(moves[i].first == 'r') {
      y += moves[i].second;
    }
    if(moves[i].first == 'd') {
      x += moves[i].second;
    }
    if(moves[i].first == 'l') {
      y -= moves[i].second;
    }
    t += moves[i].second;
  }
  unordered_map<char, vector<vector<vector<int> > > > mhere;
  for(auto chr : {'u', 'r', 'd', 'l'}) {
    mhere[chr].resize(h, vector<vector<int>>(w));
  }
  for(int i = 0; i < n; i++)
  {
    mhere[moves[i].first][xs[i]][ys[i]].push_back(i);
  }
  
  //per dir
  maxs.resize(h, vector<ll>(w, -1));
  ll wmin = t + 1, rmax = -1;
  
  set<int, dhcomp> opens;
  //per direction
  
  //down
  for(int y = 0; y < w; y++)
  {
    opens.clear();
    for(int x = 0; x < h; x++)
    {
      //insert into opens
      for(auto item : mhere['d'][x][y]) {
        //find spot of item
        auto it = opens.lower_bound(item);
        //remove items strictly worse going forward
        while(it != opens.end() && 
            ts[item] - xs[item] >= ts[*it] - xs[*it] &&
            moves[item].second + xs[item] >= moves[*it].second + xs[*it]) {
          it = opens.erase(it);
        }
        opens.insert(item);
      }
      //set new max
      if(opens.begin() != opens.end()) {
        maxs[x][y] = max(maxs[x][y], ts[*opens.rbegin()] + x - xs[*opens.rbegin()]);
      }
      //maybe remove begin
      while(opens.begin() != opens.end() &&
        x - xs[*opens.rbegin()] >= moves[*opens.rbegin()].second) {
        opens.erase(*opens.rbegin());
      }
    }
  }

  for(int y = 0; y < w; y++)
  {
    opens.clear();
    for(int x = h - 1; x >= 0; x--)
    {
      //insert into opens
      for(auto item : mhere['u'][x][y]) {
        //find spot of item
        auto it = opens.lower_bound(item);
        //remove items strictly worse going forward
        while(it != opens.end() && 
            ts[item] + xs[item] >= ts[*it] + xs[*it] &&
            moves[item].second - xs[item] >= moves[*it].second - xs[*it]) {
          it = opens.erase(it);
        }
        opens.insert(item);
      }
      //set new max
      if(opens.begin() != opens.end()) {
        maxs[x][y] = max(maxs[x][y], ts[*opens.rbegin()] - x + xs[*opens.rbegin()]);
      }
      //maybe remove begin
      while(opens.begin() != opens.end() &&
        xs[*opens.rbegin()] - x >= moves[*opens.rbegin()].second) {
        opens.erase(*opens.rbegin());
      }
    }
  }

  for(int x = h - 1; x >= 0; x--)
  {
   opens.clear();
    for(int y = w - 1; y >=0; y--)
    {    
      //insert into opens
      for(auto item : mhere['l'][x][y]) {
        //find spot of item
        auto it = opens.lower_bound(item);
        //remove items strictly worse going forward
        while(it != opens.end() && 
            ts[item] + ys[item] >= ts[*it] + ys[*it] &&
            moves[item].second - ys[item] >= moves[*it].second - ys[*it]) {
          it = opens.erase(it);
        }
        opens.insert(item);
      }
      //set new max
      if(opens.begin() != opens.end()) {
        maxs[x][y] = max(maxs[x][y], ts[*opens.rbegin()] - y + ys[*opens.rbegin()]);
      }
      //maybe remove begin
      while(opens.begin() != opens.end() &&
        ys[*opens.rbegin()] - y >= moves[*opens.rbegin()].second) {
        opens.erase(*opens.rbegin());
      }
    }
  }

  for(int x = h - 1; x >= 0; x--)
  {
   opens.clear();
    for(int y = 0; y < w; y++)
    {    
      //insert into opens
      for(auto item : mhere['r'][x][y]) {
        //find spot of item
        auto it = opens.lower_bound(item);
        //remove items strictly worse going forward
        while(it != opens.end() && 
            ts[item] - ys[item] >= ts[*it] - ys[*it] &&
            moves[item].second + ys[item] >= moves[*it].second + ys[*it]) {
          it = opens.erase(it);
        }
        opens.insert(item);
      }
      //set new max
      if(opens.begin() != opens.end()) {
        maxs[x][y] = max(maxs[x][y], ts[*opens.rbegin()] + y - ys[*opens.rbegin()]);
      }
      //maybe remove begin
      while(opens.begin() != opens.end() &&
        y - ys[*opens.rbegin()]>= moves[*opens.rbegin()].second) {
        opens.erase(*opens.rbegin());
      }
    }
  }  



  for(int y = 0; y < w; y++)
  {
    for(int x = h - 1; x >= 0; x--)
    {
      if(maxs[x][y] != -1) {
        if(board[x][y] == '.')
          wmin = min(wmin, maxs[x][y]);
        else
          rmax = max(rmax, maxs[x][y]);
      }
      else if(board[x][y] != '.') {
        wmin = -1;
        rmax = t + 1;
      }
    }
  }


  // for(int x = 0; x < h; x++)
  // {
  //   for(int y = 0; y < w; y++)
  //   {
  //     cerr << maxs[x][y] << " ";
  //   }
  //   cerr << endl;
  // }
  cerr << rmax + 1 << " " << wmin << endl;
  if(rmax + 1 <= wmin)
    cout << rmax + 1 << " " << wmin << endl;
  else
    cout << "-1 -1" << endl;
  return 0;
}
