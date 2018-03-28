#include <vector>
using namespace std;
vector<bool> *sieve(int n);

extern "C" {
  void *sieve_gen(int n) {
    return (void *)sieve(n);
  }
  bool sieve_get(void *self, int i) {
    return ((vector<bool> *)self)->at(i);
  }
  void sieve_delete(void *self) {
    delete (vector<bool> *) self;
  }
}