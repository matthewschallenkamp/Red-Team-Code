#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <functional>
#include <iomanip>

using namespace std;

typedef long double ld;

constexpr ld g = 9.81;

//Since intersection at x<=0 is not possible in the problem
//All three intersection algorithms should return a negative
//if no intersection

//Top part of slope
ld H1(ld H, ld L, ld l)
{
    ld lL = l/L;
    return H*(1-2*lL*lL);
}

ld intersectH1(ld H, ld L, ld v0_2, ld p)
{
    ld det = -p/(-g/(2*v0_2) + 2*H/(L*L));
    if(det <= 0) return -1;
    return sqrt(det);
}

ld derivativeH1(ld H, ld L, ld l)
{
    return -4*H*l/(L*L);
}

//Bottom part of slope
ld H2(ld H, ld L, ld l)
{
    ld L11 = l/L-1;
    return 2*H*L11*L11;
}

ld intersectH2(ld H, ld L, ld v0_2, ld p)
{
    ld det = (16*H*H)/(L*L) - 4*((2*H)/(L*L) + (g/(2*v0_2)))*(H-p);
    ld num = (4*H)/L + sqrt(det);
    ld denom = 2*((2*H)/(L*L) + (g/(2*v0_2)));

    return num/denom;
}

ld derivativeH2(ld H, ld L, ld l)
{
    return 4*H*l/(L*L) - 4*H/L;
}

//Flat surface
ld H3(ld H, ld L, ld l)
{
    return 0;
}

ld intersectH3(ld H, ld L, ld v0_2, ld p)
{
    ld det = ((-H - p)*2*v0_2)/-g;
    return sqrt(det);
}

ld derivativeH3(ld H, ld L, ld l)
{
    return 0;
}

ld dF(ld v0_2, ld l)
{
    return (-g*l)/v0_2;
}

ld angle(ld s1, ld s2)
{
    return (atan(s2) - atan(s1)) * 180/M_PI;
}

void doCase()
{
    ld j, p, H, L;
    cin >> j >> p >> H >> L;

    ld v0_2 = g*j*2;
    //cerr << "v0^2 = " << v0_2 << endl;

    ld l;
    function<ld(ld, ld, ld)> dH;
    function<ld(ld, ld, ld)> fl;

    ld intersect1 = intersectH1(H, L, v0_2, p);
    if(intersect1 >= 0 && intersect1 < L/2)
    {
        //Land on top of slope
        l = intersect1;
        dH = derivativeH1;
        fl = H1;
    }
    else
    {
        ld intersect2 = intersectH2(H, L, v0_2, p);
        if(intersect2 >= L/2 && intersect2 < L)
        {
            //Land on middle of slope
            l = intersect2;
            dH = derivativeH2;
            fl = H2;
        }
        else
        {
            //Land on flat part
            ld intersect3 = intersectH3(H, L, v0_2, p);
            l = intersect3;
            dH = derivativeH3;
            fl = H3;
        }
    }

    ld h = fl(H, L, l);
    ld vl = sqrt(g*(j+p+(H-h))*2);
    ld a = angle(dF(v0_2, l), dH(H, L, l));

    cout << setprecision(6) << fixed << l << " " << vl << " " << a << endl;
    //cerr << "--------------------" << endl;
}

int main()
{
    int t;
    cin >> t;
    for(int i=0; i<t; i++)
    {
        doCase();
    }
}