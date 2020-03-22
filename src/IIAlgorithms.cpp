#include <vector>
#include <iostream>
#include <climits>

#include "IIAlgorithms.h"

using namespace std;

bool betterState(int pValue, int pEmptyCells, int nValue, int nEmptyCells) {
    return nValue > pValue || (nValue == pValue && nEmptyCells < pEmptyCells);
}

State hillClimbing(const State & initialState) { // order buildings by occupied size / value rating ??

    State currentState = initialState; // random first choice, cuz no points. check both values at different steps
    int previousValue, currentValue;
    previousValue = currentValue = currentState.value();

    cout << "[+] Starting hill climbing with value " << currentValue << endl;
    while(1) {
        State neighbour = higherValueNeighbour(currentState);
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


State higherValueNeighbour(const State & state){ //TODO consider removing and changing buildings
    const vector<Project> & projects = state.getGlobalInfo()->bProjects;
    const vector<vector<uint>> & map = state.getCityMap();
    int initialStateValue = state.value();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(size_t p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(state.canCreateBuilding(currProject, col, row)){ // x = col, y = row

                    // create building and check its value
                    State newState = state;
                    newState.createBuilding(currProject, col, row);
                    int newStateValue = newState.value();
                    
                    // found better state
                    if(newStateValue > initialStateValue || 
                        (newStateValue == initialStateValue && newState.emptyCount() < state.emptyCount())){
                        return newState;
                    }

                }
            }
        }
    }

    // did not find better neighbour
    return state;
}

State addBuildingOperator(const State & initialState, bool findBest){ //TODO consider removing and changing buildings
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

State highestValueNeighbor(const State & state){
    const vector<Project> & projects = state.getGlobalInfo()->bProjects;
    const vector<vector<uint>> & map = state.getCityMap();
    State bestState = state;
    int bestValue = state.value();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(size_t p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(state.canCreateBuilding(currProject, col, row)){

                    // create building and check its value
                    State newState = state;
                    newState.createBuilding(currProject, col, row);
                    int newStateValue = newState.value();
                    
                    if(newStateValue > bestValue || 
                        (newStateValue == bestValue && newState.emptyCount() < bestState.emptyCount())){
                        bestState = newState;
                        bestValue = newStateValue;
                    }

                }
            }
        }
    }

    return bestState;
}