#include <bits/stdc++.h>
using namespace std;

vector<int> PrefixFunction(string s) {
    vector<int> p(s.size());
    int j = 0;
    for (int i = 1; i < s.size(); i++) {
        while (j > 0 && s[j] != s[i]) j = p[j-1];
        if (s[j] == s[i]) j++;
        p[i] = j;
    }   
    return p;
}

struct str {
    string s;
    int i;
    vector<int> overLap;
};

bool cmp(str x, str y) {
    if(x.overLap == y.overLap) return x.i < y.i;
    return x.overLap < y.overLap;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;
    vector<str> arr(k);
    string s;
    for(int i = 0; i < k; ++i) {
        cin >> s;
        arr[i].s = s;
        arr[i].i = i;
        vector<int> table = PrefixFunction(s);
        int pos = table.size()-1;
        while(pos > 0) {
            if(2*s.size() - table[pos] <= n) {
                arr[i].overLap.push_back(table[pos]);
            }
            pos = table[pos]-1;
        }
    }
    sort(arr.begin(), arr.end(), cmp);
    for(int i = 0; i < k; ++i) {
        cout << arr[i].s << '\n';
    }
    return 0;
}
