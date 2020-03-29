#include <vector>
#include <climits>
#include <algorithm>
#include <cmath>

#include "Building.h"

using namespace std;

int buildingsDist(const Building & b1, const Building & b2) {
    int b1X = b1.getCol(), b1Y = b1.getRow(), b2X = b2.getCol(), b2Y = b2.getRow();
    const vector<vector<char>> & b1Plan = b1.getProject()->getPlan();
    const vector<vector<char>> & b2Plan = b2.getProject()->getPlan();

    int dist = INT_MAX;
    for (size_t row1 = b1Y; row1 < b1Y + b1Plan.size(); row1++) {
        for (size_t col1 = b1X; col1 < b1X + b1Plan[0].size(); col1++) {
            if(b1Plan[row1-b1Y][col1-b1X] == '.') continue;
            for (size_t row2 = b2Y; row2 < b2Y + b2Plan.size(); row2++) {
                for (size_t col2 = b2X; col2 < b2X + b2Plan[0].size(); col2++) {
                    if(b2Plan[row2-b2Y][col2-b2X] == '.') continue;
                    dist = min(dist, manhattanDistance(row1,col1,row2,col2));
                }
            }
        }
    }
    return dist;
}

bool buildingsDistLessThanD(const Building & b1, const Building & b2, const int &D){
    int b1X = b1.getCol(), b1Y = b1.getRow(), b2X = b2.getCol(), b2Y = b2.getRow();

    const vector<vector<char>> & b1Plan = b1.getProject()->getPlan();
    const vector<vector<char>> & b2Plan = b2.getProject()->getPlan();
    
    for (size_t row1 = b1Y; row1 < b1Y + b1Plan.size(); row1++){
        for (size_t col1 = b1X; col1 < b1X + b1Plan[0].size(); col1++){
            if(b1Plan[row1-b1Y][col1-b1X] == '.') continue;
            for (size_t row2 = b2Y; row2 < b2Y + b2Plan.size(); row2++){
                if(abs((int)row2 - (int)row1) > D) continue;
                for (size_t col2 = b2X; col2 < b2X + b2Plan[0].size(); col2++){
                    if(abs((int)col2 - (int)col1) > D) continue;
                    if(b2Plan[row2-b2Y][col2-b2X] == '.') continue;
                    if(manhattanDistance(row1,col1,row2,col2) <= D) return true;
                }
            }
        }
    }

    return false;
}

int manhattanDistance(int row1, int col1, int row2, int col2) {
    int s1 = row1 > row2 ? row1 - row2 : row2 - row1;
    int s2 = col1 > col2 ? col1 - col2 : col2 - col1;
    return s1+s2;
}

bool Building::aboveRow(int separation) const {
    return row + (int)proj->getPlan().size() <= separation;
}

bool Building::belowRow(int separation) const {
    return row > separation;
}