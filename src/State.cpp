#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "State.h"

using namespace std;


bool State::canCreateBuilding(Project * proj, int x, int y) const {

    const vector<vector<char>> & plan = proj->getPlan();

    if (x < 0 || y < 0) 
        return false;   
    if(y + plan.size() > cityMap.size())
        return false;
    if(x + plan[0].size() > cityMap[0].size())
        return false;

    int cityRow, cityCol;
    for (size_t row = 0; row < plan.size(); row++) {
        for (size_t col = 0; col < plan[0].size(); col++) {
            cityRow = y+row;
            cityCol = x+col;
            if (plan[row][col] == '#' && cityMap[cityRow][cityCol] != 0) {
                return false;
            }
        }
    }

    return true;
}

uint State::createBuilding(Project * proj, int x, int y) {
    uint ID = nextID++;
    const vector<vector<char>> & plan = proj->getPlan();

    int cityRow, cityCol;
    for (size_t row = 0; row < plan.size(); row++) {
        for (size_t col = 0; col < plan[0].size(); col++) {
            if (plan[row][col] == '#') {              
                cityRow = y+row;
                cityCol = x+col;
                if (cityMap[cityRow][cityCol] == 0) emptyCells--;
                cityMap[cityRow][cityCol] = ID;
            }
        }
    }

    if (proj->getType() == BuildingType::residencial) residentialBuildings.push_back(ID);
    if (proj->getType() == BuildingType::utility) utilityBuildings.push_back(ID);

    buildings.insert(make_pair(ID, Building(proj, x, y)));
    return ID;
}

void State::removeBuilding(uint id) {
    unordered_map<uint, Building>::iterator it = buildings.find(id);
    if (it == buildings.end()) return;

    const Building & b = it->second;
    int x = b.getX(), y = b.getY();
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
                if(canCreateBuilding(&projs[i], col, row)) {
                    createBuilding(&projs[i], col, row);
                    return true;
                }
            }
        }
    }    

    // couldnt add anything
    return false;
}