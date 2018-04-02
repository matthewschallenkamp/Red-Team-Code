#include <bits/stdc++.h>
using namespace std;

vector<int> p(1000001,-1);//change size here if needed
int find(int x) {return p[x] < 0 ? x : p[x] = find(p[x]);}
void merge(int x, int y) {
    if((x=find(x)) == (y=find(y))) return;
    if(p[y] < p[x]) swap(x,y);
    p[x] += p[y];//parents store size of set as negative integer
    p[y] = x;
}

int32_t main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    cout << (find(1)==find(2)?"same set\n":"different set\n");
    cout << "size of set containing 1: " << -p[find(1)] << '\n';
    cout << "size of set containing 2: " << -p[find(2)] << "\n\n";
    merge(1,2);
    cout << (find(1)==find(2)?"same set\n":"different set\n");
    cout << "size of set containing 1: " << -p[find(1)] << '\n';
    cout << "size of set containing 2: " << -p[find(2)] << '\n';
    return 0;
}


























