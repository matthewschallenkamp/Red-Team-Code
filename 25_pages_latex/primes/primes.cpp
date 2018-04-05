#include <vector>
#include <ctgmath>
using namespace std;
vector<bool> *sieve(int n) {
  vector<bool> *res = new vector<bool>(n, true);
  vector<bool> &primes = *res; // referencify to let operator [] work
  primes[0] = primes[1] = false;
  int m = sqrt(n);
  for (int i = 2; i < m; ++i)
    if (primes[i])
      for (int k = i*i; k < n; k += i) primes[k] = false;
  return res;
}
