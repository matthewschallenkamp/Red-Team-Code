// Total mortgage
// P * (1 + r/12)^(years * 12)
//
// With payments
// pay * SUM(i => payment times)(1 + r/12))^((years * 12)-i)
//
// Geometric sum
// largest power - (smallest power - 1)

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

long double ownpow(long double d, int p)
{
  if(p == 1) return d;
  if(p == 0) return 1.0;
  long double sub = ownpow(d, p / 2);
  return ownpow(d, p % 2) * sub * sub;
}

int main()
{
    long double X, Y, N, r;
    do
    {
        cin >> X >> Y >> N >> r;
        if(X > 0.01)
        {
            r /= 100.0;
            long double morTotal = X * ownpow((1 + r/12.0), N * 12);
            long double payBase = 1.0 + (r/12.0);
            long double payTotal = Y * (ownpow(payBase,  (N * 12.0)) - 1.0) / (payBase - 1.0);
            if(r < 0.000001) {
              morTotal = X;
              payTotal = Y * 12.0 * N;
            }
            // cerr << fixed << setprecision(10) << morTotal << endl;
            // cerr << fixed << setprecision(10) << payTotal << " " << endl;

            cout << (payTotal >= morTotal ? "YES":"NO") << endl;
        }
    } while(X > 0.01);
    return 0;
}