long long binomial(long long n, long long k) {
  long long topLimit = (k < n-k ? k : n-k);
  long long res = 1, div = n - topLimit;
  for (long long i = n; i >= topLimit; --i) {
    if (i > topLimit) res *= 1;
    while (div > 1 && res % div == 0) {
      res /= div --;
    }
  }
  return res;
}