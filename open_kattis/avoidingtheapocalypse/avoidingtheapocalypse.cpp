#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <unordered_map>
#include <limits>
#include <queue>
#include <array>
#include <stack>

using namespace std;

constexpr int maxNodes = 1000 * (100 + 1) + 5;
typedef unsigned long long ull;

class node;
struct road
{
    int cap = 0;
    int flow = 0;
    road* inverse;
    node* to;
    string name;
};

struct node
{
    //Connections and amount that fit
    vector<road*> roads; 

    road* parentRoad;
    node* parent;
    ull lastTouch = 0;
};


//BFS Version, takes too long on large case
int find_path(node* start, node* end, vector<node>& nodes, array<road*, maxNodes>& path, int& min_flow)
{
    ////cerr << "BFS " << start << " -> " << end << endl;
    static ull roundNum = 0;
    roundNum++;

    //Start at first node
    //queue<node*> q;
    stack<node*> q;
    q.push(start);
    start->lastTouch = roundNum;

    //Travel until end node found
    bool done = false;
    while(!q.empty() && !done)
    {
        node* c = q.top();
        q.pop();

        ////cerr << "-----------------" << endl;
        ////cerr << "Enter node " << c << endl;

        auto it = c->roads.begin();
        auto endit = c->roads.end();
        for(; it != endit; it++)
        {
            road* r = *it;
            node* n = r->to;

            ////cerr << "Look at child " << r.first << endl;

            //Check if we've already traveled to node
            if(n->lastTouch < roundNum)
            {
                ////cerr << "Unvisited " << endl;
                ////cerr << "Flow over road " << r.second->name << " : " << r.second->cap << " - " << r.second->flow << endl;

                //If not, check if it's valid to travel to
                if(r->cap > r->flow)
                {
                    ////cerr << "Travel to child" << endl;

                    n->parent = c;
                    n->parentRoad = r;
                    n->lastTouch = roundNum;

                    q.push(n);

                    //If it was valid to travel to, and the last node, we're done
                    if(n == end)
                    {
                        ////cerr << "Final node" << endl;
                        done = true;
                        break;
                    }
                }
            }
        }
    }

    ////cerr << "Backtrace steps" << endl;
    if(end -> lastTouch != roundNum) return 0;
    node* c = end;

    min_flow = numeric_limits<int>::max();
    int i=0;
    ////cerr << "Enter node " << end << endl;
    while(c != start)
    {
        ////cerr << "Took road " << c.second->name << " from node " << c.first << endl;
        path[i++] = c->parentRoad;
        min_flow = std::min(min_flow, c->parentRoad->cap - c->parentRoad->flow);

        ////cerr << "Enter node " << c.first << endl;
        c = c->parent;
    }
    return i;
}

int ford_fulkerson(node* start, node* end, vector<node>& nodes, int max)
{
    ////cerr << "Ford-Fulkerson start" << endl;

    int out = 0;
    static array<road*, maxNodes> p = array<road*, maxNodes>();
    do
    {
        int min_flow;
        int len = find_path(start, end, nodes, p, min_flow);
        ////cerr << "Found path length " << len << endl;

        if(len == 0) return out;
    
        ////cerr << "Flow over path: " << min_flow << endl;

        int count = 0;
        for(auto it = p.begin(); count<len; count++, it++)
        {
            (*it)->flow += min_flow;
            (*it)->inverse->flow -= min_flow;
        }

        out += min_flow;
        if(out >= max) return max;
        ////cerr << endl;
    }while(true);
}

