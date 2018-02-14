#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct corner {
    int i,j,index;
    bool UL;//true iff upper-left corner
};

bool cmp(corner x, corner y) {
    if(x.j == y.j) return x.i < y.i;
    return x.j < y.j;
}

bool intersect(corner a, corner b, corner c, corner d) {
    int x1 = a.i, y1 = a.j;
    int x2 = b.i, y2 = b.j;
    int X1 = c.i, Y1 = c.j;
    int X2 = d.i, Y2 = d.j;
    if(x2 < x1) swap(x1,x2);
    if(y2 < y1) swap(y1,y2);
    if(X2 < X1) swap(X1,X2);
    if(Y2 < Y1) swap(Y1,Y2);
    if(x1 < X1 && X2 < x2) {
        if(y1 < Y1 && Y2 < y2) {
            return false;
        }
    }
    if(X1 < x1 && x2 < X2) {
        if(Y1 < y1 && y2 < Y2) {
            return false;
        }
    }
    if(X1 <= x2 && x1 <= X2) {
        if(Y1 <= y2 && y1 <= Y2) {
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    vector<corner> corners(2*n);
    for(int i = 0; i < 2*n; ++i) {
        cin >> corners[i].i >> corners[i].j;
        corners[i].UL = (i<n);
        corners[i].index = i%n;
    }
    vector<corner> original(corners);
    sort(corners.begin(), corners.end(), cmp);
    vector<int> answer(n);
    map<int, corner> upperLefts;
    //finding any solution
    for(corner &c : corners) {
        if(c.UL) {
            if(upperLefts.count(c.i)) {
                cout << "syntax error\n";
                return 0;
            }
            upperLefts[c.i] = c;
        } else {
            if(upperLefts.size() == 0) {
                cout << "syntax error\n";
                return 0;
            }
            auto it = upperLefts.upper_bound(c.i);
            if(it == upperLefts.begin()) {
                cout << "syntax error\n";
                return 0;
            }
            it--;
            if(it->second.i > c.i) {
                cout << "syntax error\n";
                return 0;
            }
            answer[it->second.index] = c.index;
            upperLefts.erase(it);
        }
    }
    //checking if solution is valid
    for(corner &c : corners) {
        if(c.UL) {
            upperLefts[c.i] = c;
            auto it = upperLefts.find(c.i);
            if(it != upperLefts.begin()) {
                it--;
                if(intersect(c, original[answer[c.index]+n], it->second, original[answer[it->second.index]+n])) {
                    cout << "syntax error\n";
                    return 0;
                }
                it++;
            }
            if(it != upperLefts.end()) {
                it++;
                if(it != upperLefts.end()) {
                    if(intersect(c, original[answer[c.index]+n], it->second, original[answer[it->second.index]+n])) {
                        cout << "syntax error\n";
                        return 0;
                    }
                }
                it--;
            }
        } else {
            auto it = upperLefts.upper_bound(c.i);
            it--;
            upperLefts.erase(it);
        }
    }
    for(int x : answer) cout << x+1 << ' ';
    cout << '\n';
    return 0;
}



















