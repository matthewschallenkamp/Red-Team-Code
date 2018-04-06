int n;
vector<int> a(n);
int len = (int) sqrt (n + .0) + 1; bucket size
vector<int> b (len);
for (int i=0; i<n; ++i)
	b[i / len] += a[i];
 
int sum = 0;
int l, r; //read these
int c_l = l / len,   c_r = r / len;
if (c_l == c_r) //one block
	for (int i=l; i<=r; ++i)
		sum += a[i];
else {
	for (int i=l, end=(c_l+1)*len-1; i<=end; ++i)
		sum += a[i];
	for (int i=c_l+1; i<=c_r-1; ++i)
		sum += b[i];
	for (int i=c_r*len; i<=r; ++i)
		sum += a[i];
}
//can also make this lazy by adding an auxiliary b array
//essentially it stores the range common difference from range update queries
//then you add that in *len for ranges, and for singles
