#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef pair<long double, long double> ansType;

//N choose K with pascals identity
long double choose(ll n, ll k)
{
    static vector<vector<long double>> _map(401, vector<long double>(401, -1));

    long double& out = _map[n][k];

    if(out >= 0) return out;
    if(n < k) return out = 0;
    if(k == 0 || k == n) return out = 1;

    return out = choose(n-1, k) + choose(n-1, k-1);
}

//Get expected result with divide by 0 check
long double expected(ansType ans)
{
    return ans.second == 0 ? 0 : ans.first/ans.second;
}

void operator += (ansType& l, const ansType& r)
{
    l.first += r.first * r.second;
    l.second += r.second;
}

//Get average of two table sizes with one of them bounded ??
long double boundAvg(ll i, ll k, ll top, ll N, vector<int>& tables)
{
    int& ci = tables[i];
    int& ck = tables[k];
    if(top > N) return 0;

    return (ci + 1 + ck) / 2.0;
}

ansType solve(int i, int j, int r, int& N, int& T, vector<int>& tables)
{
    int m = tables.size();
    ansType cont[m][m];

    //Fill up cont [i, j) with expected value for range 
    //if no one else comes to restaurant
    for(int i = m-1; i>=0; i--)
    {
        cont[i][i] += make_pair(0, 1);

        for(int j = i+1; j<m; j++)
        {
            if(i + T >= j)
            {
                for(int k = i; k < j; k++)
                {
                    ansType add = make_pair(0, 0);
                    add.second = cont[i][k].second * cont[k+1][j].second *
                        (tables[k+1] - tables[i]) * choose(j-i-1,k-i);
                    add.first = expected(cont[i][k]) + expected(cont[k+1][j]) +
                        boundAvg(k+1, i, k+1, N, tables);

                    add.first = abs(add.first);
                    add.second = abs(add.second);

                    cont[i][j] += add;
                }
            }
        }
    }

    ansType total[m][T+1];
    
    //Fill up total[i][k] with expected when filling up the first i
    //tables with k groups
    for(int i=0; i<m; i++)
    {
        if(i <= T)
        {
            total[i][i] = cont[0][i];
        }
        for(int k=0; k<T+1; k++)
        {
            if(total[i][k].second > 0)
            {
                for(int j=i+1; j<m; j++)
                {
                    int new_cnt = k + j - i - 1;
                    if(new_cnt > T) continue;

                    ansType add = make_pair(0, 0);
                    add.second = total[i][k].second * cont[i+1][j].second *
                                 choose(new_cnt, k);
                    add.first = expected(total[i][k]) + expected(cont[i+1][j]);

                    total[j][new_cnt] += add;
                }
            }
        }
    }

    return total[m-1][T];
}

int main()
{
    int N, G, T;

    cin >> N >> G >> T;

    //N tables, T extra tables for overflow??
    //1 table extra because 1-based ??
    vector<int> tables(N+T+1); 

    //1 Based tables; read in table amounts
    //from problem
    for(int i=0; i<N; i++)
    {
        int& d = tables[i+1];

        cin >> d;

        //Doesn't make sense to have table bigger
        //than max group size; so bound by that
        if(d > G) d = G;
    }

    //Set extra tables to be able to hold any group
    for(int i=0; i<T; i++)
    {
        tables[N+1+i] = G;
    }

    sort(tables.begin(), tables.end());

    auto ans = solve(0, N+T, T, N, T, tables);
    cout << fixed << setprecision(9) << expected(ans) << endl;
}