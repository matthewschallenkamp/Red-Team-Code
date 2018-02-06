#include <bits/stdc++.h>
using namespace std;
const double pi = 4*atanl(1);

//calculates the angle between 2 points in degrees, rounded to nearest int (will be unique)
int calcAngle(pair<int, int> prev, pair<int, int> curr) {
    int angle;
    if(prev.second == curr.second) {
        angle = 90;
        if(prev.first < curr.first) angle += 180;
    } else {
        angle = round(atanl((prev.first-curr.first)/(double)(curr.second-prev.second))*180/pi);
        if(prev.second > curr.second) angle += 180;
    }
    return ((angle%360)+360)%360;
}

bool insideRange(pair<int, int> range, pair<bool, bool> bounds, int angle) {
    if(range.first == range.second) return (angle == range.first);
    if(angle == range.first && bounds.first) return true;
    if(angle == range.second && bounds.second) return true;
    if(range.first < range.second) {
        if(range.first < angle && angle < range.second) return true;
    } else {
        if(range.first < angle || angle < range.second) return true;
    }
    return false;
}

int main() {
    vector<vector<int> > dots(4,vector<int>(4));
    for(auto &row : dots) {
        for(auto &elem : row) {
            cin >> elem;
        }
    }
    int segments = 1;
    //angles in degree rounded to nearest angle, these will be unique
    //angle ranges will always be start angle - end angle
    //if angle range includes angle 0, then start > end
    pair<int, int> range = make_pair(0,359), prevRange;
    pair<bool, bool> inclusive = make_pair(true, true);
    pair<int, int> prev, curr;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            if(dots[i][j] == 1) {
                prev.first = i;
                prev.second = j;
            }
        }
    }
    for(int num = 2; num <= 16; ++num) {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                if(dots[i][j] == num) {
                    curr.first = i;
                    curr.second = j;
                }
            }
        }
        int angle = calcAngle(prev,curr);
        if(insideRange(range, inclusive, angle)) {
            range.first = range.second = angle;
        } else {
            bool finished = false;
            segments++;
            pair<int, int> newRange;
            newRange.first = (range.first+180)%360;
            newRange.second = (range.second+180)%360;
            //update range with previous range
            if(newRange.second <= range.first) {
                if(newRange.second <= angle && angle <= range.first) {
                    range.first = newRange.first;
                    range.second = angle;
                    inclusive = make_pair(false, true);
                    finished = true;
                }
            } else {
                if(newRange.second <= angle || angle <= range.first) {
                    range.first = newRange.first;
                    range.second = angle;
                    inclusive = make_pair(false, true);
                    finished = true;
                }
            }
            if(!finished) {
                if(range.second <= newRange.first) {
                    if(range.second <= angle && angle <= newRange.first) {
                        range.first = angle;
                        range.second = newRange.second;
                        inclusive = make_pair(true,false);
                        finished = true;
                    }
                } else {
                    if(range.second <= angle || angle <= newRange.first) {
                        range.first = angle;
                        range.second = newRange.second;
                        inclusive = make_pair(true,false);
                        finished = true;
                    }
                }
            }
            if(!finished) {
                range = newRange;
                inclusive = make_pair(false, false);
            }
        }
        prev = curr;
        prevRange = range;
    }
    cout << segments << '\n';
    return 0;
}


























