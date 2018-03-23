#include <bits/stdc++.h>
using namespace std;

bool nextGrid[304][304];

//returns row of error
int process(vector<vector<bool> > &grid) {
    int row = grid.size();
    int col = grid[0].size();
    if(row <= 2 || col <= 2) return -2;
    for(int i = 0; i < row+2; ++i)
        for(int j = 0; j < col+2; ++j)
            nextGrid[i][j] = false;
    int errRow = -1;
    bool cnt;
    for(int i = 0; i < row-2; ++i) {
        for(int j = 0; j < col-2; ++j) {
            cnt = nextGrid[i][j];
            cnt ^= nextGrid[i][j+1];
            cnt ^= nextGrid[i+1][j];
            cnt ^= grid[i][j];
            if(i-1 >= 0 && j-1 >= 0)
                cnt ^= nextGrid[i-1][j-1];
            if(i-1 >= 0) {
                cnt ^= nextGrid[i-1][j];
                cnt ^= nextGrid[i-1][j+1];
            }
            if(j-1 >= 0) {
                cnt ^= nextGrid[i][j-1];
                cnt ^= nextGrid[i+1][j-1];
            }
            nextGrid[i+1][j+1] = cnt;
        }
        for(int j = col-2; j < col; ++j) {
            cnt = nextGrid[i][j-1];
            cnt ^= nextGrid[i][j];
            cnt ^= nextGrid[i+1][j-1];
            cnt ^= nextGrid[i+1][j];
            if(i-1 >= 0) {
                cnt ^= nextGrid[i-1][j-1];
                cnt ^= nextGrid[i-1][j];
            }
            if(cnt != grid[i][j]) {
                errRow = i;
                break;
            }
        }
        if(errRow != -1) break;
    }
    if(errRow == -1) {
        for(int i = row-2; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                cnt = nextGrid[i-1][j] ^ nextGrid[i][j];
                cnt ^= nextGrid[i-1][j+1] ^ nextGrid[i][j+1];
                if(j-1>=0) cnt ^= nextGrid[i-1][j-1] ^ nextGrid[i][j-1];
                if(cnt != grid[i][j]) {
                    errRow = i;
                    break;
                }
            }
            if(errRow != -1) break;
        }
    }
    if(errRow == -1) return -1;
    for(int i = 0; i < row+2; ++i)
        for(int j = 0; j < col+2; ++j)
            nextGrid[i][j] = false;
    for(int i = row-1; i >= 0; --i) {
        if(i == errRow) break;
        for(int j = 0; j < col-2; ++j) {
            cnt = nextGrid[i][j];
            cnt ^= nextGrid[i][j+1];
            cnt ^= nextGrid[i+1][j];
            cnt ^= nextGrid[i+1][j+1];
            cnt ^= grid[i][j];
            if(i-1 >= 0 && j-1 >= 0)
                cnt ^= nextGrid[i-1][j-1];
            if(i-1 >= 0)
                cnt ^= nextGrid[i-1][j];
            if(j-1 >= 0) {
                cnt ^= nextGrid[i][j-1];
                cnt ^= nextGrid[i+1][j-1];
            }
            nextGrid[i-1][j+1] = cnt;
        }
        for(int j = col-2; j < col; ++j) {
            cnt = nextGrid[i][j-1];
            cnt ^= nextGrid[i][j];
            cnt ^= nextGrid[i+1][j-1];
            cnt ^= nextGrid[i+1][j];
            if(i-1 >= 0) {
                cnt ^= nextGrid[i-1][j-1];
                cnt ^= nextGrid[i-1][j];
            }
            if(cnt != grid[i][j]) {
                if(i != errRow) {
                    return -2;
                }
                return errRow;
            }
        }
    }
    return errRow;
}

void iterate(vector<vector<bool> > &grid, int errI, int errJ) {//call this when you know where the error is
    int row = grid.size();
    int col = grid[0].size();
    for(int i = 0; i < row+2; ++i)
        for(int j = 0; j < col+2; ++j)
            nextGrid[i][j] = false;
    bool cnt;
    int startI = row+2, endI = 0, startJ = col+2, endJ = 0;
    for(int i = 0; i < row-2; ++i) {
        for(int j = 0; j < col-2; ++j) {
            cnt = nextGrid[i][j];
            cnt ^= nextGrid[i][j+1];
            cnt ^= nextGrid[i+1][j];
            cnt ^= grid[i][j];
            if(i-1 >= 0 && j-1 >= 0)
                cnt ^= nextGrid[i-1][j-1];
            if(i-1 >= 0) {
                cnt ^= nextGrid[i-1][j];
                cnt ^= nextGrid[i-1][j+1];
            }
            if(j-1 >= 0) {
                cnt ^= nextGrid[i][j-1];
                cnt ^= nextGrid[i+1][j-1];
            }
            nextGrid[i+1][j+1] = cnt;
            if(i == errI && j == errJ) nextGrid[i+1][j+1] ^= 1;
            if(nextGrid[i+1][j+1]) {
                startI = min(startI, i+1);
                endI = max(endI, i+1);
                startJ = min(startJ, j+1);
                endJ = max(endJ, j+1);
            }
        }
    }
    grid.clear();
    grid.resize(endI-startI+1, vector<bool>(endJ-startJ+1));
    for(int i = startI; i <= endI; ++i) {
        for(int j = startJ; j <= endJ; ++j) {
            grid[i-startI][j-startJ] = nextGrid[i][j];
        }
    }
}

int main() {
    int row, col;
    cin >> row >> col;
    swap(row,col);
    string s;
    vector<vector<bool> > grid(row, vector<bool>(col));
    vector<vector<bool> > grid90;
    for(int i = 0; i < row; ++i) {
        cin >> s;
        for(int j = 0; j < col; ++j) {
            grid[i][j] = (s[j]=='#');
        }
    }
    for(;;) {
        row = grid.size();
        col = grid[0].size();
        int errRow = process(grid);
        if(errRow == -2) break;
        if(errRow == -1) {
            iterate(grid, -1, -1);
            continue;
        }
        grid90.clear();
        grid90.resize(col, vector<bool>(row));
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                grid90[j][i] = grid[i][j];
            }
        }
        int errCol = process(grid90);
        iterate(grid, errRow, errCol);
    }
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
            cout << (grid[i][j]?'#':'.');
        }
        cout << '\n';
    }
    return 0;
}




















