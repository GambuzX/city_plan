#include <vector>
#include <iostream>
#include <climits>
#include <tuple>

#include "IIAlgorithms.h"
#include "util.h"

using namespace std;

State randomStart(const State & initialState);
State addBuildingOperator(const State & initialState, bool findBest);
State removeBuildingOperator(const State & initialState, bool findBest);
State replaceBuildingOperator(const State & initialState, bool findBest);

bool betterState(const State & s1, const State & s2) {
    int s1Val = s1.value();
    int s2Val = s2.value();
    return s2Val > s1Val || (s2Val == s1Val && s2.emptyCount() > s1.emptyCount());
}

bool betterState(int pValue, int pEmptyCells, int nValue, int nEmptyCells) {
    return nValue > pValue || (nValue == pValue && nEmptyCells > pEmptyCells);
}

// TODO only consider inserting in positions < D
// TODO do not copy states, instead apply change and then the reverse at the end

State hillClimbing(const State & initialState) { // order buildings by occupied size / value rating ??

    cout << "[+] Starting hill climbing" << endl;
    //initialState.printMap();

    cout << "[+] Choosing first building" << endl << endl;
    State currentState = randomStart(initialState); // random first choice, cuz no points. check both values at different steps
    //currentState.printMap();
    int previousValue, currentValue;
    previousValue = currentValue = currentState.value();

    while(1) {
            
        cout << "[+] Searching for neighbour" << endl;
        State neighbour = higherValueNeighbour(currentState, false);
        currentValue = neighbour.value();

        if (currentValue <= previousValue) {     
            if(currentValue == previousValue && currentState.addRandomBuilding()) {
                cout << "[+] Added a random building" << endl << endl;
                previousValue = currentState.value();
                continue;
            }
            cout << "[+] Reached local maximum: " << currentValue << endl;
            //neighbour.printMap();
            break;
        }

        cout << "[+] Found neighbour: " << currentValue << endl << endl;
        //neighbour.printMap();
        currentState = neighbour;
        previousValue = currentValue;
    }
    
    return currentState;
}

State randomStart(const State & initialState) { // TODO not random. should it be?
    vector<Project> & projs = initialState.getGlobalInfo()->bProjects;
    State newState = initialState;
    newState.createBuilding(&projs[0], 0, 0);
    return newState;
}


State higherValueNeighbour(const State & state, bool findBest){
    State * bestState = (State*) &state;
    int bestValue = state.value();

    // Add building. First because its the one who can score more points
    cout << "[!] Applying ADD operator" << endl;
    State addState = addBuildingOperator(state, findBest);
    int addStateValue = addState.value();
    if(betterState(bestValue, bestState->emptyCount(), addStateValue, addState.emptyCount())) {
        cout << "[!] Found better state by building, value: " << addStateValue << endl;
        if(!findBest) return addState;
        bestState = &addState;
        bestValue = addStateValue;
    }

    // Replace building. Second because can still increase points
    cout << "[!] Applying REPLACE operator" << endl;
    State replaceState = replaceBuildingOperator(state, findBest);
    int replaceStateValue = replaceState.value();
    if(betterState(bestValue, bestState->emptyCount(), replaceStateValue, replaceState.emptyCount())) {
        cout << "[!] Found better state by replacing, value: " << replaceStateValue << endl;
        if(!findBest) return replaceState;
        bestState = &replaceState;
        bestValue = replaceStateValue;
    }

    // Remove building. Can only improve by having same value and less occupied cells
    cout << "[!] Applying REMOVE operator" << endl;
    State removeState = removeBuildingOperator(state, findBest);
    int removeStateValue = removeState.value();
    if(betterState(bestValue, bestState->emptyCount(), removeStateValue, removeState.emptyCount())) {
        cout << "[!] Found better state by removing, value: " << removeStateValue << endl;
        if(!findBest) return removeState;
        bestState = &removeState;
        bestValue = removeStateValue;
    }

    return *bestState;
}

