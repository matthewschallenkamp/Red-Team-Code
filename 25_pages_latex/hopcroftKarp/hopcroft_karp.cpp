#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <utility>
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
void minimize(vector<node>& dfa)
{
    queue<node*> bfs;
    unordered_set<node*> visited;
    unordered_set<node*> reachableForward;
    unordered_set<node*> reachableBackward;

    //BFS forward to find all points that can be reached
    bfs.push(&dfa[0]);
    visited.insert(&dfa[0]);
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
    bfs.push(&dfa.back());
    visited.clear();
    visited.insert(&dfa.back());
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
    for(node& n : dfa)
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

/*
struct Union {
    bool hasEnd = false, hasNonend = true;
    Union* head = this;
};

Union* union_union(Union* u1, Union* u2) {
    u1->head->hasEnd = u1->head->hasEnd || u2->head->hasEnd;
    u1->head->hasNonend = u1->head->hasNonend || u2->head->hasNonend;

    u2->head->head = u1->head;
    u2->head = u1->head;

    return u2->head;
}

Union* union_find(Union* u) {
    while(u->head != u) u = u->head;
    return u;
}

struct node {
    unordered_map<int, node*> ops;
    unordered_map<node*, int> children;
    vector<pair<node*, int>> parents;
    Union uni;
};

void minimize(vector<node>& dfa) {
    queue<node*> bfs;
    unordered_set<node*> visited, reachableForward, reachableBackward;

    bfs.push(&dfa[0]);
    visited.insert(&dfa[0]);
    while(bfs.size())
    {
        node* curr = bfs.front();
        reachableForward.insert(curr);
        bfs.pop();
        for(auto& c : curr->children)
            if(visited.insert(c.first).second)
                bfs.push(c.first);
    }

    bfs.push(&dfa.back());
    visited.clear();
    visited.insert(&dfa.back());
    while(bfs.size()) {
        node* curr = bfs.front();
        reachableBackward.insert(curr);
        bfs.pop();
        for(pair<node*, int> c : curr->parents)
            if(visited.insert(c.first).second)
                bfs.push(c.first);
    }

    for(node& n : dfa)
        if(!reachableForward.count(&n) || !reachableBackward.count(&n))
            for(pair<node*, int> p : n.parents)
            {
                p.first->children.erase(&n);
                p.first->ops.erase(p.second);
            }
}

bool HopcroftKarp(vector<node>& f1, vector<node>& f2) {
    union_union(&f1.front().uni, &f2.front().uni);

    stack<pair<node*, node*>> st;
    st.emplace(&f1[0], &f2[0]);

    while(st.size()) {
        pair<node*, node*> p = st.top();
        st.pop();

        node* n1 = p.first;
        node* n2 = p.second;
        if(n1->ops.size() != n2->ops.size()) return false;
        for(auto& p : n1->children) {
            auto it = n2->ops.find(p.second);
            if(it == n2->ops.end()) return false;

            node* c1 = p.first;
            node* c2 = it->second;

            Union* u1 = union_find(&c1->uni);
            Union* u2 = union_find(&c2->uni);

            if(u1 != u2){
                Union* u = union_union(u1, u2);
                if(u->hasEnd && u->hasNonend) return false;

                st.emplace(c1, c2);
            }
        }
    }
    return true;
}
*/