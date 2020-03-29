#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "State.h"
#include "util.h"

using namespace std;

State::State(InputInfo * globalInfo) {
    int rows = globalInfo->rows;
    int cols = globalInfo->cols;
    this->globalInfo = globalInfo;
    nextID = 1;
    emptyCells = rows*cols;
}

State::State(const State &s){
    this->nextID = s.getNextID();
    this->emptyCells = s.emptyCount();
    this->globalInfo = s.getGlobalInfo();
    this->buildings = s.getBuildings();
    this->residentialBuildings = s.getResidentialBuildings();
    this->utilityBuildings = s.getUtilityBuildings();
    this->minRow = s.getMinRow();
    this->maxRow = s.getMaxRow();
    this->minCol = s.getMinCol();
    this->maxCol = s.getMaxCol();
}

bool State::canCreateBuilding(Project * proj, int row, int col) const {
    const vector<vector<char>> & plan = proj->getPlan();
    bMatrix cityMap = getFilledPositions();

    if (row < 0 || col < 0) return false;   
    if (row + (int)plan.size() > globalInfo->rows) return false;
    if (col + (int)plan[0].size() > globalInfo->cols) return false;

    for (size_t prow = 0; prow < plan.size(); prow++) {
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++) {
            if (plan[prow][pcol] == '#' && cityMap[row+prow][col+pcol] != 0) {
                return false;
            }
        }
    }
    
    return true;
}

bool State::canCreateBuilding(Project * proj, int row, int col, bMatrix * filledPos) const {
    //clock_t limitsStart = clock();

    const vector<vector<char>> & plan = proj->getPlan();

    if (row < 0 || col < 0) return false;   
    if (row + (int)plan.size() > globalInfo->rows) return false;
    if (col + (int)plan[0].size() > globalInfo->cols) return false;

    for (size_t prow = 0; prow < plan.size(); prow++) {
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++) {
            if (plan[prow][pcol] == '#' && (*filledPos)[row+prow][col+pcol] != 0) {
                return false;
            }
        }
    }


    //clock_t limitsEnd = clock();
    //double el2 = double(limitsEnd - limitsStart) / CLOCKS_PER_SEC;
    //cout << "Compare took " << el2 << endl;
    
    return true;
}

uint State::createBuilding(Project * proj, int row, int col, bool updateLimits) {
    uint ID = nextID++;
    const vector<vector<char>> & plan = proj->getPlan();

    for (size_t prow = 0; prow < plan.size(); prow++)
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++)
            if (plan[prow][pcol] == '#')
                emptyCells--;

    if (proj->getType() == BuildingType::residencial) residentialBuildings.push_back(ID);
    if (proj->getType() == BuildingType::utility) utilityBuildings.push_back(ID);

    if(updateLimits) updateMapLimitsCreate(proj, row, col);
    buildings.insert(make_pair(ID, Building(proj, row, col)));
    return ID;
}

void State::updateMapLimitsCreate(Project * proj, int row, int col) {
    const vector<vector<char>> & plan = proj->getPlan();
    if(buildings.size() == 0) {
        minRow = row;
        maxRow = (uint) (row + plan.size() - 1);
        minCol = col;
        maxCol = (uint) (col + plan[0].size() - 1);
    }
    else {
        minRow = min(minRow, row);
        maxRow = max(maxRow, row + (int) plan.size() - 1);
        minCol = min(minCol, col);
        maxCol = max(maxCol, col + (int) plan[0].size() - 1);
    }
}

Building State::removeBuilding(uint id, bool updateLimits) {
    unordered_map<uint, Building>::iterator it = buildings.find(id);
    if (it == buildings.end()) return Building();

    Building b = it->second;
    const vector<vector<char>> & plan = b.getProject()->getPlan();

    // update empty cells
    for (size_t prow = 0; prow < plan.size(); prow++)
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++)
            if (plan[prow][pcol] == '#')
                emptyCells++;

    if(it->second.getProject()->getType() == BuildingType::residencial)
        residentialBuildings.erase(remove(residentialBuildings.begin(), residentialBuildings.end(), id), residentialBuildings.end()); 

    if(it->second.getProject()->getType() == BuildingType::utility)
        utilityBuildings.erase(remove(utilityBuildings.begin(), utilityBuildings.end(), id), utilityBuildings.end());

    if(updateLimits) updateMapLimitsRemove(b);
    buildings.erase(it);
    return b;
}

void State::updateMapLimitsRemove(const Building & removed) {

    int sRow, eRow, sCol, eCol;
    const vector<vector<char>> & plan = removed.getProject()->getPlan();
    sRow = removed.getRow();
    eRow = sRow + plan.size();
    sCol = removed.getCol();
    eCol = sCol + plan[0].size();

    const bMatrix & cityMap = getFilledPositions();

    if (sCol == minCol) {
        for(int col = minCol; col <= maxCol; col++) {
            bool bExists = false;
            for(int row = minRow; row <= maxRow; row++) {
                if (cityMap[row][col]) {
                    bExists = true;
                    break;
                }
            }
            if(bExists) break;

            // no building exists in this column
            minCol++;
        }
    }

    if(eCol == maxCol) {
        for(int col = maxCol; col >= minCol; col--) {
            bool bExists = false;
            for(int row = minRow; row <= maxRow; row++) {
                if (cityMap[row][col]) {
                    bExists = true;
                    break;
                }
            }
            if(bExists) break;

            // no building exists in this column
            maxCol--;
        }
    }

    if (sRow == minRow) {
        for(int row = minRow; row <= maxRow; row++) {
            bool bExists = false;
            for(int col = minCol; col <= maxCol; col++) {
                if (cityMap[row][col]) {
                    bExists = true;
                    break;
                }
            }
            if(bExists) break;

            // no building exists in this column
            minRow++;
        }
    }

    if (eRow == maxRow) {
        for(int row = maxRow; row >= minRow; row--) {
            bool bExists = false;
            for(int col = minCol; col <= maxCol; col++) {
                if (cityMap[row][col]) {
                    bExists = true;
                    break;
                }
            }
            if(bExists) break;

            // no building exists in this column
            maxRow--;
        }
    }

}

