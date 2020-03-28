#include <vector>
#include <iostream>
#include <climits>
#include <tuple>
#include <random>
#include <time.h>

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

void updateUsedMap(bMatrix & map, Project * p, int row, int col, bool used) {
    const vector<vector<char>> & plan = p->getPlan();
    for (size_t r = 0; r < plan.size(); r++) {
        for (size_t c = 0; c < plan[0].size(); c++) {
            if (plan[r][c] == '#') {
                map[row+r][col+c] = used;
            }
        }
    }
}

State hillClimbing(InputInfo * info) { // order buildings by occupied size / value rating ??

    cout << "[+] Starting hill climbing" << endl;

    cout << "[+] Generating initial state" << endl;
    State currentState = generateState(info);
    int previousValue, currentValue;
    previousValue = currentValue = currentState.value();

    cout << "[+] Starting Hill Climbing with value: " << currentValue << endl << endl;
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
            break;
        }

        cout << "[+] Found neighbour: " << currentValue << endl << endl;
        currentState = neighbour;
        previousValue = currentValue;
    }
    
    return currentState;
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
    int D = initialState.getGlobalInfo()->maxWalkDist;
    const vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
    bMatrix map = initialState.getFilledPositions();

    State state = initialState;
    int initialValue = state.value();

    Project * bProject;
    uint bEmptyCount = state.emptyCount();
    int bRow, bCol, bValue = initialValue;

    int minRow = max(0, initialState.getMinRow()-D);
    int maxRow = min((int)map.size()-1, initialState.getMaxRow() + D);
    int minCol = max(0, initialState.getMinCol()-D);
    int maxCol = min((int)map[0].size()-1, initialState.getMaxCol() + D);

    for(int row = minRow; row <= maxRow; row++){
        for(int col = minCol; col <= maxCol; col++){

            // check if empty
            if(map[row][col])
                continue;
            
            // try to build all projects
            for(size_t p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(state.canCreateBuilding(currProject, row, col, &map)){ // x = col, y = row

                    // create building and check its value
                    uint newBuildingID = state.createBuilding(currProject, row, col, false); // do not update map limits in this step
                    int newStateValue = state.value();   

                    if(betterState(bValue, bEmptyCount, newStateValue, state.emptyCount())) {
                        // if only want a better solution return immediately
                        if(!findBest) {
                            state.updateMapLimitsCreate(currProject, row, col);
                            return state;
                        }

                        // assign variables
                        bProject = currProject;
                        bRow = row;
                        bCol = col;
                        bValue = newStateValue;
                        bEmptyCount = state.emptyCount();
                    }

                    // remove building to maintain state
                    // must pass map without the new building to add
                    state.removeBuilding(newBuildingID, false);
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

State removeBuildingOperator(const State & initialState, bool findBest){ // TODO do not copy states, instead apply change and then the reverse at the end
    State state = initialState;
    int initialValue = state.value();

    uint bestToRemove = 0;
    int bValue = initialValue, bEmptyCount = state.emptyCount();

    vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();
    for (uint b : buildingsIDs) {

        Building removed = state.removeBuilding(b, false);
        int newValue = state.value();
        if(betterState(bValue, bEmptyCount, newValue, state.emptyCount())) {
            // only want one better solution
            if(!findBest) {
                state.updateMapLimitsRemove(removed);
                return state;
            }
            
            // update best solution so far
            bestToRemove = b;
            bValue = newValue;
            bEmptyCount = state.emptyCount();
        }

        state.createBuilding(removed.getProject(), removed.getRow(), removed.getCol(), false);
    }

    // reached a better solution
    if(betterState(initialValue, state.emptyCount(), bValue, bEmptyCount)) {
        state.removeBuilding(bestToRemove);
        return state;
    }

    return initialState;
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
    
    int v1 = new_s1.value();
    int v2 = new_s2.value();
    if(v1 > v2)
        return new_s1;
    if(v2 > v1)
        return new_s2;

    if(new_s1.emptyCount() >= new_s2.emptyCount())
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

        if(b.getRow() < (int)mid_row){
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

State generateState(InputInfo *globalInfo){
    vector<Project> &projs = globalInfo->bProjects;

    State s(globalInfo);

    bMatrix used(globalInfo->rows, vector<bool>(globalInfo->cols, false));
    int col_inc = 1;
    for(int row = 0; row < globalInfo->rows; row += 1){
        for(int col = 0; col < globalInfo->cols; col += col_inc){

            if(used[row][col]) continue;
            col_inc = 1;

            Project & p = projs[rand() % projs.size()];

            if(s.canCreateBuilding(&p, row, col, &used)){
                s.createBuilding(&p, row, col);
                updateUsedMap(used, &p, row, col, true);
                col_inc = p.getPlan()[0].size();
            }
        }
    }

    return s;
}

vector<State> generatePopulation(InputInfo *global_info, int populationSize){
    vector<State> population(populationSize);

    for(int i = 0; i < populationSize; i++){
        population[i] = generateState(global_info);
    }

    return population;
}