#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
using namespace std;

//jobs X workers cost matrix
//cost[i][j] is cost of job i done by worker j
//#jobs must be <= #workers
//Default finds max cost; to find min cost set all costs[i][j] to -costs[i][j]
//Adapted from the judges solution to cordonbleu from SWERC 2017
int64_t HungarianMatch(const vector<vector<int64_t>>& costs)
{
    uint64_t nx = costs.size();
    uint64_t ny = costs[0].size();

    vector<int64_t> xy(nx, -1), yx(ny, -1), lx(nx), ly(ny, 0), augmenting(ny);
    vector<bool> s(nx);
    vector<pair<int64_t, int64_t>> slack(ny, make_pair(0, 0));

    for (uint64_t x = 0; x < nx; x++)
    {
        int64_t& lxx = lx[x];
        lxx = costs[x][0];
        for(const int64_t& i : costs[x])
            lxx = max(lxx, i);   
    }

    for (uint64_t root = 0; root < nx; root++) {
        fill(augmenting.begin(), augmenting.end(), -1);
        fill(s.begin(), s.end(), false);

        s[root] = true;
        auto lxroot = lx[root]; auto criter = costs[root].begin();
        auto lyiter = ly.begin(); auto sliter = slack.begin();
        for (uint64_t y = 0; y < ny; y++, criter++, lyiter++, sliter++)
        *sliter = make_pair(lxroot + *lyiter - *criter, root);
        
        int64_t y = -1;
        for (;;) {
        int64_t delta = numeric_limits<int64_t>::max(), x = -1;
        auto aiter = augmenting.begin(); auto sliter = slack.begin();
        for (uint64_t yy = 0; yy < ny; yy++, aiter++, sliter++)
            if (*aiter == -1 && sliter -> first < delta) {
                delta = sliter->first;
                x = sliter->second;
                y = yy;
            }

        if (delta > 0) {
            auto siter = s.begin(); auto lxiter = lx.begin();
            for (uint64_t x = 0; x < nx; x++, siter++, lxiter++)
                if (*siter)
                    *lxiter -= delta;

            aiter = augmenting.begin(); lyiter = ly.begin(); sliter = slack.begin();
            for (uint64_t y = 0; y < ny; y++, lyiter++, aiter++, sliter++)
                if (*aiter > -1)
                    *lyiter += delta;
                else
                    sliter->first -= delta;
        }

        augmenting[y] = x;
        x = yx[y];
        if (x == -1)
            break;
        s[x] = true;
        
        aiter = augmenting.begin(); sliter = slack.begin();
        auto lxx = lx[x]; lyiter = ly.begin();
        auto cxiter = costs[x].begin();
        for (uint64_t y = 0; y < ny; y++, aiter++, sliter++, lyiter++, cxiter++)
            if (*aiter == -1) {
                pair<int64_t, int64_t> alt = make_pair(lxx + *lyiter - *cxiter, x);
                if (sliter->first > alt.first)
                    *sliter = alt;
                }
            }

        while (y > -1) {
            int x = augmenting[y];
            int prec = xy[x];
            yx[y] = x;
            xy[x] = y;
            y = prec;
        }
    }
    return -(accumulate(lx.cbegin(), lx.cend(), 0) + accumulate(ly.cbegin(), ly.cend(), 0));
}

struct point
{
    int32_t x, y;
};

bool operator !=(const point& l, const point& r)
{
    return l.x != r.x || l.y != r.y;
}

int32_t dist(const point& l, const point& r)
{
    return abs(l.x - r.x) + abs(l.y - r.y);
}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<point> wine(n);
    for(point& p : wine)
        cin >> p.x >> p.y;

    vector<point> couriers(m);
    for(point& p : couriers)
        cin >> p.x >> p.y;

    point restaurant;
    cin >> restaurant.x >> restaurant.y;

    //Add phantom couriers at restaurant
    uint64_t totalCouriers = couriers.size() + wine.size() - 1;
    couriers.reserve(totalCouriers);
    while(couriers.size() < totalCouriers)
        couriers.push_back(restaurant);

    //Build cost array
    vector<vector<int64_t>> costs(wine.size(), vector<int64_t>(couriers.size()));
    for(uint64_t i=0; i<wine.size(); i++)
        for(uint64_t j=0; j<couriers.size(); j++)
            costs[i][j] = -(dist(couriers[j], wine[i]) + dist(wine[i], restaurant));

    //If we have multiple wine bottles far from the restaurant
    //use the Hungarian method to determine which
    //couriers should pick up which bottles
    cout << HungarianMatch(costs) << endl;
}