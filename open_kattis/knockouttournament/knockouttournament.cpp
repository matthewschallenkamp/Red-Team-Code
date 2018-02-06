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
typedef long double ld;

vector<vector<int>> adj;
vector<vector<pair<ld,int>>>node;//vector of probability, rating pairs

void calcSubtree(int curr) {
    if(adj[curr].size()==0) return;
    calcSubtree(2*curr+1);
    calcSubtree(2*curr+2);
    for(auto &p1 : node[2*curr+1]) {
        pair<ld, int> temp = make_pair(0,p1.second);
        for(auto &p2 : node[2*curr+2]) {
            temp.first += p1.first*p2.first*(p1.second/(ld)(p1.second+p2.second));
        }
        node[curr].push_back(temp);
    }
    for(auto &p1 : node[2*curr+2]) {
        pair<ld, int> temp = make_pair(0,p1.second);
        for(auto &p2 : node[2*curr+1]) {
            temp.first += p1.first*p2.first*(p1.second/(ld)(p1.second+p2.second));
        }
        node[curr].push_back(temp);
    }
}

int main()
{
    int n;
    cin >> n;
    int dale;
    cin >> dale;
    vector<int> arr(n-1);
    for(int &x : arr) cin >> x;
    //greedily sort array, face harder opponents last
    sort(arr.rbegin(), arr.rend());
    int powOfTwo = ceil(log2(n));
    int size = 1<<powOfTwo;
    adj.resize(2*size);
    vector<int> height(2*size,-1);
    height[0] = 0;
    int maxHeight = 0;
    //build tree, 0-based indexing
    for(int i = 0; i < n-1; i++) {
        adj[i].push_back(2*i+1);
        adj[i].push_back(2*i+2);
        height[2*i+1] = height[i] + 1;
        height[2*i+2] = height[i] + 1;
        maxHeight = max(maxHeight, height[2*i+2]);
    }
    node.resize(2*size);
    int pos = 0;
    int daleIndex;
    for(int i = 0; i < 2*size; ++i) {
        if(height[i] == maxHeight && pos < n-1) {
            node[i].push_back(make_pair(1,arr[pos++]));
        } else if(height[i] == maxHeight && pos == n-1) {
            node[i].push_back(make_pair(1,dale));
            daleIndex = i;
        }
    }
    for(int i = 0; i < 2*size; ++i) {
        if(height[i] == maxHeight-1 && pos < n-1 && height[2*i+1] != maxHeight) {
            node[i].push_back(make_pair(1,arr[pos++]));
        } else if(height[i] == maxHeight-1 && pos == n-1 && height[2*i+1] != maxHeight) {
            node[i].push_back(make_pair(1,dale));
            daleIndex = i;
        }
    }
    //calculate max probability
    ld winProb = 1;
    calcSubtree(0);
    while(daleIndex > 0) {
        int oppIndex = daleIndex-1;
        if(height[oppIndex] == 0) oppIndex = oppIndex*2+2;
        vector<pair<ld, int> > &opp = node[oppIndex];
        ld newProb = 0;
        for(auto &p : opp) {
            newProb += p.first*(dale/(ld)(dale+p.second));
        }
        winProb *= newProb;
        daleIndex = (daleIndex+1)/2-1;
    }
    cout << setprecision(10) << fixed << winProb << '\n';
}

















