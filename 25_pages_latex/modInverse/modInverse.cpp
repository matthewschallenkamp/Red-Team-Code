#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

//returns x such that x*a == 1 (mod b)
//x only exists if gcd(a, b) == 1
ll modInverse(ll a, ll b) {	return 1 < a ? b - modInverse(b % a, a) * b / a : 1; }

int main() {
	for (int i = 2; i <= 10; i++) {
		for (int j = i + 1; j < 100; ++j) {
			if (__gcd(i, j) == 1)
				cout << i << ' ' << j << ' ' << modInverse(i, j) << '\n';
}	}	return 0;	}

//calculates multiplicative inverse of a(modb) s.t.a*x=1(modb)
//inverse only exists if a and b are relatively prime: gcd(a,b)=1
long long modInverse(long long a, long long b) {
	long long b0 = b, x0 = 0, x1 = 1, q, temp; if (b == 1)r eturn 1;
	while (a > 1) {
		q = a / b; //quotient temp=a;
		a = b;
		b = temp % b; //remainder temp=x0; x0=x1­q*x0; x1=temp;
	} if (x1 < 0)x1 += b0; return x1; }

//returns smallest integer x such that x%n[i]=a[i] for all i
long l ong chineseRemainder(vector<l ong l ong> n, vector<l ong l ong> a) {
	long l ong p, prod = 1, sm = 0; for (i nt i = 0; i < n.size(); i++)prod *= n[i]; for (i nt i = 0; i < n.size(); i++) {
		p = prod / n[i];
		sm += a[i] * modInverse(p, n[i]) * p;
	}
	return sm % prod; }
	
int main() {
	cout << modInverse(3, 7) << " " << modInverse(3, 11) << endl; //5,4 
	//mod=3,4,5; rem=2,3,1=> 11 mod 60;
	//mod=5,7;rem=1,3=> 31 mod 35 
	res=chineseRemainder(mod,rem);
}










