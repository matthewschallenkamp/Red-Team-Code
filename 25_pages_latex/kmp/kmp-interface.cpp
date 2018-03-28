#include "kmp.hpp"

extern "C" {
  void * kmp_create(const char* str) {
    string s(str);
    return (void*) new KMP_Match(s);
  }
  void * kmp_find(void *kmp, const char* str) {
    vector<int> *found = new vector<int>();
    string s(str);
    *found = ((KMP_Match *) kmp)->find(s);
    return (void *) found;
  }
  int kmp_res_get(void* res, int i) {
    return ((vector<int> *)res)->at(i);
  }
  int kmp_res_len(void* res) {
    return ((vector<int> *)res)->size();
  }
  void kmp_res_free(void* res) {
    delete ((vector<int> *)res);
  }
  void kmp_free(void* kmp) {
    delete ((KMP_Match *) kmp);
  }
}