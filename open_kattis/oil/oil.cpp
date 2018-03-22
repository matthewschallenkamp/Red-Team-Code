#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct oil {
    ll x0, x1, y;
    oil() {}
    oil(int a, int b, int c) : x0(a), x1(b), y(c) {}
};

struct point {
    ll x, y, len;
    bool left;
    point() {}
    point(ll a, ll b, ll c, bool d) : x(a), y(b), len(c), left(d) {}
};

vector<point> points, sorted;
vector<oil> arr;
ll X, Y;

bool cmp(const point& a, const point& b) {
    ll aNum = a.y - Y;
    ll aDem = a.x - X;
    ll bNum = b.y - Y;
    ll bDem = b.x - X;
    if(a.y > Y) {
        aNum = -aNum;
        aDem = -aDem;
    }
    if(b.y > Y) {
        bNum = -bNum;
        bDem = -bDem;
    }
    if(aNum < 0 && aDem < 0) aNum = -aNum, aDem = -aDem;
    if(bNum < 0 && bDem < 0) bNum = -bNum, bDem = -bDem;
    if(aNum*bDem == bNum*aDem) {
        if(a.y < Y) {
            if(!a.left && !b.left) {
                return a.len > b.len;
            }
            if(!a.left) return true;
            if(!b.left) return false;
            return a.len > b.len;
        }
        if(a.left && b.left) {
            return a.len > b.len;
        }
        if(a.left) return true;
        if(b.left) return false;
        return a.len > b.len;
        
    }
    return aNum*bDem > bNum*aDem;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    set<ll> yVals;
    ll x0, x1, y;
    arr.resize(n);
    for(int i = 0; i < n; ++i) {
        cin >> x0 >> x1 >> y;
        if(x1 < x0) swap(x1,x0);
        arr[i] = oil(x0,x1,y);
        ll length = x1 - x0;
        if(length == 0) continue;
        points.push_back(point(x0,y,length, true));
        points.push_back(point(x1,y,length, false));
        yVals.insert(y);
    }
    if(yVals.size() == 1) {
        ll largest = 0;
        for(auto &p : points) {
            largest = max(largest, p.len);
        }
        cout << largest << '\n';
        return 0;
    }
    sorted.resize(2*n+1);
    ll answer = 0;
    for(int i = 0; i < points.size(); ++i) {
        X = points[i].x;
        Y = points[i].y;
        int pos = 0;
        for(auto &p : points) {
            if(p.y == Y) continue;
            sorted[pos++] = p;
        }
        sort(sorted.begin(), sorted.begin() + pos, cmp);
        ll sum = 0; 
        ll slopeNum = sorted[0].y - Y;
        ll slopeDem = sorted[0].x - X;
        if(slopeNum < 0 && slopeDem < 0) slopeNum = -slopeNum, slopeDem = -slopeDem;
        if(slopeDem == 0) {
            ll tempX = sorted[0].x;
            for(auto &seg : arr) {
                if(seg.x0 <= tempX && tempX <= seg.x1) {
                    sum += seg.x1 - seg.x0;
                }
            }
        } else {
            ll tempX = sorted[0].x;
            ll tempY = sorted[0].y;
            for(auto &seg : arr) {
                ll xNum = slopeDem*(seg.y-Y)+slopeNum*X;
                ll xDem = slopeNum;
                if(xNum < 0 && xDem < 0) xNum = -xNum, xDem = -xDem;
                if(seg.x0*xDem <= xNum && xNum <= seg.x1*xDem) {
                    sum += seg.x1 - seg.x0;
                }
            }
        }
        answer = max(answer, sum);
        bool currentSlope = true;
        for(int j = 0; j < pos; ++j) {
            point &p = sorted[j];
            ll num = p.y - Y;
            ll dem = p.x - X;
            if(p.y > Y) num = -num, dem = -dem;
            if(num < 0 && dem < 0) num = -num, dem = -dem;
            if(slopeNum*dem != num*slopeDem) currentSlope = false;
            if(p.y < Y) {
                if(p.left) sum -= p.len;
                else if(!currentSlope) sum += p.len;
            } else {
                if(p.left) {
                    if(!currentSlope) sum += p.len;
                } else sum -= p.len;
            }
            answer = max(answer, sum);
        }
    }
    cout << answer << '\n';
    return 0;
}




























