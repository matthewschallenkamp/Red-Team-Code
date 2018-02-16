#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n, m;
int board[101][101], maxRow[101], maxCol[101];
bool empty[101][101], filled[101][101];

struct BipartiteMatcher {
	vector<vector<int>> G;
	vector<int> L, R, Viz;

	BipartiteMatcher(int n, int m) :
		G(n), L(n, -1), R(m, -1), Viz(n) {}

	void AddEdge(int a, int b) {
		G[a].push_back(b);
	}

	bool Match(int node) {
		if(Viz[node]) 
			return false;
		Viz[node] = true;
	
		for(auto vec : G[node]) {
			if(R[vec] == -1 || Match(R[vec])) {
				L[node] = vec;
				R[vec] = node;
				return true;
			}
		}

		return false;
	}
	int Solve() {
		bool ok = true;
		while(ok) {
			ok = false;
			fill(Viz.begin(), Viz.end(), 0);
			for(int i = 0; i < L.size(); ++i)
				if(L[i] == -1)
					ok |= Match(i);
		}
		
		int ret = 0;
		for(int i = 0; i < L.size(); ++i)
			ret += (L[i] != -1);
		return ret;
	}
};

int main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    cin >> n >> m;
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            cin >> board[i][j];
    for(int i = 0; i < n; ++i) maxRow[i] = 0;
    for(int j = 0; j < m; ++j) maxCol[j] = 0;
    vector<int> arr;
    int nonEmptyCount = 0;
    ll total = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            total += board[i][j];
            if(board[i][j]>1) arr.push_back(board[i][j]);
            if(board[i][j] > 0) nonEmptyCount++;
            filled[i][j] = false;
            empty[i][j] = (board[i][j]==0);
            maxRow[i] = max(maxRow[i], board[i][j]);
            maxCol[j] = max(maxCol[j], board[i][j]);
        }
    }
    sort(arr.rbegin(), arr.rend());
    arr.erase(unique(arr.begin(), arr.end()), arr.end());
    ll sum = 0;
    int spots;
    for(int value : arr) {
        vector<int> rows, cols;
        for(int i = 0; i < n; ++i) 
            if(maxRow[i] == value)
                rows.push_back(i);
            for(int j = 0; j < m; ++j)
                if(maxCol[j] == value)
                    cols.push_back(j);
        BipartiteMatcher g(n,m);
        for(int i : rows) {
            for(int j : cols) {
                if(!empty[i][j]) {
                    g.AddEdge(i,j);
                }
            }
        }
        int temp = g.Solve();
        nonEmptyCount -= (rows.size()+cols.size()-temp);
        sum += 1LL*value*(rows.size()+cols.size()-temp);
    }
    sum += nonEmptyCount;
    cout << total-sum << '\n';
    return 0;
}












