int State::value() const { //TODO calculate value when a building is adedd;
    const int D = globalInfo->maxWalkDist;
    
    int points = 0;

    // for all residential buildings
    for (int rIndex : residentialBuildings) { 
        const Building & resBuilding = buildings.find(rIndex)->second;
        unordered_set<BuildingType> seen;
        for (int uIndex : utilityBuildings) {
            const Building & utilBuilding = buildings.find(uIndex)->second;
            BuildingType t = utilBuilding.getProject()->getType();

            if(seen.find(t) == seen.end() && buildingsDistLessThanD(resBuilding, utilBuilding, D)) {
                seen.insert(t);
                points += resBuilding.getProject()->getValue();                
            }
        }
    }
    return points;
}


list<uint> State::getAllBuildingsIDs() const {
    list<uint> allIDs = residentialBuildings;
    allIDs.insert(allIDs.end(), utilityBuildings.begin(), utilityBuildings.end());
    return allIDs;
}

void State::printMap() const {
    const vector<vector<uint>> & m = getCityMap();
    for (const vector<uint> & row : m) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool State::addRandomBuilding() {
    vector<Project> & projs = getGlobalInfo()->bProjects;
    const bMatrix & map = getFilledPositions();

    for(int row = 0; row < globalInfo->rows; row++){
        for(int col = 0; col < globalInfo->cols; col++){
            // already occupied
            if(map[row][col])
                continue;

            // find some project to add
            for (size_t i = 0; i < projs.size(); i++) {
                if(canCreateBuilding(&projs[i], row, col)) {
                    createBuilding(&projs[i], row, col);
                    return true;
                }
            }
        }
    }    

    // couldnt add anything
    return false;
}

bool State::isPositionNearBuildings(int row, int col) const {
    if(buildings.size() == 0) return true;
    int D = getGlobalInfo()->maxWalkDist;

    // careful with unsigned int and underflows
    return minRow-D <= row && row <= maxRow+D && 
           minCol-D <= col && col <= maxCol+D;
}

vector<vector<uint>> State::getCityMap() const {
    vector<vector<uint>> map (globalInfo->rows, vector<uint>(globalInfo->cols, 0));
    for(uint b : residentialBuildings) {
        unordered_map<uint, Building>::const_iterator it = buildings.find(b);
        if(it == buildings.end()) continue;

        const vector<vector<char>> & plan = it->second.getProject()->getPlan();
        for (size_t r = 0; r < plan.size(); r++) {
            for (size_t c = 0; c < plan[0].size(); c++) {
                if(plan[r][c] == '#')
                    map[it->second.getRow()+r][it->second.getCol()+c] = b;
            }
        }

    }

    for(uint b : utilityBuildings) {
        unordered_map<uint, Building>::const_iterator it = buildings.find(b);
        if(it == buildings.end()) continue;

        const vector<vector<char>> & plan = it->second.getProject()->getPlan();
        for (size_t r = 0; r < plan.size(); r++) {
            for (size_t c = 0; c < plan[0].size(); c++) {
                if(plan[r][c] == '#')
                    map[it->second.getRow()+r][it->second.getCol()+c] = b;
            }
        }

    }
    return map;
}

bMatrix State::getFilledPositions() const {
    bMatrix map (globalInfo->rows, vector<bool>(globalInfo->cols, false));
    for(uint b : residentialBuildings) {
        unordered_map<uint, Building>::const_iterator it = buildings.find(b);
        if(it == buildings.end()) continue;

        const vector<vector<char>> & plan = it->second.getProject()->getPlan();
        for (size_t r = 0; r < plan.size(); r++) {
            for (size_t c = 0; c < plan[0].size(); c++) {
                if(plan[r][c] == '#')
                    map[it->second.getRow()+r][it->second.getCol()+c] = true;
            }
        }

    }

    for(uint b : utilityBuildings) {
        unordered_map<uint, Building>::const_iterator it = buildings.find(b);
        if(it == buildings.end()) continue;

        const vector<vector<char>> & plan = it->second.getProject()->getPlan();
        for (size_t r = 0; r < plan.size(); r++) {
            for (size_t c = 0; c < plan[0].size(); c++) {
                if(plan[r][c] == '#')
                    map[it->second.getRow()+r][it->second.getCol()+c] = true;
            }
        }

    }
    return map;
}

bool State::betterState(const State & s1, const State & s2) {
    int s1Val = s1.value();
    int s2Val = s2.value();
    return s2Val > s1Val || (s2Val == s1Val && s2.emptyCount() > s1.emptyCount());
}

bool State::betterState(int pValue, int pEmptyCells, int nValue, int nEmptyCells) {
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

State * generatePopulationPtr(InputInfo *global_info, int populationSize){
    State * population = new State[populationSize];

    for(int i = 0; i < populationSize; i++){
        population[i] = generateState(global_info);
    }

    return population;
}