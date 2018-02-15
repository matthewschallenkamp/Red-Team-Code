#include <limits>
#include <unordered_map>
#include <queue>
#include <stack>

using namespace std;

class FlowNetwork
{
    struct pipe
    {
        int64_t cap, flow;
    };

    struct node
    {
        //Connected nodes
        unordered_map<int64_t, pipe> edges;

        //Information for updates during search
        uint64_t touched = 0;
        int64_t parent;

        //Find max flow as part of search routine
        int64_t maxFlow;
    };

    unordered_map<int64_t, node> _nodes;
    uint64_t _lastTouch = 0;

    bool _search(int64_t from, int64_t to)
    {
        //Increment touch counter to 
        //mark all nodes as untouched this search
        _lastTouch++;

        //Add start node to search list
        //queue<int64_t> search;      // Swap lines
        stack<int64_t> search;    // for bfs/dfs
        search.push(from);

        //Mark first node as touched
        //And set flow to it infinite
        _nodes[from].touched = _lastTouch;
        _nodes[from].maxFlow = numeric_limits<int64_t>::max();

        while(search.size())
        {
            //int64_t currInd = search.front(); // Swap lines
            int64_t currInd = search.top(); // for bfs/dfs
            search.pop();

            node& currNode = _nodes[currInd];
            for(auto& edge : currNode.edges)
            {
                pipe& p = edge.second;
                node& child = _nodes[edge.first];

                //Step to untouched children with positive flow remaining 
                int64_t pipeFlow = p.cap - p.flow;
                if(pipeFlow > 0 && child.touched < _lastTouch)
                {
                    //Mark child touched so it's not stepped on from elsewhere
                    child.touched = _lastTouch;

                    //Mark where we stepped to child from
                    child.parent = currInd;

                    //Mark min flow available thus far to reach this child
                    child.maxFlow = std::min(currNode.maxFlow, pipeFlow);

                    //Check if we're done
                    if(edge.first == to) return true;

                    search.push(edge.first);
                }
            }
        }

        return false;
    }

public:
    //Adds an edge from 'from' to 'to' with
    //max capacity 'capacity'
    //If an edge from -> to or to -> from previously existed,
    //it is deleted
    void addEdge(int64_t from, int64_t to, int64_t capacity = numeric_limits<int64_t>::max())
    {
        //Add forward edge
        _nodes[from].edges[to] = pipe{capacity, 0};

        //Add backwards edge
        _nodes[to].edges[from] = pipe{0, 0};
    }

    //Computes the max flow in the graph from 'from' to 'to'
    //with an optional upper limit for exiting early
    int64_t maxFlow(int64_t from, int64_t to, 
                    int64_t upperLimit = numeric_limits<int64_t>::max())
    {
        int64_t flow = 0;

        while(_search(from, to))
        {
            //Get min edge flow along path
            int64_t maxAmnt = _nodes[to].maxFlow;

            //Adjust flows of edges
            int64_t curr = to;
            while(curr != from)
            {
                int64_t parent = _nodes[curr].parent;

                pipe& p1 = _nodes[parent].edges[curr];
                pipe& p2 = _nodes[curr].edges[parent];

                p1.flow += maxAmnt;
                p2.flow -= maxAmnt;

                curr = parent;
            }

            //Add new flow amount to total flow
            flow += maxAmnt;
            if(flow >= upperLimit)
            {
                flow = upperLimit;
                break;
            }
        }
        return flow;
    }

};

/*
class FlowNetwork
{
    struct pipe{int64_t cap, flow;};

    struct node
    {
        unordered_map<int64_t, pipe> edges;
        uint64_t touched = 0;
        int64_t parent, maxFlow;
    };

    unordered_map<int64_t, node> _nodes;
    uint64_t _lastTouch = 0;

    bool _search(int64_t from, int64_t to)
    {
        _lastTouch++;
        //queue<int64_t> search;      // Swap lines
        stack<int64_t> search;    // for bfs/dfs
        search.push(from);
        _nodes[from].touched = _lastTouch;
        _nodes[from].maxFlow = numeric_limits<int64_t>::max();

        while(search.size())
        {
            //int64_t currInd = search.front(); // Swap lines
            int64_t currInd = search.top(); // for bfs/dfs
            search.pop();
            node& currNode = _nodes[currInd];
            for(auto& edge : currNode.edges)
            {
                pipe& p = edge.second;
                node& child = _nodes[edge.first];
                int64_t pipeFlow = p.cap - p.flow;
                if(pipeFlow > 0 && child.touched < _lastTouch)
                {
                    child.touched = _lastTouch; child.parent = currInd;
                    child.maxFlow = std::min(currNode.maxFlow, pipeFlow);
                    if(edge.first == to) return true;
                    search.push(edge.first);
                }
            }
        }

        return false;
    }

public:
    void addEdge(int64_t from, int64_t to, int64_t capacity = numeric_limits<int64_t>::max())
    {   _nodes[from].edges[to] = pipe{capacity, 0};     _nodes[to].edges[from] = pipe{0, 0};    }

    int64_t maxFlow(int64_t from, int64_t to, 
                    int64_t upperLimit = numeric_limits<int64_t>::max())
    {
        int64_t flow = 0;
        while(_search(from, to))
        {
            int64_t maxAmnt = _nodes[to].maxFlow, curr = to;
            while(curr != from)
            {
                int64_t parent = _nodes[curr].parent;
                pipe& p1 = _nodes[parent].edges[curr].flow += maxAmnt;
                pipe& p2 = _nodes[curr].edges[parent].flow -= maxAmnt;
                curr = parent;
            }
            flow += maxAmnt;
            if(flow >= upperLimit){flow = upperLimit;break;}
        }
        return flow;
    }
};
*/

/////////////////////////////////////////////////////////////////////////////////
// Example use case: AvoidingTheApocalypse
/////////////////////////////////////////////////////////////////////////////////
#include<iostream>

#define node_time(_i, _t) ((_i) + nodecount * (_t))
void doCase()
{
    //Define sink node as -1 since it's not a node number
    //in the problem
    constexpr int64_t SINK = -1;
    FlowNetwork fn;

    int nodecount, start, people, timeLimit, medics;
    cin >> nodecount >> start >> people >> timeLimit >> medics;

    //Read all locations with medics
    int64_t x;
    for(int i=0; i<medics; i++)
    {
        cin >> x;
        //Connect medic station at all valid time points
        //to sink node
        for(int t=0; t<=timeLimit; t++)
            fn.addEdge(node_time(x-1, t), SINK);
    }

    int r;
    cin >> r;

    //Connect every node to itself one step in the future
    //So people can wait
    for(int i=0; i<nodecount; i++)
        for(int t=0; t<timeLimit; t++)
            fn.addEdge(node_time(i, t), node_time(i, t+1));

    int a, b, p, d;
    for(int i=0; i<r; i++)
    {
        cin >> a >> b >> p >> d;

        //Connect every node to its child at each allowable time point
        for(int t=0; t <= timeLimit - d; t++)
            fn.addEdge(node_time(a-1, t), node_time(b-1, t+d), p);
        
    }

    cout << fn.maxFlow(node_time(start-1, 0), SINK, people) << endl;
}

int main()
{
    int t;
    cin >> t;

    for(int i=0; i < t; i++)
        doCase();

    return 0;
}