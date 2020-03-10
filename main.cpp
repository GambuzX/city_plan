#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "State.h"
#include "InputParse.h"

using namespace std;

int manhattanDistance(Building b1, Building b2);

int main() {

    vector<Plan> bPlans;

    InputInfo globalInfo = parseInput("inputs/example.txt", bPlans);
    State initialState = State(globalInfo.rows, globalInfo.cols);
    
    return 0;
}

int manhattanDistance(Building b1, Building b2) {
    return 0;
}