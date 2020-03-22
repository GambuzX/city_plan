#include <vector>
#include <unordered_map>
#include <algorithm>

#include "State.h"

using namespace std;


bool State::canCreateBuilding(Project * proj, int x, int y) const {

    const vector<vector<char>> & plan = proj->getPlan();

    if (x < 0 || y < 0) 
        return false;   
    if(x + plan.size() >= cityMap.size())
        return false;
    if(y + plan[0].size() >= cityMap[0].size())
        return false;

    for (size_t row = y; row < plan.size(); row++) {
        for (size_t col = x; col < plan[0].size(); col++) {
            if (plan[row][col] == '#' && cityMap[row][col] != 0) {
                return false;
            }
        }
    }

    return true;
}

void State::createBuilding(Project * proj, int x, int y) {
    const vector<vector<char>> & plan = proj->getPlan();
    for (size_t row = y; row < plan.size(); row++) {
        for (size_t col = x; col < plan[0].size(); col++) {
            if (plan[row][col] == '#') {                
                if (cityMap[row][col] == 0) emptyCells--;
                cityMap[row][col] = nextID;
            }
        }
    }

    if (proj->getType == BuildingType::residencial) residentialBuildings.push_back(nextID);
    if (proj->getType == BuildingType::utility) utilityBuildings.push_back(nextID);

    buildings.insert(make_pair(nextID++, Building(proj, x, y)));
}

void State::removeBuilding(uint id) {
    unordered_map<uint, Building>::iterator it = buildings.find(id);
    if (it == buildings.end()) return;

    const Building & b = it->second;
    const vector<vector<char>> & plan = b.getProject()->getPlan();
    for (size_t row = b.getY(); row < plan.size(); row++) {
        for (size_t col = b.getX(); col < plan[0].size(); col++) {
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