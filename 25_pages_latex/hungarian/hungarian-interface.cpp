#include <vector>
#include <cstdint>
using namespace std;
int64_t HungarianMatch(const vector<vector<int64_t>>& costs);

extern "C" {
  int64_t hungarian_match(int64_t** tbl, uint64_t height, uint64_t width ) {
    vector<vector<int64_t>> vect(height, vector<int64_t>(width));
    for(int i=0; i<height; i++)
        for(int j=0; j<width; j++)
            vect[i][j] = tbl[i][j];

    return HungarianMatch(vect);
  }
}