#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod = 13;

ll modInverse(ll a, ll b){
    return 1<a ? b - modInverse(b%a,a)*b/a : 1;
}

void reduce(vector<vector<int> > &matrix) {
    int n = matrix.size(), m = matrix[0].size();
    for(int row = 0; row < n; ++row) {
        bool found = true;
        if(matrix[row][row] == 0) {
            found = false;
            for(int i = row+1; i < n; ++i) {
                //find first non-zero element in submatrix
                if(matrix[i][row] != 0) {
                    found = true;
                    for(int j = row; j < matrix[i].size(); ++j) {
                        matrix[row][j] += matrix[i][j];
                        matrix[row][j] %= mod;
                    }
                    break;
                }
            }
        }
        if(!found) continue;
        int inverse = modInverse(matrix[row][row], mod);
        for(int j = row; j < m; ++j) {
            matrix[row][j] *= inverse;
            matrix[row][j] %= mod;
        }
        for(int i = 0; i < n; ++i) {
            if(i != row) {
                int mult = matrix[i][row];
                for(int j = row; j < matrix[i].size(); ++j) {
                    matrix[i][j] -= matrix[row][j] * mult;
                    matrix[i][j] = (matrix[i][j]%mod+mod)%mod;
                }
            }
        }
    }
}

int adjMatrix[202][202], n;
int Distance[202];

void dijkstras(int start) {
    set<pair<int, int> > q;
    vector<bool> visited(n, false);
    for(int i = 0; i < n; ++i) Distance[i] = 1e9;
    Distance[start] = 0;
    q.insert({0,start});
    while(q.size() > 0) {
        auto it = q.begin();
        int curr = it->second;
        visited[curr] = true;
        q.erase(it);
        for(int to = 0; to < n; ++to) {
            if(!visited[to]) {
                if(Distance[to] > Distance[curr] + adjMatrix[curr][to]) {
                    Distance[to] = Distance[curr] + adjMatrix[curr][to];
                    q.insert({Distance[to], to});
                }
            }
        }
    }
}

int main() {
    int start, goal, t;
    cin >> n >> start >> goal >> t;
    vector<vector<int> > matrix;
    vector<pair<int, vector<int> > > trips(t);
    vector<vector<bool> > edges(n,vector<bool>(n,false));
    int numNodes;
    for(int row = 0; row < t; ++row) {
        cin >> trips[row].first >> numNodes;
        int previous = -1, curr;
        while(numNodes--) {
            cin >> curr;
            curr--;
            trips[row].second.push_back(curr);
            if(previous != -1) edges[min(previous, curr)][max(previous, curr)] = true;
            previous = curr;
        }
    }
    vector<vector<int> > table(n, vector<int>(n));
    int index = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            if(edges[i][j]) {
                table[i][j] = index++;
            }
        }
    }
    matrix.resize(t, vector<int>(index+1));//one extra column for the time
    for(int i = 0; i < t; ++i) {
        int size = trips[i].second.size();
        matrix[i].back() = trips[i].first;
        for(int j = 1; j < size; ++j) {
            int smaller = trips[i].second[j];
            int larger = trips[i].second[j-1];
            if(smaller > larger) swap(smaller, larger);
            matrix[i][table[smaller][larger]]++;
        }
    }
    reduce(matrix);
    index = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = i+1; j < n; ++j) {
            if(edges[i][j]) adjMatrix[j][i] = adjMatrix[i][j] = matrix[index++].back();
            else adjMatrix[j][i] = adjMatrix[i][j] = 1e9;
        }
    }
    dijkstras(start-1);
    cout << Distance[goal-1] << '\n';
    return 0;
}






