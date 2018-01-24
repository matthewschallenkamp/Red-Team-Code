#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <set>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

void doTest() {
    int length, s;
    cin >> length >> s;
    //possible == true iff there are no conflicting letters
    bool possible = true;
    string answer(length,'$');
    int index;
    string suffix;
    while(s--) {
        cin >> index >> suffix;
        index--;
        bool asterix = false;
        //go from end of both strings to start, breaking if an '*' is found
        //and setting characters in answer string
        for(int i = length-1, j=suffix.size()-1; i >= 0 && j >= 0; i--, j--) {
            if(suffix[j] == '*') {
                asterix = true;
                break;
            }
            if(answer[i] != '$' && answer[i] != suffix[j]) {
                possible = false;
            }
            answer[i] = suffix[j];
        }
        //if an '*' is found, there might be a prefix of characters which aren't
        //initialized in the answer string
        if(asterix) {
            for(int i = index; i < length; ++i) {
                if(suffix[i-index] == '*') break;
                if(answer[i] != '$' && answer[i] != suffix[i-index]) {
                    possible = false;
                }
                answer[i] = suffix[i-index];
            }
        }
    }
    //if theres any character which isn't initialized, impossible
    for(char c : answer) {
        if(c == '$') {
            possible = false;
        }
    }
    if(!possible) cout << "IMPOSSIBLE\n";
    else cout << answer << '\n';
}

int main() {
    int t;
    cin >> t;
    while(t--) {
        doTest();
    }
    return 0;
}
