#include <iostream>
#include <vector>
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
    cityMap = std::vector<std::vector<uint>>(rows, std::vector<uint>(cols, 0));
}

State::State(const State &s){
    this->nextID = s.getNextID();
    this->emptyCells = s.emptyCount();
    this->globalInfo = s.getGlobalInfo();
    this->buildings = s.getBuildings();
    this->cityMap = s.getCityMap();
    this->residentialBuildings = s.getResidentialBuildings();
    this->utilityBuildings = s.getUtilityBuildings();
    this->minRow = s.getMinRow();
    this->maxRow = s.getMaxRow();
    this->minCol = s.getMinCol();
    this->maxCol = s.getMaxCol();
}

State::State(std::vector<std::vector<uint>> v1, std::unordered_map<uint, Building> um1, uint max_id1, 
              std::vector<std::vector<uint>> v2, std::unordered_map<uint, Building> um2, InputInfo *globalInfo){
    
    uint nextID = max_id1 + 1;

    uint emptyCells = 0;

    for(size_t i = 0; i < v2.size(); i++){
        for(size_t j = 0; j < v2[i].size(); j++){
            if(v2[i][j] == 0){
                emptyCells++;
                continue;
            }

            v2[i][j] += max_id1;
            
            if(um1.find(v2[i][j]) == um1.end()){
                um1.insert(make_pair(v2[i][j], um2.at(v2[i][j])));
                
                if(v2[i][j] >= nextID)
                    nextID = v2[i][j] + 1;
            }
        }
    }

    for(size_t i = 0; i < v1.size(); i++){
        for(size_t j = 0; j < v1[i].size(); j++){
            if(v1[i][j] == 0){
                emptyCells++;
            }
        }
    }

    vector<vector<uint>> map = concatenate_vectors(v1, v2);

    vector<uint> resid_build, util_build;

    for(auto it = um1.begin(); it != um1.end(); it++){
        auto type = (*it).second.getProject()->getType();
        switch (type)
        {
        case BuildingType::residencial:
            resid_build.push_back((*it).first);
            break;
        case BuildingType::utility:
            util_build.push_back((*it).first);
            break;
        default:
            break;
        }
    }

    this->nextID = max_id1;
    this->emptyCells = emptyCells;
    this->globalInfo = globalInfo;
    this->buildings = um1;
    this->cityMap = map;
    this->residentialBuildings = resid_build;
    this->utilityBuildings = util_build;
}

bool State::canCreateBuilding(Project * proj, int row, int col) const {

    const vector<vector<char>> & plan = proj->getPlan();

    if (row < 0 || col < 0) return false;   
    if (row + plan.size() > cityMap.size()) return false;
    if (col + plan[0].size() > cityMap[0].size()) return false;

    int cityRow, cityCol;
    for (size_t prow = 0; prow < plan.size(); prow++) {
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++) {
            cityRow = row+prow;
            cityCol = col+pcol;
            if (plan[prow][pcol] == '#' && cityMap[cityRow][cityCol] != 0) {
                return false;
            }
        }
    }

    return true;
}

uint State::createBuilding(Project * proj, int row, int col) {
    uint ID = nextID++;
    const vector<vector<char>> & plan = proj->getPlan();

    int cityRow, cityCol;
    for (size_t prow = 0; prow < plan.size(); prow++) {
        for (size_t pcol = 0; pcol < plan[0].size(); pcol++) {
            if (plan[prow][pcol] == '#') {              
                cityRow = row+prow;
                cityCol = col+pcol;
                if (cityMap[cityRow][cityCol] == 0) emptyCells--;
                cityMap[cityRow][cityCol] = ID;
            }
        }
    }

    if (proj->getType() == BuildingType::residencial) residentialBuildings.push_back(ID);
    if (proj->getType() == BuildingType::utility) utilityBuildings.push_back(ID);

    // update map limits
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

    buildings.insert(make_pair(ID, Building(proj, row, col)));
    return ID;
}

void State::removeBuilding(uint id) {
    unordered_map<uint, Building>::iterator it = buildings.find(id);
    if (it == buildings.end()) return;

    const Building & b = it->second;
    const vector<vector<char>> & plan = b.getProject()->getPlan();
    int x = b.getCol(), y = b.getRow();
    int endX=x+plan[0].size()-1, endY=y+plan.size()-1;
    for (int row = y; row <= endY; row++) {
        for (int col = x; col <= endX; col++) {
            if (cityMap[row][col] != 0) emptyCells++;
            cityMap[row][col] = 0;
        }
    }

    if(it->second.getProject()->getType() == BuildingType::residencial)
        residentialBuildings.erase(remove(residentialBuildings.begin(), residentialBuildings.end(), id), residentialBuildings.end()); 

    if(it->second.getProject()->getType() == BuildingType::utility)
        utilityBuildings.erase(remove(utilityBuildings.begin(), utilityBuildings.end(), id), utilityBuildings.end()); 

    updateMapLimits(y, endY, x, endX);
    buildings.erase(it);
}

void State::updateMapLimits(int sRow, int eRow, int sCol, int eCol) {
    if (sCol == minCol) {
        for(int col = minCol; col <= maxCol; col++) {
            bool bExists = false;
            for(int row = minRow; row <= maxRow; row++) {
                if (cityMap[row][col] != 0) {
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
                if (cityMap[row][col] != 0) {
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
                if (cityMap[row][col] != 0) {
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
                if (cityMap[row][col] != 0) {
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

int State::value() const {
    const vector<int> & utilityTypes = globalInfo->allUtilities;
    const int D = globalInfo->maxWalkDist;
    
    int points = 0;

    // for all residential buildings
    for (int rIndex : residentialBuildings) { 
        const Building & resBuilding = buildings.find(rIndex)->second;

        // check all utilities that exist
        for (int utilityType : utilityTypes) {

            // for all utility buildings
            for (int uIndex : utilityBuildings) {
                const Building & utilBuilding = buildings.find(uIndex)->second;
                
                // check if type matches and if is in range D
                if (utilBuilding.getProject()->getType() == utilityType &&
                    buildingsDist(resBuilding, utilBuilding) <= D) {

                    // utility type exists, add points
                    points += resBuilding.getProject()->getValue();
                    break;
                }
            }
        }
    }

    return points;
}


vector<uint> State::getAllBuildingsIDs() const {
    vector<uint> allIDs = residentialBuildings;
    allIDs.insert(allIDs.end(), utilityBuildings.begin(), utilityBuildings.end());
    return allIDs;
}

void State::printMap() const {
    const vector<vector<uint>> & m = cityMap;
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
    const vector<vector<uint>> & map = getCityMap();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // already occupied
            if(map[row][col] != 0)
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