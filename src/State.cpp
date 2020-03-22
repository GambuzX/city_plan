#include <vector>
#include <unordered_map>

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
    buildings.insert(make_pair(nextID++, Building(proj, x, y)));
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