#include <iostream>
#include <vector>
#include <cmath>
#define INF (int)(pow(2,31)-1)
#define SQR(a) ((a)*(a))

using namespace std;

struct point {
    int x, y;
};

int crossProduct(point A, point B, point C) {
    return ((B.x - A.x)*(C.y - A.y) - (B.y - A.y)*(C.x - A.x));
}

int pointSquaredDist(point A, point B) {
    return SQR(A.x - B.x) + SQR(A.y - B.y);
}

int areaOfTriangle(point A, point B, point C) {
    //This returns twice the area of triangle ABC, I'll divide by 2 at the end.
    //Twice the area of a triangle formed by integer coordinates is always an
    //integer.
    return abs((A.x-C.x)*(B.y-A.y) - (A.x-B.x)*(C.y-A.y));
}

//returns the convex hull of n points, O(n^2)
vector<point> convexHull(vector<point> &X, bool onEdge = false) {
    vector<point> hull;
    int N = X.size();
    vector<bool> used(N, false);
    int p = 0;
    for(int i = 1; i < N; ++i) if(X[i].x < X[p].x) p=i;
    int start = p;
    do {
        int n = -1;
        int dist = onEdge ? INF : 0;
        for(int i = 0; i < N; ++i) {
            if(i == p || used[i]) continue;
            if(n == -1) n = i;
            int cross = crossProduct(X[p], X[i], X[n]);
            int d = pointSquaredDist(X[i], X[p]);
            if(cross < 0 || (cross == 0 && ((onEdge && d < dist) || (!onEdge && d > dist)))) {
                n = i;
                dist = d;
            }
        }
        p = n;
        used[p] = true;
        hull.push_back(X[p]);
    } while(start != p);
    return hull;
}

void doTest() {
    int n;
    cin >> n;
    vector<point> locations(n);
    for(point &p : locations) cin >> p.x >> p.y;
    vector<point> hull = convexHull(locations);
    //if there are less than 3 points in your convex hull, zero area is best.
    //this can occur if the >= 3 points given are colinear (on the same line)
    if(hull.size() < 3) {
        cout << "0\n";
        return;
    }
    //special case 3 points in convex hull, this triangle gives max area
    if(hull.size() == 3) {
        cout << (double)areaOfTriangle(hull[0], hull[1], hull[2])/2 << '\n';
        return;
    }
    n = hull.size();
    int maxArea = 0;
    /*Iterate over all possible segments which lie inside the convex hull.
    In other words, iterate over all segments formed by pairwise points in the
    convex hull excluding points next to each other which form segments of the
    convex hull.*/
    for(int i = 0; i < n; ++i) {
        for(int j = i+2; (j+1)%n != i; ++j) {
            /*for each of these segments, do 2 binary searches: one on each side
            of the segment to find the maximum area of a triangle formed on that
            side.*/
            int start = i+1, end = j-1, mid=i+1;
            int area1 = areaOfTriangle(hull[i], hull[j%n], hull[start%n]);
            while(end-start > 1) {
                mid = (start+end)>>1;
                int areaMid = areaOfTriangle(hull[i], hull[j%n], hull[mid%n]);
                int areaMidPlusOne = areaOfTriangle(hull[i], hull[j%n], hull[(mid+1)%n]);
                if(areaMid == areaMidPlusOne) break;
                if(areaMid < areaMidPlusOne) {
                    start = mid;
                } else {
                    end = mid;
                }
            }
            area1 = max(areaOfTriangle(hull[i], hull[j%n], hull[start%n]),
                    max(areaOfTriangle(hull[i], hull[j%n], hull[mid%n]),
                        areaOfTriangle(hull[i], hull[j%n], hull[end%n])));
            start = j+1, end = i+n-1, mid=j+1;
            int area2 = areaOfTriangle(hull[i], hull[j%n], hull[start%n]);
            while(end-start > 1) {
                mid = (start+end)>>1;
                int areaMid = areaOfTriangle(hull[i], hull[j%n], hull[mid%n]);
                int areaMidPlusOne = areaOfTriangle(hull[i], hull[j%n], hull[(mid+1)%n]);
                if(areaMid == areaMidPlusOne) break;
                if(areaMid < areaMidPlusOne) {
                    start = mid;
                } else {
                    end = mid;
                }
            }
            area2 = max(areaOfTriangle(hull[i], hull[j%n], hull[start%n]),
                    max(areaOfTriangle(hull[i], hull[j%n], hull[mid%n]),
                        areaOfTriangle(hull[i], hull[j%n], hull[end%n])));
            /*The max area of the quadrilateral formed with the segment formed
            by points hull[i], and hull[j] is the sum of the areas of these two
            triangles. Take the max for each possible segment.*/
            maxArea = max(maxArea, area1 + area2);
        }
    }
    if(maxArea%2==0) {
        cout << maxArea/2 << '\n';
    } else {
        cout << maxArea/2 << ".5\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);//fast IO
    int t;
    cin >> t;
    while(t--) doTest();
    return 0;
}






























