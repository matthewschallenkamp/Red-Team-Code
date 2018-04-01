#include <bits/stdc++.h>
using namespace std;


#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template<class TI>
using indexed_set = tree<TI,null_type,less<TI>,rb_tree_tag,tree_order_statistics_node_update>;

const int Max = 505, inf = (int)1e9;
bool seen[Max], goodSubstr[Max][Max];
int arr[Max], memo[Max][Max], cost[Max][Max], numSmaller[Max][Max][Max], least[Max][Max];

int minMoves(int i, int j) {
    if(i > j) return inf;
    if(goodSubstr[i][j]) return cost[i][j];
    if(memo[i][j] != -1) return memo[i][j];
    memo[i][j] = inf;
    for(int split = i; split < j; ++split) {
        memo[i][j] = min(memo[i][j], minMoves(i,split) + minMoves(split+1, j));
    }
    return memo[i][j];
}

int getCost(int i, int j) {
    if(i == j) return cost[i][j] = 0;
    if(i+1 == j) return cost[i][j] = 1;
    if(cost[i][j] != -1) return cost[i][j];
    cost[i][j] = inf;
    int posLeast;
    for(int k = i; k <= j; ++k) {
        if(arr[k] == least[i][j]) {
            posLeast = k;
            break;
        }
    }
    for(int split = i; split < j; ++split) {
        int mergeCost = j-i+1;
        if(posLeast <= split) {
            mergeCost -= numSmaller[i][split][least[split+1][j]];
        } else {
            mergeCost -= numSmaller[split+1][j][least[i][split]];
        }
        cost[i][j] = min(cost[i][j], getCost(i,split) + getCost(split+1, j) + mergeCost);
    }
    return cost[i][j];
}

int main() {
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i) cin >> arr[i];
    for(int i = 0; i < n; ++i) {
        for(int j = i; j < n; ++j) {
            indexed_set<int> bst;
            least[i][j] = inf;
            cost[i][j] = memo[i][j] = -1;
            int len = j-i+1;
            for(int k = 1; k <= len; ++k) {
                seen[k] = false;
            }
            for(int k = i; k <= j; ++k) {
                bst.insert(arr[k]);
                least[i][j] = min(least[i][j], arr[k]);
                seen[arr[k]] = true;
            }
            goodSubstr[i][j] = true;
            for(int k = 1; k <= len; ++k) {
                if(!seen[k]) {
                    goodSubstr[i][j] = false;
                    break;
                }
            }
            for(int k = 1; k <= 500; ++k) {
                numSmaller[i][j][k] = bst.order_of_key(k);
            }
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = i; j < n; ++j) {
            if(!goodSubstr[i][j]) continue;
            getCost(i,j);
        }
    }
    int temp = minMoves(0,n-1);
    if(temp == inf) {
        cout << "impossible\n";
    } else cout << temp << '\n';
    return 0;
}



























