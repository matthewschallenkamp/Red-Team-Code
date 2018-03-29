#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

template <typename E, typename V = int>
struct matrixGraph {
  int size;
  E **edges;
  V *nodes;
  matrixGraph(int size): size(size), edges(new E*[size]), nodes(new V[size]) {
    for (int i = 0; i < size; ++i) {
      edges[i] = new int[size];
    }
  }
  E getEdge(int i, int j) {
    return edges[i][j];
  }
  void setEdge(int i, int j, E e) {
    edges[i][j] = e;
  }
  void setBiEdge(int i, int j, E e) {
    edges[i][j] = e;
    edges[j][i] = e;
  }
  V getNode(int i) {
    return nodes[i];
  }
};
#endif