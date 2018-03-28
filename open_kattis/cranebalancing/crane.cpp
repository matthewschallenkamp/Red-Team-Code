#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int> > poly;
double area;

double polyArea() {
    double result = 0;
    int n = poly.size();
    for(int i=0, j=1; i < n; i++, j=(j+1)%poly.size())
    {
        result += poly[i].first * poly[j].second;
        result -= poly[i].second * poly[j].first;
    }
    return double(result)/2.0;
}

pair<double, double> centroid() {
    int n = poly.size();
    double sumX = 0, sumY = 0;
    for(int i = 0; i < n; ++i) {
        double xi = poly[i].first;
        double yi = poly[i].second;
        double xi1 = poly[(i+1)%n].first;
        double yi1 = poly[(i+1)%n].second;
        sumX += (xi+xi1)*(xi*yi1-xi1*yi);
        sumY += (yi+yi1)*(xi*yi1-xi1*yi);
    }
    return make_pair(sumX/(6.0*area), sumY/(6.0*area));
}

int main() {
    int n;
    cin >> n;
    poly.resize(n);
    int X, Y;
    int leastX = 4000, mostX = -4000;
    for(auto &p : poly) {
        cin >> X >> Y;
        if(Y == 0) {
            leastX = min(leastX, X);
            mostX = max(mostX, X);
        }
        p = make_pair(X,Y);
    }
    area = polyArea();
    auto center = centroid();
    if(area < 0) {
        area = -area;
    }
    if(leastX <= poly[0].first && poly[0].first <= mostX) {
        if(leastX == poly[0].first && mostX < center.first) {
            double maxWeight = (mostX * area - center.first*area) / (poly[0].first - mostX);
            cout << maxWeight << " .. inf\n";
            return 0;
        }
        if(mostX == poly[0].first && center.first < leastX) {
            double minWeight = (leastX * area - center.first*area) / (poly[0].first - leastX);
            cout << minWeight << " .. inf\n";
            return 0;
        }
        if(center.first < leastX || mostX < center.first) {
            cout << "unstable\n";
            return 0;
        }
        cout << "0 .. inf\n";
        return 0;
    }
    if(mostX < center.first && mostX <= poly[0].first) {
        cout << "unstable\n";
        return 0;
    }
    if(leastX > center.first && leastX >= poly[0].first) {
        cout << "unstable\n";
        return 0;
    }
    double minWeight, maxWeight;
    maxWeight = (mostX * area - center.first*area) / (poly[0].first - mostX);
    minWeight = (leastX * area - center.first*area) / (poly[0].first - leastX);
    if(maxWeight < minWeight) swap(maxWeight, minWeight);
    if(maxWeight < 0) {
        cout << "unstable\n";
        return 0;
    }
    cout << max(0, (int)floor(minWeight)) << " .. " << (int)ceil(maxWeight) << '\n';
    return 0;
}



















