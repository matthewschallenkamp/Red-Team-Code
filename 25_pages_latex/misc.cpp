//gray code finder
int g (int n) {
	return n ^ (n >> 1);
}

int rev_g (int g) {
	int n = 0;
	for (; g; g>>=1)
		n ^= g;
	return n;
}

//loop over all mask subpatterns (but 0) in ~n^3
for (int m=0; m<(1<<n); ++m)
	for (int s=m; s; s=(s-1)&m)
	  //mask calculation goes here

//center of mass equations:
//points is point average, ez
//for polygon edges, take the center of each edge weighted by the edge length
//for a triangle: take the vertexes and average them
//for polygon area: pick some point. per edge, calculate the area of the triangle with that point and the sign of the area based
//   on the order of the points. sum.
//for a polygon: pick a point, get the triangle for each edge and that point
//   average the triangles weighted by their area as a fraction of the polygon area
//   you do need to consider the sign of the triangles areas, so 

// Josephs algo:
// taking list of n items and a step k, repeatedly step k and remove. Which element is left?
//O(klogn)
int joseph (int n, int k) {
	if (n == 1)  return 0;
	if (k == 1)  return n-1;
	if (k > n)  return (joseph (n-1, k) + k) % n;
	int cnt = n / k;
	int res = joseph (n - cnt, k);
	res -= n % k;
	if (res < 0)  res += n;
	else  res += res / (k - 1);
	return res;
}

//Farey sequence, grows n^2/3 with n
def farey_function(n, descending=False):
    """Print the nth Farey sequence, either ascending or descending."""
    a, b, c, d = 0, 1, 1, n
    if descending: 
        a, c = 1, n-1
    print "%d/%d" % (a,b)
    while (c <= n and not descending) or (a > 0 and descending):
        k = int((n + b) / d)
        a, b, c, d = c, d, (k*c-a), (k*d-b)
        print "%d/%d" % (a,b)
