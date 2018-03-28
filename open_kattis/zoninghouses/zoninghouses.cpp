#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

vector<pair<long long, long long> > houses;

struct hbounds {
  long long u, d, l, r;
  hbounds() {};
  hbounds combine(hbounds ot) {
    //take best of each side
    // by bound then clockwise better -> corners might remove 2
    hbounds nh;
    if(houses[ot.l].first == houses[this->l].first) {
      //min other
      if(houses[ot.l].second < houses[this->l].second)
        nh.l = ot.l;
      else
        nh.l = this->l;
    } 
    else {
      if(houses[ot.l].first < houses[this->l].first)
        nh.l = ot.l;
      else
        nh.l = this->l;
    }
    
    if(houses[ot.r].first == houses[this->r].first) {
      //min other
      if(houses[ot.r].second > houses[this->r].second)
        nh.r = ot.r;
      else
        nh.r = this->r;
    } 
    else {
      if(houses[ot.r].first > houses[this->r].first)
        nh.r = ot.r;
      else
        nh.r = this->r;
    }
    
    if(houses[ot.u].second == houses[this->u].second) {
      //min other
      if(houses[ot.u].first < houses[this->u].first)
        nh.u = ot.u;
      else
        nh.u = this->u;
    } 
    else {
      if(houses[ot.u].second > houses[this->u].second)
        nh.u = ot.u;
      else
        nh.u = this->u;
    }
    
    if(houses[ot.d].second == houses[this->d].second) {
      //min other
      if(houses[ot.d].first > houses[this->d].first)
        nh.d = ot.d;
      else
        nh.d = this->d;
    } 
    else {
      if(houses[ot.d].second < houses[this->d].second)
        nh.d = ot.d;
      else
        nh.d = this->d;
    }
    nh.score();
    return nh;
  }
  long long score() {
    // cerr << "sf " << l << " " << r << " " << d << " " << u << endl;
    // cerr << max((houses[u].second - houses[d].second), (houses[r].first - houses[l].first)) << endl;
    return max((houses[u].second - houses[d].second), (houses[r].first - houses[l].first));
  }
};

typedef hbounds dt;

//Segment tree code
struct dp
{
    dt val;
    dp() {};
    dp(dt i): val(i){};
    void update(const dt& u)
    {
        val = u;
    }
};

dp combine(dp l, dp r)
{
    return l.val.combine(r.val);
}

struct segment
{
    vector<dp> data;
    int s;
    
    segment(int sz, const vector<dp>& base = vector<dp>()): s(sz)
    {
        data.resize(2*s);
        for(int i=0; i < base.size(); i++)
        {
            data[s+i] = base[i];
        }
        for(int i=s-1; i>0; i--)
        {
            data[i] = combine(data[2*i], data[2*i+1]);
        }
    }

    void update(int spot, const dt& update)
    {
        data[spot + s].update(update);
        for(int i=(spot + s)/2; i>0; i/=2)
            data[i] = combine(data[2*i], data[2*i+1]);
    }
    dp query(int si, int ei)
    {
        dp res; bool first = true;
        for(si += s, ei += s; si < ei; si /= 2, ei /= 2)
        {
            if(si % 2 == 1) {
              if (first) {
                res = data[si++];
                first = false;
              }
              else
                res = combine(res, data[si++]);
            }
            if(ei % 2 == 1) {
              if(first) {
                res = data[--ei];
                first = false;
              }
              else
                res = combine(res, data[--ei]);
            }
        }
        // if(first) cerr << "None found" << endl;
        return res;
    }
};

int main() {
  int n, q;
  cin >> n >> q;
  houses.resize(n);
  for(auto &it : houses) {
    cin >> it.first >> it.second;
  }
  vector<dp> idxs(n);
  for(int i = 0; i < n; i++)
  {
    idxs[i].val.u = i;
    idxs[i].val.r = i;
    idxs[i].val.d = i;
    idxs[i].val.l = i;
  }
  segment s(n, idxs);
  int l, r;
  for(int i = 0; i < q; i++) {
    cin >> l >> r;
    //get query
    //try removing each item lrud and doing remaining queries
    //and return best res;
    // cerr << l-1 << " " << r << endl;
    hbounds h = s.query(l-1, r).val;
    long long bres = h.score();
    
    for(auto item : {h.l, h.r, h.d, h.u}) 
    {
      // cerr << l - 1 << " " << item << " " << r << " " << bres << endl;
      //query without item
      if(item == l - 1) {
        bres = min(bres, s.query(l, r).val.score());
      }
      else if(item == r - 1) {
        bres = min(bres, s.query(l-1, r-1).val.score());
      }
      else {
        bres = min(bres, s.query(l-1, item).val.combine(s.query(item + 1, r).val).score());
      }
    }
    cout << bres << endl;
  }
}