State addBuildingOperator(const State & initialState, bool findBest = false){
    const vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
    const vector<vector<uint>> & map = initialState.getCityMap();

    State state = initialState;
    int initialValue = state.value();

    Project * bProject;
    uint bEmptyCount = state.emptyCount();
    int bRow, bCol, bValue = initialValue;

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(size_t p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(state.canCreateBuilding(currProject, row, col)){ // x = col, y = row

                    // create building and check its value
                    uint newBuildingID = state.createBuilding(currProject, row, col);

                    int newStateValue = state.value();                    
                    if(betterState(bValue, bEmptyCount, newStateValue, state.emptyCount())) {

                        // if only want a better solution return immediatelly
                        if(!findBest) return state;

                        // assign variables
                        bProject = currProject;
                        bRow = row;
                        bCol = col;
                        bValue = newStateValue;
                        bEmptyCount = state.emptyCount();
                    }

                    // remove building to maintain state
                    state.removeBuilding(newBuildingID);
                } 
            }
        }
    }
    // reached a better solution
    if(betterState(initialValue, state.emptyCount(), bValue, bEmptyCount)) {
        state.createBuilding(bProject, bRow, bCol);
        return state;
    }

    return initialState;
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
            if(it->second.getProject()->getID() == proj->getID()) continue;

            if(newStateRemove.canCreateBuilding(proj, it->second.getRow(), it->second.getCol())) {
                State newStateReplace = newStateRemove;
                newStateReplace.createBuilding(proj, it->second.getRow(), it->second.getCol());
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

State breeding(const State &s1, const State &s2){
    uint num_rows = s1.getGlobalInfo()->rows;
    uint mid_row = num_rows / 2;
    uint num_cols = s1.getGlobalInfo()->cols;

    tuple<vector<vector<uint>>, vector<vector<uint>>, uint> s1_results = divideState(s1, num_rows, mid_row, num_cols);
    tuple<vector<vector<uint>>, vector<vector<uint>>, uint> s2_results = divideState(s2, num_rows, mid_row, num_cols);
    
    State new_s1 = State(get<0>(s1_results), s1.getBuildings(), get<2>(s1_results), get<1>(s2_results), s2.getBuildings(), s1.getGlobalInfo());
    State new_s2 = State(get<0>(s2_results), s2.getBuildings(), get<2>(s2_results), get<1>(s1_results), s1.getBuildings(), s2.getGlobalInfo());
    
    if(new_s1.value() >= new_s2.value())
        return new_s1;
    
    return new_s2;
}

tuple<vector<vector<uint>>, vector<vector<uint>>, uint> divideState(const State &s, const uint &num_rows, const uint &mid_row, const uint &num_cols){
    vector<uint> buildingIDs = s.getAllBuildingsIDs();
    unordered_map<uint, Building> buildings = s.getBuildings();

    vector<vector<uint>> top = vector<vector<uint>>(mid_row, vector<uint>(num_cols, 0));
    vector<vector<uint>> bottom = vector<vector<uint>>(num_rows-mid_row, vector<uint>(num_cols, 0));

    uint top_max_id = 0;

    for(size_t i = 0; i < buildingIDs.size(); i++){
        Building b = buildings.at(buildingIDs[i]);

        if(b.getRow() < mid_row){
            if(b.getRow() + b.getProject()->getPlan().size() <= mid_row){
                for(size_t j = 0; j < b.getProject()->getPlan().size(); j++){
                    for(size_t k = 0; k < b.getProject()->getPlan()[0].size(); k++){
                        top[b.getRow() + j][b.getCol() + k] = b.getProject()->getID();
                    }
                }
                if(buildingIDs[i] > top_max_id)
                    top_max_id = buildingIDs[i];
            }else
                continue;
            
        }else{
            for(size_t j = 0; j < b.getProject()->getPlan().size(); j++){
                for(size_t k = 0; k < b.getProject()->getPlan()[0].size(); k++){
                    bottom[b.getRow() + j - mid_row][b.getCol() + k] = b.getProject()->getID();
                }
            }
        }
    } 

    return make_tuple(top, bottom, top_max_id);
}