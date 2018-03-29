typedef long long ll;
void fenwick_update(ll *a, int n, int i, int delta) {
  do a[i] += delta; while((i |= i+1) < n);
}
ll fenwick_sum(ll *a,int i) {
  ll sum = 0;
  while(i>0) { sum += a[i-1]; i &= i-1; }
  return sum;
}
ll fenwick_getsum(ll *a, int l, int r) {
  return fenwick_sum(a,r+1)-fenwick_sum(a,l);
}
