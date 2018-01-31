#include <queue>
#include <iostream>
#include <vector>
#include <set>
#include <cstdio>

using namespace std;

void doCase()
{
    int16_t n, m, k;
    scanf("%hi %hi %hi ", &n, &m, &k);
    //cerr << n << " " << m << " " << k << endl;

    //Info about nodes
    vector<vector<int16_t>> edges(n);
    vector<set<int16_t>> parents(n);
    vector<int16_t> weights(n, 0);
    vector<bool> safe(n, true);
    int16_t safeCount = n;

    //List of leaf nodes
    vector<int16_t> leaves;

    //List of nodes in trees
    vector<int16_t> weighted;

    for(int16_t i=0; i<m; i++)
    {
        int16_t a, b;
        scanf("%hi %hi ", &a, &b);
        
        a--;
        b--;

        parents[a].insert(b);
        parents[b].insert(a);

        //cerr << a << " -> " << b << endl;
    }

    for(int16_t i=0; i<n; i++)
    {
        if(parents[i].size() == 1)
        {
            //cerr << "Leaf " << i << endl;
            leaves.push_back(i);
        }
    }

    //For each node that's a leaf
    //to begin with, weight it and
    //parents upward until a parent with > 1
    //possible parents found
    for(int16_t l : leaves)
    {
        do
        {            
            //Mark unsafe
            safe[l] = false;
            safeCount--;

            //Find max child weight
            int16_t maxChild = 0;
            for(int16_t e : edges[l])
            {
                maxChild = max(weights[e], maxChild);
            }

            //Set weight and include self
            weights[l] = maxChild+1;
            //cerr << "Unsafe " << l << " : " << weights[l] << endl;

            //We know there's only one parent
            //Remove this node as a parent of that node
            int16_t parent = *(parents[l].begin());
            parents[parent].erase(l);
            edges[parent].push_back(l);

            //Mark as a weighted node
            weighted.push_back(l);

            //Move to parent
            l = parent;
        }while(parents[l].size() == 1);
    }

    //Put all weighted nodes int16_to a priority queue
    //pair<weight, node>
    //This sorts by most weight
    priority_queue<pair<int16_t, int16_t>> pq;
    for(int16_t n : weighted)
    {
        pq.emplace(weights[n], n);
    }

    //Place boats until we run out of boats or
    //places that would benefit one
    while(pq.size() && k)
    {
        pair<int16_t, int16_t> t = pq.top();
        pq.pop();
        
        //If unsafe tree root,
        //secure longest path down tree
        if(!safe[t.second])
        {
            int16_t curr = t.second;
            //cerr << "Securing branch " << curr << endl;
            do
            {
                //cerr << "Move to " << curr << endl;
                
                //Mark the node safe
                safe[curr] = true;
                safeCount++;

                int16_t maxChildNum = -1;
                int16_t maxChildWeight = 0;

                for(int16_t c : edges[curr])
                {
                    //Find non-parent edge
                    //with largest weight
                    if(maxChildWeight < weights[c])
                    {
                        maxChildWeight = weights[c];
                        maxChildNum = c;
                    }
                }

                //Move to child
                curr = maxChildNum;

            //End when no child nodes found
            }while(curr >= 0);
            k--;
        }
    }

    cout << n - safeCount << endl;
}

int main()
{
    int16_t c;
    cin >> c;
    for(int16_t i=0; i<c; i++)
    {
        doCase();
    }
}