#include <vector>
#include <climits>
#include <algorithm>

#include "Building.h"

using namespace std;

int buildingsDist(const Building & b1, const Building & b2) {
    int b1X = b1.getX(), b1Y = b1.getY(), b2X = b2.getX(), b2Y = b2.getY();
    const vector<vector<char>> & b1Plan = b1.getProject()->getPlan();
    const vector<vector<char>> & b2Plan = b2.getProject()->getPlan();

    int dist = INT_MAX;
    for (size_t row1 = b1Y; row1 < b1Y + b1Plan.size(); row1++)
        for (size_t col1 = b1X; col1 < b1X + b1Plan[0].size(); col1++)
            for (size_t row2 = b2Y; row2 < b2Y + b2Plan.size(); row2++)
                for (size_t col2 = b2X; col2 < b2X + b2Plan[0].size(); col2++)
                    dist = min(dist, manhattanDistance(row1,col1,row2,col2));
    return dist;
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}