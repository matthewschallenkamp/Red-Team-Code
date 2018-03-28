#include <bits/stdc++.h>
using namespace std;

string arr[101][7], correct[101][7];
vector<string> digit[10];
bool fixedArr[7][21], blank[7][21], fixedCorrect[7][21], question[7][21];


            

int main() {
    digit[0] = {".XX.",
                "X..X",
                "X..X",
                "....",
                "X..X",
                "X..X",
                ".XX."};
                
    digit[1] = {"....",
                "...X",
                "...X",
                "....",
                "...X",
                "...X",
                "...."};
                
    digit[2] = {".XX.",
                "...X",
                "...X",
                ".XX.",
                "X...",
                "X...",
                ".XX."};
                
    digit[3] = {".XX.",
                "...X",
                "...X",
                ".XX.",
                "...X",
                "...X",
                ".XX."};
                
    digit[4] = {"....",
                "X..X",
                "X..X",
                ".XX.",
                "...X",
                "...X",
                "...."};
                
    digit[5] = {".XX.",
                "X...",
                "X...",
                ".XX.",
                "...X",
                "...X",
                ".XX."};
                
    digit[6] = {".XX.",
                "X...",
                "X...",
                ".XX.",
                "X..X",
                "X..X",
                ".XX."};
                
    digit[7] = {".XX.",
                "...X",
                "...X",
                "....",
                "...X",
                "...X",
                "...."};
                
    digit[8] = {".XX.",
                "X..X",
                "X..X",
                ".XX.",
                "X..X",
                "X..X",
                ".XX."};
                
    digit[9] = {".XX.",
                "X..X",
                "X..X",
                ".XX.",
                "...X",
                "...X",
                ".XX."};
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < 7; ++j) {
            cin >> arr[i][j];
            correct[i][j].resize(arr[i][j].size());
        }
    }
    for(int j = 0; j < 7; ++j) {
        for(int k = 0; k < 21; ++k) {
            fixedArr[j][k] = true;
            for(int i = 0; i < n; ++i) {
                if(arr[i][j][k] != arr[0][j][k]) fixedArr[j][k] = false;
            }
        }
    }
    for(int j = 0; j < 7; ++j) {
        for(int k = 0; k < 21; ++k) {
            blank[j][k] = false;
            if(k == 4 || k == 9 || k == 11 || k == 16) {
                blank[j][k] = true;
            } else if(k < 4) {
                if(digit[8][j][k] == '.') blank[j][k] = true;
            } else if(k < 9) {
                if(digit[8][j][k-5] == '.') blank[j][k] = true;
            } else if(k == 10) {
                if(!(j == 2 || j == 4)) blank[j][k] = true;
            } else if(k < 16) {
                if(digit[8][j][k-12] == '.') blank[j][k] = true;
            } else {
                if(digit[8][j][k-17] == '.') blank[j][k] = true;
            }
        }
    }
    vector<string> answer(7,string(21,'W'));
    for(int j = 0; j < 7; ++j) {
        for(int k = 0; k < 21; ++k) {
            question[j][k] = false;
            if(blank[j][k]) {
                answer[j][k] = '.';
            }
        }
    }
    bool isPossible = false;
    for(int hour = 0; hour < 24; ++hour) {
        for(int minute = 0; minute < 60; ++minute) {
            int h = hour;
            int m = minute;
            bool possibleTime = true;
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < 7; ++j) {
                    for(int k = 0; k < 21; ++k) {
                        if(blank[j][k]) {
                            correct[i][j][k] = answer[j][k] = '.';
                            continue;
                        } else if(k < 4) {
                            if(h/10 == 0) {
                                correct[i][j][k] = '.';
                            } else {
                                correct[i][j][k] = digit[h/10][j][k];
                            }
                        } else if(k < 9) {
                            correct[i][j][k] = digit[h%10][j][k-5];
                        } else if(k == 10) {
                            if(j == 2 || j == 4) correct[i][j][k] = 'X';
                        } else if(k < 16) {
                            correct[i][j][k] = digit[m/10][j][k-12];
                        } else {
                            correct[i][j][k] = digit[m%10][j][k-17];
                        }
                    }
                }
                m++;
                if(m == 60) {
                    m = 0;
                    h++;
                    if(h == 24) {
                        h = 0;
                    }
                }
            }
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < 7; ++j) {
                    for(int k = 0; k < 21; ++k) {
                        if(!fixedArr[j][k] && arr[i][j][k] != correct[i][j][k]) {
                            possibleTime = false;
                        }
                    }
                }
            }
            if(possibleTime) {
                isPossible = true;
                for(int j = 0; j < 7; ++j) {
                    for(int k = 0; k < 21; ++k) {
                        fixedCorrect[j][k] = true;
                        for(int i = 0; i < n; ++i) {
                            if(correct[i][j][k] != correct[0][j][k]) fixedCorrect[j][k] = false;
                        }
                    }
                }
                for(int j = 0; j < 7; ++j) {
                    for(int k = 0; k < 21; ++k) {
                        if(blank[j][k] || !fixedArr[j][k]) {
                            continue;
                        }
                        if(fixedCorrect[j][k]) {
                            if(arr[0][j][k] == correct[0][j][k]) {
                                question[j][k] = true;
                                answer[j][k] = '?';
                            } else if(arr[0][j][k] == 'X') {
                                answer[j][k] = '1';
                            } else {
                                answer[j][k] = '0';
                            }
                        } else {
                            if(arr[0][j][k] == 'X') {
                                answer[j][k] = '1';
                            } else answer[j][k] = '0';
                        }
                    }
                }
            }
        }
    }
    if(isPossible) {
        for(int j = 0; j < 7; ++j) {
            for(int k = 0; k < 21; ++k) {
                if(question[j][k]) cout << '?';
                else cout << answer[j][k];
            }
            cout << '\n';
        }
    } else cout << "impossible\n";
    return 0;
}
































