#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int> > arr;

vector<int> bfs(int goal) {
    //cout << "\n\ngoal: " << char(goal+'a') << '\n';
    vector<int> Dist(n, -1);
    int calculated = 0;
    //queue<int> q;
    //q.push(goal);
    calculated |= (1<<goal);
    Dist[goal] = 0;
    bool repeat = true;
    while(repeat) {
        repeat = false;
    //for(int j = 0; j < n; ++j) {
    //while(!q.empty()) {
        //int node = q.front();
        //q.pop();
        for(int i = 0; i < n; ++i) {
            for(auto &group : arr[i]) {
                bool canCalculate = true;
                for(int bit = 0; bit < n; ++bit) {
                    if(group&(1<<bit)) {
                        if(Dist[bit] == -1) {
                            canCalculate = false;
                            break;
                        }
                    }
                }
                if(canCalculate) {
                //if((group&calculated) == calculated) {
                    int maxDist = 0;
                    for(int bit = 0; bit < n; ++bit) {
                        if(group&(1<<bit)) {
                            maxDist = max(maxDist, Dist[bit]);
                        }
                    }
                    if(Dist[i] == -1 || Dist[i] > 1+maxDist) {
                        calculated |= (1<<i);
                        //q.push(i);
                        //cout << "updating node: " << char(i+'a') << " with plan: " << group << '\t' << 1+maxDist << '\n';
                        Dist[i] = 1+maxDist;
                        repeat = true;
                    }
                }
            }
        }
    }
    return Dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cin >> n;
    arr.resize(n);
    for(int i = 0; i < n; ++i) {
        int len, mask;
        cin >> len;
        string group;
        for(int j = 0; j < len; ++j) {
            mask = 0;
            cin >> group;
            bool found = false;
            for(char c : group) {
                mask |= (1<<(c-'a'));
                if(c-'a' == i) {
                    found = true;
                    break;
                }
            }
            if(!found) arr[i].push_back(mask);
        }
    }
    /*
    cout << "plans:\n";
    for(int i = 0; i < n; ++i) {
        cout << "node: " << char(i+'a') << '\n';
        for(auto &plan : arr[i]) cout << plan << ' ';
        cout << '\n';
    }
    */
    vector<vector<int> > matrix(n, vector<int>(n));
    for(int goal = 0; goal < n; ++goal) {
        vector<int> Dist = bfs(goal);
        for(int start = 0; start < n; ++start) {
            matrix[start][goal] = Dist[start];
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cout << matrix[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}

