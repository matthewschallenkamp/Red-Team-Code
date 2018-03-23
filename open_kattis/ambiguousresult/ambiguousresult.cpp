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

ll memoMax[101][101], memoMin[101][101];

void doTest(string &s) {
  vector<ll> nums;
  vector<char> opps;
  int n = s.size();
  int prevPos = 0;
  for(int i = 0; i < n; ++i) {
    if(s[i] == '+' || s[i] == '*') {
      nums.push_back(stoi(s.substr(prevPos, i-prevPos)));
      opps.push_back(s[i]);
      prevPos = i+1;
    }
  }
  nums.push_back(stoi(s.substr(prevPos)));
  n = nums.size();
  for(int i = 0; i < n; ++i) {
    for(int j = i; j < n; ++j) {
      memoMax[i][j] = 0;
      memoMin[i][j] = (1LL<<63)-1;
    }
  }
  for(int i = 0; i < n; ++i) {
   memoMin[i][i] = memoMax[i][i] = nums[i];
  }
  for(int Length = 1; Length <= n; ++Length) {
    for(int i = 0; i+Length < n; ++i) {
      int j = i+Length;
      for(int split = i; split <= j-1; ++split) {
        if(opps[split] == '+') {
          memoMax[i][j] = max(memoMax[i][j], memoMax[i][split] + memoMax[split+1][j]);
          memoMin[i][j] = min(memoMin[i][j], memoMin[i][split] + memoMin[split+1][j]);
        }
        else {
          memoMax[i][j] = max(memoMax[i][j], memoMax[i][split] * memoMax[split+1][j]);
          memoMin[i][j] = min(memoMin[i][j], memoMin[i][split] * memoMin[split+1][j]);
        }
      }
    }
  }
  cout << memoMin[0][n-1] << ' ' << memoMax[0][n-1] << endl;
  return;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string s;
  while(cin >> s && s != "END")
  {
    doTest(s);
  }
}
