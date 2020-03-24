#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "State.h"

using namespace std;

bool State::canCreateBuilding(Project * proj, uint row, uint col) const {

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

uint State::createBuilding(Project * proj, uint row, uint col) {
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

    minRow = min(minRow, row);
    maxRow = max(maxRow, (uint) (row + plan.size()));
    minCol = min(minCol, col);
    maxCol = max(maxCol, (uint) (col + plan[0].size()));

    buildings.insert(make_pair(ID, Building(proj, row, col)));
    return ID;
}

void State::removeBuilding(uint id) {
    unordered_map<uint, Building>::iterator it = buildings.find(id);
    if (it == buildings.end()) return;

    const Building & b = it->second;
    int x = b.getCol(), y = b.getRow();
    const vector<vector<char>> & plan = b.getProject()->getPlan();
    for (size_t row = y; row < y + plan.size(); row++) {
        for (size_t col = x; col < x + plan[0].size(); col++) {
            if (cityMap[row][col] != 0) emptyCells++;
            cityMap[row][col] = 0;
        }
    }

    if(it->second.getProject()->getType() == BuildingType::residencial)
        residentialBuildings.erase(remove(residentialBuildings.begin(), residentialBuildings.end(), id), residentialBuildings.end()); 

    if(it->second.getProject()->getType() == BuildingType::utility)
        utilityBuildings.erase(remove(utilityBuildings.begin(), utilityBuildings.end(), id), utilityBuildings.end()); 

    buildings.erase(it);
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