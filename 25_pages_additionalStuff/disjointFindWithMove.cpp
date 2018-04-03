#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
private:
    struct node;
    struct group;
    struct node {
        group *parent;
        ll value;
    };
    struct group {
        group *parent;
        ll sum, size;
    };
    vector<group> groups;
    vector<node> nodes;
    group* find(ll i) {
        group *temp = nodes[i].parent;
        while(temp != temp->parent) {
            temp = temp->parent;
        }
        nodes[i].parent = temp;//path compression
        return temp;
    }
public:
    DisjointSet(ll n) {//input the number of nodes
        nodes.resize(n+1);
        groups.resize(n+1);
        for(ll i = 1; i <= n; i++) {
            nodes[i].value = i;
            nodes[i].parent = &groups[i];
            groups[i].parent = &groups[i];
            groups[i].sum = i;//one based indexing
            groups[i].size = 1;
        }
    }
    void move(ll a, ll b) {//move first node to second's group, time complexity: O(1)
        group *groupA = find(a), *groupB = find(b);
        if(groupA == groupB) return;//if a is already in b's group
        //update sum
        groupA->sum -= a;
        groupB->sum += a;
        //update size
        groupA->size--;
        groupB->size++;
        //update node pointers
        nodes[a].parent = groupB;
    }
    void merge(ll a, ll b) {//union, time complexity: O(1)
        group *groupA = find(a), *groupB = find(b);
        if(groupA == groupB) return;//same group
        if(groupA->size > groupB->size) {//union by size
            /*moving all nodes from group b to group a*/
            //update sums
            groupA->sum += groupB->sum;
            groupB->sum = 0;
            //update size
            groupA->size += groupB->size;
            groupB->size = 0;
            groupB->parent = groupA;
        } else {
            /*moving all nodes from group a to group b*/
            //update sums
            groupB->sum += groupA->sum;
            groupA->sum = 0;
            //update size
            groupB->size += groupA->size;
            groupA->size = 0;
            groupA->parent = groupB;
        }
    }
    void print(ll n) {//for output
        cout << find(n)->size << ' ' << find(n)->sum << endl;
    }
    bool sameSet(ll a, ll b) {
        return find(a) == find(b);
    }
};

int main() {ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    ll n, k;
    while(cin >> n >> k) {
        DisjointSet ds(n);
        while(k--) {
            ll opp, a, b;
            cin >> opp;
            switch(opp) {
            case 1://union
                cin >> a >> b;
                ds.merge(a, b);
                break;
            case 2://move
                cin >> a >> b;
                ds.move(a, b);
                break;
            case 3://print
                cin >> a;
                ds.print(a);
                break;
            }
        }
    }
    return 0;
}



