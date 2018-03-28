#include "graph.hpp"

extern "C" {
  void *mgi_new(int size) {
    return new matrixGraph<int, int>(size);
  }
  int mgi_getEdge(void *self, int i, int j) {
    return ((matrixGraph<int, int> *)self)->getEdge(i, j);
  }
  void mgi_setEdge(void *self, int i, int j, int e) {
    ((matrixGraph<int, int> *)self)->setEdge(i, j, e);
  }
  void mgi_setBiedge(void *self, int i, int j, int e) {
    ((matrixGraph<int, int> *)self)->setBiEdge(i, j, e);
  }
  void mgi_destroy(void *self) {
    delete (matrixGraph<int, int> *)self;
  }
}