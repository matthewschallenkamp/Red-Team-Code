#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

int numKayak;
pair<int, int> beginner, normal, expert;//skill, count
vector<int> kayak;

bool possible(int minValue) {
  pair<int, int> b = beginner, n = normal, e = expert;
  int i;
  for(i = numKayak-1; i >= 0; --i) {//backwards, decreasing
    ll c = kayak[i];
    if(c*2*b.first >= minValue && b.second >= 2) {//beginner-beginner
      b.second -= 2;
    } else if(c*(b.first+n.first) >= minValue && b.second > 0 && n.second > 0) {//beginner-normal
      b.second--;
      n.second--;
    } else {
        // i--;
        break;
    }
  }
  int endI = i;

  for(i = 0; i < numKayak; ++i) {//forwards, increasing
    ll c = kayak[i];
    if( (c*2*(n.first) >= minValue) && (c*(b.first + e.first) >= minValue) ) break;
    if(c*2*n.first >= minValue && n.second >= 2) {//medium-medium
      n.second -= 2;
    } else if(c*(b.first + e.first) >= minValue && b.second > 0 && e.second > 0) {//beginner-expert
      b.second--;
      e.second--;
    } else if(c*(e.first+n.first) >= minValue && e.second > 0 && n.second > 0) {//expert-normal
      e.second--;
      n.second--;
    } else if(c*2*e.first >= minValue && e.second >= 2) {//expert-expert
      e.second -= 2;
    } else {
        // i++;
        break;
    }
  }
  int startI = i;
  bool mmIsLess = (2*n.first < b.first+e.first);
  //range from startI to endI needs to use (normal-normal) or (beginner-expert) pairs
  for(i = startI; i <= endI; ++i) {
    ll c = kayak[i];
    if(mmIsLess) {//medium-medium is less than begin-end
      if(c*2*n.first >= minValue && n.second >= 2) {//medium-medium
        n.second -= 2;
      } else if(c*(b.first + e.first) >= minValue && b.second > 0 && e.second > 0) {//beginner-expert
        b.second--;
        e.second--;
      } else if(c*(e.first+n.first) >= minValue && e.second > 0 && n.second > 0) {//expert-normal
        e.second--;
        n.second--;
      } else if(c*2*e.first >= minValue && e.second >= 2) {//expert-expert
        e.second -= 2;
      } else {
          return false;
      }
    } else {
      if(c*(b.first + e.first) >= minValue && b.second > 0 && e.second > 0) {//beginner-expert
        b.second--;
        e.second--;
      } else if(c*2*n.first >= minValue && n.second >= 2) {//medium-medium
        n.second -= 2;
      } else if(c*(e.first+n.first) >= minValue && e.second > 0 && n.second > 0) {//expert-normal
        e.second--;
        n.second--;
      } else if(c*2*e.first >= minValue && e.second >= 2) {//expert-expert
        e.second -= 2;
      } else {
          return false;
      }
    }
  }
  return true;
}

int main()
{
  cin >> beginner.second >> normal.second >> expert.second >> beginner.first >> normal.first >> expert.first;
  numKayak = (beginner.second+normal.second+expert.second)/2;
  kayak.resize(numKayak);
  for(int &k : kayak) cin >> k;
  sort(kayak.begin(), kayak.end());
  int start = 0, end = 1e9, mid;
  while(end-start>1) {
    mid = (start+end)>>1;
    if(possible(mid)) {
      start = mid;
    } else {
      end = mid;
    }
  }
  cout << start << '\n';
}