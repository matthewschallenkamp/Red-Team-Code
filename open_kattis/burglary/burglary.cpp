#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;


int n, m, leftSeg, rightSeg;
string shelfCookies[1005];
vector<int> ladders[1005];//location
int prefixSum[1005][5005], memo[1005][11][11];//memo[i][j][j] = max cookies given row i and lower, and ladders at index j,k
int prefixCnt[1005][5005];
int nextCookie[1005][5005], prevCookie[1005][5005];//index

int main()
{
    cin >> n >> m;
    string row, lastRow;
    cin >> lastRow;
    for(int i = 0; i < n; ++i) {
        cin >> row;
        for(int j = 0; j < m; ++j) {
            if(row[j] == '|') {
                ladders[i].push_back(j);
            }
        }
        if(i < n-1) cin >> row;
        else row = lastRow;
        shelfCookies[i] = row;
    }
    //calculate prefix sums/counts on cookies and next/prev cookies
    for(int row = 0; row < n; ++row) {
        if('0' <= shelfCookies[row][0] && shelfCookies[row][0] <= '9') {
            prefixCnt[row][0] = 1;
            prefixSum[row][0] = shelfCookies[row][0]-'0';
        } else {
            prefixCnt[row][0] = 0;
            prefixSum[row][0] = 0;
        }
        for(int j = 1; j < m; ++j) {
            prefixSum[row][j] = prefixSum[row][j-1];
            prefixCnt[row][j] = prefixCnt[row][j-1];
            if('0' <= shelfCookies[row][j] && shelfCookies[row][j] <= '9') {
                prefixCnt[row][j]++;
                prefixSum[row][j] += shelfCookies[row][j]-'0';
            }
        }
        int prevIndex = 0;
        for(int j = 0; j < m; ++j) {
            if('0' <= shelfCookies[row][j] && shelfCookies[row][j] <= '9') {
                prevIndex = j;
            }
            prevCookie[row][j] = prevIndex;
        }
        int nextIndex = m-1;
        for(int j = m-1; j >= 0; --j) {
            if('0' <= shelfCookies[row][j] && shelfCookies[row][j] <= '9') {
                nextIndex = j;
            }
            nextCookie[row][j] = nextIndex;
        }
    }
    for(int row = n-2; row >= 0; --row) {
        for(int j = 0, size = ladders[row].size(); j < size; ++j) {
            int left = ladders[row][j];
            for(int k = j; k < size; ++k) {
                int right = ladders[row][k];
                memo[row][j][k] = prefixSum[row][nextCookie[row][right]] - (prevCookie[row][left]-1>=0?prefixSum[row][prevCookie[row][left]-1]:0);
                for(int jLower = 0, sizeLower = ladders[row+1].size(); jLower < sizeLower; ++jLower) {
                    int leftLower = ladders[row+1][jLower];
                    for(int kLower = jLower; kLower < sizeLower; ++kLower) {
                        int rightLower = ladders[row+1][kLower];
                        if(right <= leftLower && prefixCnt[row][leftLower] - (right-1>=0?prefixCnt[row][right-1]:0) >= 1) continue;
                        if(rightLower <= left && prefixCnt[row][left] - (rightLower-1>=0?prefixCnt[row][rightLower-1]:0) >= 1) continue;
                        if(j == k && '0' <= shelfCookies[row][left] && shelfCookies[row][left] <= '9') continue;
                        if(jLower == kLower && '0' <= shelfCookies[row][leftLower] && shelfCookies[row][leftLower] <= '9') continue;
                        int start1 = prevCookie[row][min(left, leftLower)];
                        int end1 = nextCookie[row][max(left, leftLower)];
                        int start2 = prevCookie[row][min(right, rightLower)];
                        int end2 = nextCookie[row][max(right, rightLower)];
                        int sum = 0;
                        if(end1 >= start2) {
                            sum += prefixSum[row][end2];
                            sum -= (start1-1>=0?prefixSum[row][start1-1]:0);
                        } else {
                            sum += prefixSum[row][end1];
                            sum -= (start1-1>=0?prefixSum[row][start1-1]:0);
                            sum += prefixSum[row][end2];
                            sum -= (start2-1>=0?prefixSum[row][start2-1]:0);
                        }
                        memo[row][j][k] = max(memo[row][j][k], memo[row+1][jLower][kLower] + sum);
                    }
                }
            }
        }
    }
    int largest = 0;
    for(int j = 0; j < ladders[0].size(); ++j) {
        for(int k = j; k < ladders[0].size(); ++k) {
            largest = max(largest, memo[0][j][k]);
        }
    }
    cout << largest << '\n';
}
































