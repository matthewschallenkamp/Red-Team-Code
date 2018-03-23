#include <bits/stdc++.h>
using namespace std;
const double pi = 4*atanl(1.0);

struct sphere {
    double r,x,y,z;
    sphere(double a, double b, double c, double d) : r(a), x(b), y(c), z(d) {}
};

vector<sphere> arr;

double volume(double height) {
    double vol = 100*100*height;
    for(sphere &s : arr) {
        if(s.z+s.r <= height) {
            vol -= 4.0/3.0*pi*s.r*s.r*s.r;
        } else if(s.z-s.r < height && height <= s.z) {
            double h = height - (s.z-s.r);
            vol -= pi*h*h*s.r - pi/3.0*h*h*h;
        } else if(s.z < height && height < s.z+s.r) {
            double h = s.z + s.r - height;
            vol -= 4.0/3.0*pi*s.r*s.r*s.r;
            vol += pi*h*h*s.r - pi/3.0*h*h*h;
        }
    }
    return vol;
}

int main() {
    int n, s;
    cin >> n >> s;
    double r,x,y,z;
    double totalArea = 100*100*100;
    for(int i = 0; i < n; ++i) {
        cin >> r >> x >> y >> z;
        r/=1000.0;
        x/=1000.0;
        y/=1000.0;
        z/=1000.0;
        totalArea -= 4.0/3.0*pi*r*r*r;
        arr.push_back(sphere(r,x,y,z));
    }
    double slice = totalArea/s;
    double prevHeight = 0;
    for(int i = 1; i <= s; ++i) {
        double start = 0, end = 100, mid;
        while(abs(end-start)>0.0000001) {
            mid = (start+end)/2;
            if(volume(mid) < i*slice) start = mid;
            else end = mid;
        }
        cout << setprecision(7) << fixed << mid-prevHeight << '\n';
        prevHeight = mid;
    }
    return 0;
}
