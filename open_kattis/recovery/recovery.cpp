#include <iostream>
#include <vector>
using namespace std;

int main() {
  string colsi, rowsi;
  cin >> rowsi >> colsi;
  vector<bool> cols, rows;
  cols.resize(colsi.size()); rows.resize(rowsi.size());
  for(int i = 0; i < cols.size(); i++) {
    cols[i] = colsi[i] == '1';
  }
  for(int i = 0; i < rows.size(); i++)
  {
    rows[i] = rowsi[i] == '1';
  }
  vector<vector<bool> > board(rows.size(), vector<bool>(cols.size(), true));
  vector<bool> rowsol(rows.size(), cols.size() % 2);
  vector<bool> colsol(cols.size(), rows.size() % 2);
  //first pass, fix all doubles
  //gotta fix all the extras in the first row/col
  int rc = 0, cc = 0;
  for(int i = 0; i < rows.size(); i++)
  {
    if(rows[i] != rowsol[i]) rc++;
  }
  for(int j = 0; j < cols.size(); j++) {
    if(cols[j] != colsol[j]) cc++;
  }
  int diff = cc - rc;
  for(int j = 0; j < cols.size() && diff > 0; j++) {
    if(cols[j] != colsol[j]) {
        rowsol[0] = !rowsol[0];
        colsol[j] = !colsol[j];
        board[0][j] = !board[0][j];
        diff--;
    }
  }
  for(int i = 0; i < rows.size() && diff < 0; i++) {
    if(rows[i] != rowsol[i]) {
        rowsol[i] = !rowsol[i];
        colsol[0] = !colsol[0];
        board[i][0] = !board[i][0];
        diff++;
    }
  }
  
  for(int i = 0; i < rows.size(); i++)
  {
    for(int j = 0; j < cols.size(); j++) {
      if(rowsol[i] != rows[i] && colsol[j] != cols[j]) {
        rowsol[i] = !rowsol[i];
        colsol[j] = !colsol[j];
        board[i][j] = !board[i][j];
      }
    }
  }

  if(rowsol != rows || colsol != cols) {
    cout << -1 << endl;
    return 0;
  }
  for(int i = 0; i < rows.size(); i++)
  {
    for(int j = 0; j < cols.size(); j++) {
      cout << board[i][j];
    }
    cout << endl;
  }
  return 0;
}
