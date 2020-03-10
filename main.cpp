#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "State.h"
#include "InputParse.h"

using namespace std;

int manhattanDistance(Plan b1, Plan b2);
InputInfo parseInput(string filename, vector<Plan>&b);
void assignBuildingsIDs(vector<Plan>&b, unordered_map<int,Plan**>& h);

int main() {

    vector<Plan> bPlans;

    InputInfo globalInfo = parseInput("inputs/example.txt", bPlans);
    State initialState = State(globalInfo.rows, globalInfo.cols);
    
    return 0;
}

int manhattanDistance(Plan b1, Plan b2) {
    return 0;
}