//*insert UNION­FIND struct here, version with count but not size
//edge struct and sort function
struct Edge {int u, v, w;};

bool weightSort(Edge const t1, Edge const t2) {
	return (t1.w < t2.w);
}
//returns list of edges in MST, weight W of mst; 
//connected=T if one tree, F if forest 
//V=#vertices, E=#edges, edges contains all directed edges of graph
vector<Edge> minSpanTree(int V, int E, vector<Edge> edges, int &W, bool &connected) {
	vector<Edge> mst;
	union_find comp = union_find(V); //initially, each vertex in own component
	sort(edges.begin(), edges.end(), weightSort); // sort edges by weight, small­ > large
//add edges small to large that do not cause cycles
	W = 0;
	for (int i = 0; i < edges.size() && mst.size() < V­; i++) {
		if (!comp.share_set(edges[i].u, edges[i].v)) {
			comp.unite(edges[i].u, edges[i].v);
			mst.push_back(edges[i]);
			W += edges[i].w;
		}
	}
	connected = (comp.count == 1); // connected tree if all nodes in same set
	return mst;
}
