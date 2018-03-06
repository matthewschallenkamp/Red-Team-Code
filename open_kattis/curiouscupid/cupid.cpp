#include <bits/stdc++.h>
using namespace std;

const int Max = 5e4+2, languages = 1e6+1;
int block, totalMatches;
int men[Max], women[Max], answer[Max];
int cntShared[languages], cntMen[languages], cntWomen[languages];

struct query {
    int l, r, index;
};

bool cmp(query x, query y) {
    if(x.l/block == y.l/block) return x.r < y.r;
    return x.l < y.l;
}

void add(int pos) {
    if(men[pos] == women[pos]) {
        cntShared[men[pos]]++;
        totalMatches++;
        return;
    }
    if(cntWomen[men[pos]] > 0) {
        cntShared[men[pos]]++;
        cntWomen[men[pos]]--;
        totalMatches++;
    } else {
        cntMen[men[pos]]++;
    }
    if(cntMen[women[pos]] > 0) {
        cntShared[women[pos]]++;
        cntMen[women[pos]]--;
        totalMatches++;
    } else {
        cntWomen[women[pos]]++;
    }
}

void remove(int pos) {
    if(men[pos] == women[pos]) {
        cntShared[men[pos]]--;
        totalMatches--;
        return;
    }
    if(cntMen[men[pos]] > 0) {
        cntMen[men[pos]]--;
    } else {
        cntShared[men[pos]]--;
        cntWomen[men[pos]]++;
        totalMatches--;
    }
    if(cntWomen[women[pos]] > 0) {
        cntWomen[women[pos]]--;
    } else {
        cntShared[women[pos]]--;
        cntMen[women[pos]]++;
        totalMatches--;
    }
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    block = ceil(sqrt(n));
    for(int i = 0; i < n; ++i) cin >> men[i];
    for(int i = 0; i < n; ++i) cin >> women[i];
    vector<query> queries(m);
    for(int i = 0; i < m; ++i) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].index = i;
        answer[i] = 0;
    }
    sort(queries.begin(), queries.end(), cmp);
    int left = 0, right = 0;//store inclusive ranges, start at [0,0]
    totalMatches = 0;
    add(0);
    for(auto &q : queries) {
        while(left > q.l) {
            left--;
            add(left);
        }
        while(right < q.r) {
            right++;
            add(right);
        }
        while(left < q.l) {
            remove(left);
            left++;
        }
        while(right > q.r) {
            remove(right);
            right--;
        }
        answer[q.index] = totalMatches;
    }
    for(int i = 0; i < m; ++i) cout << answer[i] << '\n';
    return 0;
}













