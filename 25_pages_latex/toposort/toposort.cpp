#define MAX 100 //max number of nodes 

int graph[MAX][MAX]; //adjacency matrix
int indegree[MAX]; //in degree of each vertex
int sorted[MAX]; //list of sorted vertices
int V, E; //#vertices,#edges

void computeInDegrees() {
	//compute indegrees of all nodes, save in table
	memset(indegree, 0, sizeof(indegree));
	for (int i = 0; i < V; i++)
		for (int j = 0; j < V; j++)
			if (graph[j][i])
				indegree[i]++;
}

//performs topological sort on graph, puts result in sorted
//returns false if not a DAG or,if need strict ordering, ordering not possible
bool toposort(bool strict = false) {
	queue<int> zeroin; //vertices of indegree 0
	int current, cnt = 0; //current vertex and processed vertex count
	computeInDegrees(); //compute indegree of all nodes
	for (int i = 0; i < V; i++) //push all nodes with indegree 0 to queue
		if (indegree[i] == 0)
			zeroin.push(i);
	while (!zeroin.empty()) { 
	//if need a strictly defined ordering and queue size > 1, return false 
		if (strict && zeroin.size() > 1) 
			return false;
		current = zeroin.front();
		zeroin.pop();
		sorted[cnt] = current;
		for (int i = 0; i < V; i++)
			if (graph[current][i])  //if have edge to i, indegree[i]­­; //decrement indegree of i
				if (indegree[i] == 0)
					zeroin.push(i); //degree = 0,push to q
		cnt += 1;
	}
	if (cnt != V)
		return false;  //not DAG, return false (still have toposort)
	else { //valid ordering, print and return true
		for (int i = 0; i < V; i++)
		{
			cout << sorted[i] << " ";
		}
		cout << endl;
		return true;
	}
}
