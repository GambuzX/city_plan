#include <vector>
#include <iostream>
#include <climits>

#include "IIAlgorithms.h"

using namespace std;

State randomStart(const State & initialState);
State addBuildingOperator(const State & initialState, bool findBest);
State removeBuildingOperator(const State & initialState, bool findBest);
State replaceBuildingOperator(const State & initialState, bool findBest);

bool betterState(const State & s1, const State & s2) {
    int s1Val = s1.value();
    int s2Val = s2.value();
    return s2Val > s1Val || (s2Val == s1Val && s2.emptyCount() < s1.emptyCount());
}

bool betterState(int pValue, int pEmptyCells, int nValue, int nEmptyCells) {
    return nValue > pValue || (nValue == pValue && nEmptyCells < pEmptyCells);
}

State hillClimbing(const State & initialState) { // order buildings by occupied size / value rating ??

    State currentState = randomStart(initialState); // random first choice, cuz no points. check both values at different steps
    int previousValue, currentValue;
    previousValue = currentValue = currentState.value();

    cout << "[+] Starting hill climbing with value " << currentValue << endl;
    while(1) {
        State neighbour = higherValueNeighbour(currentState, false);
        currentValue = neighbour.value();
        cout << "[+] Found neighbour: " << currentValue << endl;

        if (currentValue <= previousValue) {     
            cout << "[+] Reached local maximum" << endl;
            break;
        }
        currentState = neighbour;
        previousValue = currentValue;
    }
    
    return currentState;
}

State randomStart(const State & initialState) {
    vector<Project> & projs = initialState.getGlobalInfo()->bProjects;
    State newState = initialState;
    newState.createBuilding(&projs[0], 0, 0);
    return newState;
}


State higherValueNeighbour(const State & state, bool findBest){
    State * bestState = (State*) &state;
    int bestValue = state.value();

    // Add building. First because its the one who can score more points
    State addState = addBuildingOperator(state, findBest);
    int addStateValue = addState.value();
    if(betterState(bestValue, bestState->emptyCount(), addStateValue, addState.emptyCount())) {
        if(!findBest) return addState;
        bestState = &addState;
        bestValue = addStateValue;
    }

    // Replace building. Second because can still increase points
    State replaceState = replaceBuildingOperator(state, findBest);
    int replaceStateValue = replaceState.value();
    if(betterState(bestValue, bestState->emptyCount(), replaceStateValue, replaceState.emptyCount())) {
        if(!findBest) return replaceState;
        bestState = &replaceState;
        bestValue = replaceStateValue;
    }

    // Remove building. Can only improve by having same value and less occupied cells
    State removeState = removeBuildingOperator(state, findBest);
    int removeStateValue = removeState.value();
    if(betterState(bestValue, bestState->emptyCount(), removeStateValue, removeState.emptyCount())) {
        if(!findBest) return removeState;
        bestState = &removeState;
        bestValue = removeStateValue;
    }

    return *bestState;
}

State addBuildingOperator(const State & initialState, bool findBest){
    const vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
    const vector<vector<uint>> & map = initialState.getCityMap();

    State bestState = initialState;
    int bestValue = initialState.value();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(size_t p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(initialState.canCreateBuilding(currProject, col, row)){ // x = col, y = row

                    // create building and check its value
                    State newState = initialState;
                    newState.createBuilding(currProject, col, row);
                    int newStateValue = newState.value();
                    
                    if(betterState(bestValue, bestState.emptyCount(), newStateValue, newState.emptyCount())) {
                        
                        // if only want a better solution return immediatelly
                        if(!findBest) return newState;

                        bestState = newState;
                        bestValue = newStateValue;
                    }

                }
            }
        }
    }
    return bestState;
}

State removeBuildingOperator(const State & initialState, bool findBest){
    State bestState = initialState;
    int bestValue = initialState.value();

    vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();
    for (int b : buildingsIDs) {
        State newState = initialState;
        newState.removeBuilding(b);
        int newStateValue = newState.value();

        if(betterState(bestValue, bestState.emptyCount(), newStateValue, newState.emptyCount())) {
            if(!findBest) return newState;

            bestState = newState;
            bestValue = newStateValue;
        }

    }
    return bestState;
}

State replaceBuildingOperator(const State & initialState, bool findBest){
    const unordered_map<uint, Building> & buildings = initialState.getBuildings();
    const vector<Project> & projects = initialState.getGlobalInfo()->bProjects;

    State bestState = initialState;
    int bestValue = initialState.value();

    vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();
    for (int b : buildingsIDs) {
        unordered_map<uint, Building>::const_iterator it = buildings.find(b);
        if(it == buildings.end()) continue;

        // remove building so that it can be replaced
        State newStateRemove = initialState;
        newStateRemove.removeBuilding(b);

        // try to replace removed building with all projects
        for (size_t p = 0; p < projects.size(); p++) {
            Project * proj = (Project*) &projects[p];
            if(newStateRemove.canCreateBuilding(proj, it->second.getX(), it->second.getY())) {
                State newStateReplace = newStateRemove;
                newStateReplace.createBuilding(proj, it->second.getX(), it->second.getY());
                int newStateValue = newStateReplace.value();

                if(betterState(bestValue, bestState.emptyCount(), newStateValue, newStateReplace.emptyCount())) {
                    if(!findBest) return newStateReplace;

                    bestState = newStateReplace;
                    bestValue = newStateValue;
                }

            }
        }
    }
    return bestState;
}