#define node_time(_i, _t) ((_i) + nodecount * (_t))
void doCase()
{
    //Number of nodes
    //Start node
    //Number of people
    //Amount of time to get to safety
    //Number of safe locations
    int nodecount, start, people, timeLimit, medics;
    cin >> nodecount >> start >> people >> timeLimit >> medics;
    start--;

    vector<int> medLocs(medics);
    for(int i=0; i<medics; i++)
    {
        cin >> medLocs[i];
    }

    int r;
    cin >> r;

    //Node for each location at each time + supersink
    vector<node> nodes(nodecount * (timeLimit + 1) + 1);
    vector<road> roads(((nodecount + r) * timeLimit + medics * (timeLimit + 1)) * 2);

    //cerr << nodes.size() << " nodes" << endl;
    //cerr << roads.size() << " roads" << endl;
    node& sink = nodes.back();

    int roadCount = 0;

    ////cerr << "=================================================" << endl;
    ////cerr << nodecount << " locations, " << start << " is beginning" << endl;

    //Set of all locations with medics
    for(int x : medLocs)
    {
        x--;
        
        //Connect medic station at all valid time points
        //to sink node
        for(int t=0; t<=timeLimit; t++)
        {
            road& r = roads[roadCount++];
            road& r_i = roads[roadCount++];

            r.cap = numeric_limits<int>::max();
            r.inverse = &r_i;
            r.to = &sink;
            r_i.inverse = &r;
            r_i.to = &nodes[node_time(x, t)];

            //r->name = to_string(x) + ", " + to_string(t) + " -> sink";
            //r_i->name = "sink -> " + to_string(x) + ", " + to_string(t);

            nodes[node_time(x, t)].roads.push_back(&r);
            sink.roads.push_back(&r_i);

            //////cerr << "Connect " << x << ", " << t << "(" << node_time(x, t) << ") to sink" << endl;
        }
    }

    //Connect every node to itself one step in the future
    //So people can wait
    for(int i=0; i<nodecount; i++)
    {
        for(int t=0; t<timeLimit; t++)
        {
            road& r = roads[roadCount++];
            road& r_i = roads[roadCount++];

            r.cap = numeric_limits<int>::max();
            r.inverse = &r_i;
            r.to = &nodes[node_time(i, t+1)];
            r_i.inverse = &r;
            r_i.to = &nodes[node_time(i, t)];

            //r->name = to_string(i) + ", " + to_string(t) + " -> " + to_string(i) + ", " + to_string(t+1);
            //r_i->name = to_string(i) + ", " + to_string(t+1) + " -> " + to_string(i) + ", " + to_string(t);

            nodes[node_time(i, t)].roads.push_back(&r);
            nodes[node_time(i, t+1)].roads.push_back(&r_i);

            //////cerr << "Connect " << i << ", " << t << "(" << node_time(i, t) << ") to " << i << ", " << t+1 << "(" << node_time(i, t+1) << ")" << endl;
        }
    }

    int a, b, p, d;
    for(int i=0; i<r; i++)
    {
        cin >> a >> b >> p >> d;
        a--;
        b--;

        //Connect every node to its child at each allowable time point
        for(int t=0; t <= timeLimit - d; t++)
        {
            road& r = roads[roadCount++];
            road& r_i = roads[roadCount++];

            r.cap = p;
            r.inverse = &r_i;
            r.to = &nodes[node_time(b, t+d)];
            r_i.inverse = &r;
            r_i.to = &nodes[node_time(a, t)];

            //r->name = to_string(a) + ", " + to_string(t) + " -> " + to_string(b) + ", " + to_string(t+d);
            //r_i->name = to_string(b) + ", " + to_string(t+d) + " -> " + to_string(a) + ", " + to_string(t);

            nodes[node_time(a, t)].roads.push_back(&r);
            nodes[node_time(b, t+d)].roads.push_back(&r_i);

            //////cerr << "Connect " << a << ", " << t << "(" << node_time(a, t) << ") to " << b << ", " << t+d << "(" << node_time(b, t+d) << ")" << endl;
        }
    }
    //cerr << "Used " << roadCount << " roads" << endl;

    cout << ford_fulkerson(&nodes[node_time(start, 0)], &sink, nodes, people) << endl;
}

int main()
{
    int t;
    cin >> t;

    for(int i=0; i < t; i++)
        doCase();

    return 0;
}