#include <bits/stdc++.h>
using namespace std;

int xMax, yMax;

struct window {
    int x1, y1, x2, y2, pos;
    window(){}
    window(int a, int b, int c, int d, int e) : x1(a), y1(b), x2(c), y2(d), pos(e) {}
    bool intersect(window w) {
        if(w.x2 >= this->x1 && this->x2 >= w.x1) {
            if(w.y2 >= this->y1 && this->y2 >= w.y1) {
                return true;
            }
        }
        return false;
    }
    bool inside(int x, int y) {
        if(this->x1 <= x && x <= this->x2) {
            if(this->y1 <= y && y <= this->y2) {
                return true;
            }
        }
        return false;
    }
};

bool cmpX(window a, window b) {
    return a.x1 < b.x1;
}

bool cmpY(window a, window b) {
    return a.y1 < b.y1;
}

bool cmpPos(window a, window b) {
    return a.pos < b.pos;
}

int moveX(vector<window> &arr, vector<int> &moved, int pos, int dist) {
    int over = 0;
    moved[pos] = dist;
    if(dist > 0) {
        for(int j = pos; j < arr.size(); ++j) {
            if(moved[j] == 0) continue;
            window curr = arr[j];
            dist = moved[j];
            over = max(over, curr.x2 + moved[j] - xMax);
            for(int i = j+1; i < arr.size(); ++i) {
                window left = arr[i];
                if(curr.y2 >= left.y1 && curr.y1 <= left.y2) {
                    int gap = left.x1 - curr.x2 - 1;
                    if(dist > gap) {
                        moved[i] = max(moved[i], dist-gap);
                    }
                }   
            }
        }
    } else {
        for(int j = pos; j >= 0; --j) {
            if(moved[j] == 0) continue;
            window curr = arr[j];
            dist = -moved[j];
            over = min(over, curr.x1 + moved[j]);
            for(int i = j-1; i >= 0; --i) {
                window right = arr[i];
                if(curr.y2 >= right.y1 && curr.y1 <= right.y2) {
                    int gap = curr.x1 - right.x2 - 1;
                    if(dist > gap) {
                        moved[i] = min(moved[i], gap-dist);
                    }
                }   
            }
        }
    }
    return over;
}

