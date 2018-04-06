#include <iostream>
using namespace std;

constexpr uint64_t ITER = 450000000;

int main()
{
    uint64_t j=0;
    for(uint64_t i = 1; i < ITER; i++)
        j+=ITER/i;
    
    cout << j << endl;
    return 0;
}