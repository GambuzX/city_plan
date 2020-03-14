#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "State.h"
#include "InputParse.h"

using namespace std;

int main() {

    vector<Plan> bPlans;

    InputInfo globalInfo = parseInput("inputs/example.txt", bPlans);
    State initialState = State(globalInfo.rows, globalInfo.cols);
    
    return 0;
}

int buildingsDist(const Building & b1, const Building & b2) {
    return 0;
}

int value(const State & state, vector<int> utilityTypes, int D) {
    int points = 0;
    const vector<Building> & buildings = state.getBuildings();
    const vector<int> & resBuildings = state.getResidentialBuildings();
    const vector<int> & utilBuildings = state.getUtilityBuildings();

    // for all residential buildings
    for (int rIndex : resBuildings) { 
        const Building & resBuilding = buildings[rIndex];

        // check all utilities that exist
        for (int utilityType : utilityTypes) {

            // for all utility buildings
            for (int uIndex : utilBuildings) {
                const Building & utilBuilding = buildings[uIndex];
                
                // check if type matches and if is in range D
                if (utilBuilding.getPlan()->getType() == utilityType &&
                    buildingsDist(resBuilding, utilBuilding) <= D) {

                    // utility type exists, add points
                    points += resBuilding.getPlan()->getValue();
                    break;
                }
            }
        }
    }

    return points;
}