#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int inf = (int)1e8;

void doTest() {
    int n, widthRiver;
    cin >> n >> widthRiver;
    vector<pair<int, int> > boats;
    int l,r;
    for(int i = 0; i < n; ++i) {
        cin >> l >> r;
        boats.push_back({l,r});
    }
    //visited[i] = true iff mario can get to boat i in time t
    //right[i] = true iff boat i is going right at time t
    vector<bool> visited(n,false), right(n,true);
    vector<int> position(n);
    for(int i = 0; i < n; ++i) {
        position[i] = boats[i].first;
        if(position[i] == 0) visited[i] = true;
    }
    //simulate boats
    for(int t = 1; t <= widthRiver*widthRiver+1; ++t) {
        unordered_map<int, int> marioPos;//map next position to previous position
        vector<int> previousPos(position);
        //update boat positions
        for(int i = 0; i < n; ++i) {
            if(right[i]) {
                position[i]++;
                if(position[i] == boats[i].second) right[i] = false;
            } else {
                position[i]--;
                if(position[i] == boats[i].first) right[i] = true;
            }
            if(position[i] == widthRiver && visited[i]) {
                cout << t << '\n';
                return;
            }
            if(visited[i]) {
                marioPos[position[i]] = previousPos[i];
            }
        }
        //update which boats mario can get to in time t
        for(int i = 0; i < n; ++i) {
            if(!visited[i]) {
                if(marioPos.count(position[i])) {
                    visited[i] = true;
                } else if(marioPos.count(previousPos[i]) && marioPos[previousPos[i]] == position[i]) {
                    visited[i] = true;
                }
            }
        }
    }
    cout << "IMPOSSIBLE\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);//fast IO
    int t;
    cin >> t;
    while(t--) doTest();
    return 0;
}











































