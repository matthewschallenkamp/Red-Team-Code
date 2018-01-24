#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>
#include <array>
#include <unordered_map>

using namespace std;

//Union type for doing union-find
//during Hopcroft-Karp

//Keeps track of if the union it is head of
//has an end or non-end node in it
//The hasEnd/hasNonend data is meaningless for non-head nodes
struct Union
{
    bool hasEnd = false;
    bool hasNonend = true;
    Union* head = this;
};

//Unions two sets and returns the head of the new union
Union* union_union(Union* u1, Union* u2)
{
    u1->head->hasEnd = u1->head->hasEnd || u2->head->hasEnd;
    u1->head->hasNonend = u1->head->hasNonend || u2->head->hasNonend;

    u2->head->head = u1->head;
    u2->head = u1->head;

    return u2->head;
}

//Returns the head of a union
Union* union_find(Union* u)
{
    while(u->head != u) u = u->head;
    return u;
}

//Graph node
//knows its children and what type of operation connects to them
//This is mapped both ways for easy lookup
//
//knows its parents for back-tracking from end to find unconnected nodes
//
//knows which union it is in during Hopcroft-Karp
struct node
{
    unordered_map<int, node*> ops;
    unordered_map<node*, int> children;
    vector<pair<node*, int>> parents;
    Union uni;
};

//Mark remove nodes from their parents if
//they can't be reached from the start or from the end
void minimize(vector<node>& factory)
{
    queue<node*> bfs;
    set<node*> visited;
    set<node*> reachableForward;
    set<node*> reachableBackward;

    //BFS forward to find all points that can be reached
    bfs.push(&factory[0]);
    visited.insert(&factory[0]);
    while(bfs.size())
    {
        node* curr = bfs.front();
        reachableForward.insert(curr);

        bfs.pop();

        for(auto& c : curr->children)
        {
            if(visited.insert(c.first).second)
            {
                bfs.push(c.first);
            }
        }
    }

    //BFS backwards to find all points that can reach end
    bfs.push(&factory.back());
    visited.clear();
    visited.insert(&factory.back());
    while(bfs.size())
    {
        node* curr = bfs.front();
        reachableBackward.insert(curr);
        
        bfs.pop();

        for(pair<node*, int> c : curr->parents)
        {
            if(visited.insert(c.first).second)
            {
                bfs.push(c.first);
            }
        }
    }

    //Any node not in both lists is removed as a child from its parents
    for(node& n : factory)
    {
        if(!reachableForward.count(&n) || !reachableBackward.count(&n))
        {
            for(pair<node*, int> p : n.parents)
            {
                p.first->children.erase(&n);
                p.first->ops.erase(p.second);
            }
        }
    }
}


//Use Hopkroft-Karp algorithm to determine
//equivalence
bool equivalent(vector<node>& f1, vector<node>& f2)
{
    union_union(&f1.front().uni, &f2.front().uni);

    stack<pair<node*, node*>> st;
    st.emplace(&f1[0], &f2[0]);

    while(st.size())
    {
        pair<node*, node*> p = st.top();
        st.pop();

        node* n1 = p.first;
        node* n2 = p.second;

        //Unusable children are removed from these lists,
        //so they need to be the same size
        if(n1->ops.size() != n2->ops.size()) return false;

        for(auto& p : n1->children)
        {
            //Check that all operations accessible from node1
            //are accessible from node2
            auto it = n2->ops.find(p.second);
            if(it == n2->ops.end()) return false;

            node* c1 = p.first;
            node* c2 = it->second;

            Union* u1 = union_find(&c1->uni);
            Union* u2 = union_find(&c2->uni);

            if(u1 != u2)
            {
                //Combine unions of matching children
                Union* u = union_union(u1, u2);

                //If combined union has an end node and a non end node,
                //not equivalent
                if(u->hasEnd && u->hasNonend) return false;

                st.emplace(c1, c2);
            }
        }
        
    }

    return true;
}

//Read all edges in a factory and store the connections
//store backwards direction as well
//so we can find nodes which can't reach the end
void readFactory(vector<node>& factory, int m, int n, int k)
{
    factory.clear();
    factory.resize(n);

    int from, to, op;
    for(int i = 0; i < m; i++)
    {
        cin >> from >> to >> op;
        factory[from].children[&factory[to]] = op;
        factory[from].ops[op] = &factory[to];

        factory[to].parents.emplace_back(&factory[from], op);
    }
    
    factory.back().uni.hasEnd = true;
    factory.back().uni.hasNonend = false;
}

//Read and minimize both factories,
//then check if they are equivalent
void doCase()
{
    int m1, n1, k1, m2, n2, k2;
    cin >> m1 >> n1 >> k1 >> m2 >> n2 >> k2;

    vector<node> factory1, factory2;
    readFactory(factory1, m1, n1, k1);
    readFactory(factory2, m2, n2, k2);

    minimize(factory1);
    minimize(factory2);

    if(!equivalent(factory1, factory2)) cout << "not ";
    cout << "eligible" << endl;
}

int main()
{
    int t;
    cin >> t;

    for(int i=0; i<t; i++)
        doCase();

    return true;
}