int moveY(vector<window> &arr, vector<int> &moved, int pos, int dist) {
    int over = 0;
    moved[pos] = dist;
    if(dist > 0) {
        for(int j = pos; j < arr.size(); ++j) {
            if(moved[j] == 0) continue;
            window curr = arr[j];
            dist = moved[j];
            over = max(over, curr.y2 + moved[j] - yMax);
            for(int i = j+1; i < arr.size(); ++i) {
                window up = arr[i];
                if(curr.x2 >= up.x1 && curr.x1 <= up.x2) {
                    int gap = up.y1 - curr.y2 - 1;
                    if(dist > gap) {
                        moved[i] = max(moved[i], dist-gap);
                    }
                }
            }
        }
    } else {
        for(int j = pos; j >= 0; --j) {
            if(moved[j] == 0) continue;
            window curr = arr[j];
            dist = -moved[j];
            over = min(over, curr.y1 + moved[j]);
            for(int i = j-1; i >= 0; --i) {
                window down = arr[i];
                if(curr.x2 >= down.x1 && curr.x1 <= down.x2) {
                    int gap = curr.y1 - down.y2 - 1;
                    if(dist > gap) {
                        moved[i] = min(moved[i], gap-dist);
                    }
                }
            }
        }
    }
    return over;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int x, y, dx, dy;
    cin >> xMax >> yMax;
    xMax--,yMax--;
    string command;
    vector<window> arr;
    int index = 1;
    while(cin >> command) {
        if(command == "OPEN") {
            cin >> x >> y >> dx >> dy;
            window temp(x,y,x+dx-1, y+dy-1, index);
            bool canInsert = true;
            if(temp.x2 > xMax) canInsert = false;
            if(temp.y2 > yMax) canInsert = false;
            if(canInsert) {
                for(window &w : arr) {
                    if(w.intersect(temp)) {
                        canInsert = false;
                        break;
                    }
                }
            }
            if(canInsert) {
                arr.push_back(temp);
            } else {
                cout << "Command " << index << ": OPEN - window does not fit\n";
            }
        } else if(command == "CLOSE") {
            cin >> x >> y;
            int i = 0;
            bool found = false;
            for(window &w : arr) {
                if(w.inside(x,y)) {
                    found = true;
                    arr.erase(arr.begin()+i);
                    break;
                }
                i++;
            }
            if(!found) {
                cout << "Command " << index << ": CLOSE - no window at given position\n";
            }
        } else if(command == "MOVE") {
            cin >> x >> y >> dx >> dy;
            if(dx != 0) {
                sort(arr.begin(), arr.end(), cmpX);
            } else {
                sort(arr.begin(), arr.end(), cmpY);
            }
            bool found = false;
            int i = 0;
            for(window &w : arr) {
                if(w.inside(x,y)) {
                    found = true;
                    if(dx != 0) {
                        vector<int> moved(arr.size(),0);
                        int over = moveX(arr, moved, i, dx);
                        if(over) {
                            cout << "Command " << index << ": MOVE - moved " << abs(dx)-abs(over) << " instead of " << abs(dx) << '\n';
                        }
                        moved.clear();
                        moved.resize(arr.size(),0);
                        moveX(arr, moved, i, (dx>0?abs(dx)-abs(over):abs(over)-abs(dx)));
                        int j = 0;
                        for(window &win : arr) {
                            win.x1 += moved[j];
                            win.x2 += moved[j];
                            j++;
                        }
                    } else {
                        vector<int> moved(arr.size(),0);
                        int over = moveY(arr, moved, i, dy);
                        if(over) {
                            cout << "Command " << index << ": MOVE - moved " << abs(dy)-abs(over) << " instead of " << abs(dy) << '\n';
                        }
                        moved.clear();
                        moved.resize(arr.size(),0);
                        moveY(arr, moved, i, (dy>0?abs(dy)-abs(over):abs(over)-abs(dy)));
                        int j = 0;
                        for(window &win : arr) {
                            win.y1 += moved[j];
                            win.y2 += moved[j];
                            j++;
                        }
                    }
                    break;
                }
                i++;
            }
            if(!found) {
                cout << "Command " << index << ": MOVE - no window at given position\n";
            }
        } else {//resize
            cin >> x >> y >> dx >> dy;
            bool found = false;
            int i = 0;
            window temp, erased;
            for(window &w : arr) {
                if(w.inside(x,y)) {
                    found = true;
                    temp = window(w.x1,w.y1,w.x1+dx-1, w.y1+dy-1, w.pos);
                    erased = w;
                    arr.erase(arr.begin() + i);
                    break;
                }
                i++;
            }
            if(!found) {
                cout << "Command " << index << ": RESIZE - no window at given position\n";
                index++;
                continue;
            }
            bool canInsert = true;
            if(temp.x2 > xMax || temp.y2 > yMax) {
                canInsert = false;
            }
            if(canInsert) {
                for(window &w : arr) {
                    if(w.intersect(temp)) {
                        canInsert = false;
                        break;
                    }
                }
            }
            if(!canInsert) {
                cout << "Command " << index << ": RESIZE - window does not fit\n";
                arr.push_back(erased);
            } else {
                arr.push_back(temp);
            }
        }
        index++;
    }
    sort(arr.begin(), arr.end(), cmpPos);
    cout << arr.size() << " window(s):\n";
    for(window &w : arr) {
        cout << w.x1 << ' ' << w.y1 << ' ' << w.x2-w.x1+1 << ' ' << w.y2-w.y1+1 << '\n';
    }
    return 0;
}























