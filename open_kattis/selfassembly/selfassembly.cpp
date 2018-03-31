#include <bits/stdc++.h>
using namespace std;
#define forwards 1
#define backwards 2

const int Max = 40002*4;
int visited[Max];
set<int> type[26][2];
string molecule[Max];
int portUsed[Max][4];

bool dfs(int node) {
    visited[node] = 1;
    string s = molecule[node];
    for(int j = 0; j < 8; j += 2) {
        if(s[j] == '0') continue;
        if(portUsed[node][j/2]) continue;
        for(int to : type[s[j]-'A'][(s[j+1]=='+')]) {
            string toS = molecule[to];
            int portTo = -1, portNode;
            for(int i = 0; i < 8; i += 2) {
                if(toS[i] == '0') continue;
                if(toS[i] == s[j] && toS[i+1] != s[j+1] && portUsed[to][i/2] != forwards) {
                    portNode = j/2;
                    portTo = i/2;
                    break;
                }
            }
            if(portTo != -1) {
                portUsed[node][portNode] = forwards;
                portUsed[to][portTo] = backwards;
                if(visited[to] == 1) {
                    return true;
                }
                if(visited[to] == 0) {
                    if(dfs(to)) {
                        return true;
                    }
                }
                portUsed[node][portNode] = 0;
                portUsed[to][portTo] = 0;
            }
        }
    }
    visited[node] = 2;
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    string s;
    set<string> seen;
    for(int i = 0; i < 4*n; ++i) {
        visited[i] = 0;
        if(i < n) {
            cin >> molecule[i];
            if(seen.count(molecule[i])) {
                molecule[i] = "00000000";
            }
            seen.insert(molecule[i]);
        } else molecule[i] = molecule[i-n];
        s = molecule[i];
        for(int j = 0; j < 8; j += 2) {
            for(int k = j+2; k < 8; ++k) {
                if(s[j] == '0' || s[k] == '0') continue;
                if(s[j] == s[k] && s[j+1] != s[k+1]) {
                    cout << "unbounded\n";
                    return 0; 
                }
            }
            portUsed[i][j/2] = 0;
            if(s[j] != '0') {
                type[s[j]-'A'][(s[j+1]=='-')].insert(i);
            }
        }
    }
    for(int i = 0; i < 4*n; ++i) {
        if(!visited[i]) {
            if(dfs(i)) {
                cout << "unbounded\n";
                return 0;
            }
        }
    }
    cout << "bounded\n";
    return 0;
}














