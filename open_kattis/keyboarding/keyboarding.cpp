#include <bits/stdc++.h>
using namespace std;

string s;
int dist[51][51][10002], inf = 1e9;
bool visited[51][51][10002];
vector<pair<int, int> > moves[51][51];
vector<string> keyboard;

struct node {
    int i, j, pos;
    node();
    node(int a, int b, int c) : i(a), j(b), pos(c) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    keyboard.resize(n);
    for(string &str : keyboard) cin >> str;
    cin >> s;
    s = s + '*';
    int pos;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            pos = i;
            while(pos < n && keyboard[i][j] == keyboard[pos][j]) pos++;
            if(pos < n) moves[i][j].push_back({pos,j});
            pos = i;
            while(pos >= 0 && keyboard[i][j] == keyboard[pos][j]) pos--;
            if(pos >= 0) moves[i][j].push_back({pos,j});
            pos = j;
            while(pos < m && keyboard[i][j] == keyboard[i][pos]) pos++;
            if(pos < m) moves[i][j].push_back({i,pos});
            pos = j;
            while(pos >= 0 && keyboard[i][j] == keyboard[i][pos]) pos--;
            if(pos >= 0) moves[i][j].push_back({i,pos});
        }
    }
    queue<node> q;
    q.push(node(0,0,0));
    dist[0][0][0] = 0;
    int i,j, currDist;
    while(!q.empty()) {
        i = q.front().i;
        j = q.front().j;
        pos = q.front().pos;
        q.pop();
        currDist = dist[i][j][pos];
        if(pos==s.size()) {
            cout << currDist << '\n';
            break;
        }
        if(keyboard[i][j] == s[pos] && !visited[i][j][pos+1]) {
            visited[i][j][pos+1] = true;
            q.push(node(i,j,pos+1));
            dist[i][j][pos+1] = currDist + 1;
        }
        for(auto &move : moves[i][j]) {
            if(!visited[move.first][move.second][pos]) {
                q.push(node(move.first, move.second, pos));
                visited[move.first][move.second][pos] = true;
                dist[move.first][move.second][pos] = currDist + 1;
            }
        }
    }
    return 0;
}
























