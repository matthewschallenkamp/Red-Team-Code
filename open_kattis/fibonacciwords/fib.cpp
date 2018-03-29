#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ll;

const ll inf = 1e5+1;
ll Fib[101];
vector<ll> memo;

struct KMP_Match {
    vector<int> T;      //prefix table for KMP algorithm
    string pat;         //pattern for current prefix table and for searching

    //constructors: empty and with pattern, which builds prefix table
    KMP_Match() {};
    KMP_Match(string pattern) : pat(pattern) { this->buildTable(pat); };

    //build prefix table for KMP algorithm
    void buildTable(string pattern) {
        pat = pattern;
        T.clear();
        T.resize(pat.length()+1);       
        int i = 0, j = -1;
        T[i] = j;
        while (i < pat.size()) {
            while (j >= 0 && pat[i] != pat[j]) j = T[j];
            i++, j++;
            T[i] = j;
        }
    }

    //returns list of all match positions of pat in txt; if no matches, returns empty vector
    //if all = false, returns vector with single element, position of first match
    int find(string &txt)
    {
        int cnt = 0;
        int m = 0, i = 0;   //start of current match in txt, position in pat
    
        while (m + i < txt.length()) {      //search to end of txt
            if (pat[i] == txt[m+i]) {       //characters match              
                if (i == pat.length()-1) {  //end of pattern,  store location of match
                    cnt++;
                    m = m + i - T[i];           //move forward to continue searching
                    i = T[i];
                }   
                i++;        //move to next character of pat         
            }               
            else {                  //characters do not match, keep searching               
                if (T[i] != -1) {   //valid border, skip ahead
                    m = m + i - T[i];   //current pos + length of match - known matched
                    i = T[i];           //start just after known matched
                }               
                else {      //no valid border, try next character
                    i = 0;
                    m++;
                }
            }       
        }       
        return cnt;   //return list of matches
    }
};

char memoLetRev[101][inf];

char letterRev(int n, ll pos) {
    if(memoLetRev[n][pos] != '$') return memoLetRev[n][pos];
    if(n == 0) return '0';
    if(n == 1) return '1';
    if(pos < Fib[n-2]) return memoLetRev[n][pos] = letterRev(n-2,pos);
    return memoLetRev[n][pos] = letterRev(n-1,pos-Fib[n-2]);
}

char memoLet[101][inf];

char letter(int n, ll pos) {
    if(memoLet[n][pos] != '$') return memoLet[n][pos];
    if(n == 0) return '0';
    if(n == 1) return '1';
    if(pos < Fib[n-1]) return memoLet[n][pos] = letter(n-1, pos);
    return memoLet[n][pos] = letter(n-2, pos - Fib[n-1]);
}

ll ways(int n, string &s) {
    if(s.size() > Fib[n]) return 0;
    if(s.size() == Fib[n]) {
        for(int i = 0; i < s.size(); ++i) {
            if(s[i] != letter(n,i)) return memo[n] = 0;
        }
        return memo[n] = 1;
    }
    if(n == 0) return (s=="0");
    if(n == 1) return (s=="1");
    if(memo[n] != -1) return memo[n];
    int boundFirst = min((int)s.size()-2, (int)Fib[n-1]-1);
    int boundSecond = min((int)s.size()-2, (int)Fib[n-2]-1);
    string match(boundFirst + boundSecond + 2, ' ');
    for(int i = boundFirst; i >= 0; --i) {
        match[boundFirst-i] = letterRev(n-1,i);
    }
    for(int i = 0; i <= boundSecond; ++i) {
        match[boundFirst+1+i] = letter(n-2,i);
    }
    KMP_Match temp(s);
    ll cntOverLap = temp.find(match);
    return memo[n] = ways(n-1,s) + ways(n-2,s) + cntOverLap;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    for(int i = 0; i <= 100; ++i) {
        for(int j = 0; j < inf; ++j) {
            memoLetRev[i][j] = memoLet[i][j] = '$';
        }
    }
    int n;
    string s;
    Fib[0] = Fib[1] = 1;
    for(n = 2; n <= 100; ++n) {
        Fib[n] = min(Fib[n-1] + Fib[n-2], 5*inf);
    }
    int caseNum = 1;
    while(cin >> n >> s) {
        memo.clear();
        memo.resize(n+1,-1);
        cout << "Case " << caseNum++ << ": " << ways(n,s) << '\n';
    }
    return 0;
}









































