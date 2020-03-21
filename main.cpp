#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <climits>
#include <algorithm>

#include "State.h"
#include "InputParse.h"

using namespace std;

int value(const State & state, const InputInfo & i);
int manhattanDistance(int x1, int y1, int x2, int y2);
int buildingsDist(const Building & b1, const Building & b2);
State neighborhood(const State &state, const vector<Project> &projects, const vector<int> &utilityTypes, const int &D);
bool fits(const vector<vector<int>> & map, Project *proj, int x, int y);
State hillClimbing(const vector<Project> & p, const InputInfo & i, const State & s);
State getHigherValueNeighbour(const vector<Project> & projects, const InputInfo & globalInfo, const State & state);

int main() {
    vector<Project> bProjects;
    InputInfo globalInfo = parseInput("inputs/example.txt", bProjects);
    State initialState = State(globalInfo.rows, globalInfo.cols);

    State localMaximum = hillClimbing(bProjects, globalInfo, initialState);
    
    return 0;
}

State hillClimbing(const vector<Project> & projects, const InputInfo & globalInfo, const State & initialState) {

    State currentState = initialState;
    int previousValue, currentValue;
    previousValue = currentValue = value(currentState, globalInfo);

    cout << "[+] Starting hill climbing with value " << currentValue << endl;
    while(1) {
        State neighbour = getHigherValueNeighbour(projects, globalInfo, currentState);
        currentValue = value(neighbour, globalInfo);
        cout << "[+] Found neighbour: " << currentValue << endl;

        if (currentValue < previousValue) {     
            cout << "[+] Reached local maximum" << endl;
            break;
        }
        currentState = neighbour;
        previousValue = currentValue;
    }
    
    return currentState;
}

int buildingsDist(const Building & b1, const Building & b2) {
    int b1X = b1.getX(), b1Y = b1.getY(), b2X = b2.getX(), b2Y = b2.getY();
    const vector<vector<char>> & b1Plan = b1.getProject()->getPlan();
    const vector<vector<char>> & b2Plan = b2.getProject()->getPlan();

    int dist = INT_MAX;
    for (int row1 = b1Y; row1 < b1Y + b1Plan.size(); row1++)
        for (int col1 = b1X; col1 < b1X + b1Plan[0].size(); col1++)
            for (int row2 = b2Y; row2 < b2Y + b2Plan.size(); row2++)
                for (int col2 = b2X; col2 < b2X + b2Plan[0].size(); col2++)
                    dist = min(dist, manhattanDistance(row1,col1,row2,col2));
    return dist;
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int value(const State & state, const InputInfo & globalInfo) { //TODO must consider number of buildings also. more buildings is penalized
    const vector<int> & utilityTypes = globalInfo.allUtilities;
    const int D = globalInfo.maxWalkDist;
    
    int points = 0;
    const unordered_map<uint, Building> & buildings = state.getBuildings();
    const vector<int> & resBuildings = state.getResidentialBuildings();
    const vector<int> & utilBuildings = state.getUtilityBuildings();

    // for all residential buildings
    for (int rIndex : resBuildings) { 
        const Building & resBuilding = buildings.find(rIndex)->second;

        // check all utilities that exist
        for (int utilityType : utilityTypes) {

            // for all utility buildings
            for (int uIndex : utilBuildings) {
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

State getHigherValueNeighbour(const vector<Project> & projects, const InputInfo & globalInfo, const State & state){
    const vector<vector<int>> & map = state.getCityMap();
    int stateValue = value(state, globalInfo);

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(int p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(fits(map, currProject, row, col)){

                    // create building and check its value
                    State new_state = state;
                    new_state.addBuilding(currProject, row, col);
                    int new_state_value = value(new_state, globalInfo);
                    
                    // found better state
                    if(new_state_value > stateValue){
                        return new_state;
                    }

                }
            }
        }
    }

    // did not find better neighbour
    return state;
}

State getHighestValueNeighbor(vector<Project> & projects, const InputInfo & globalInfo, const State & state){
    State best_state = state;
    int best_value = INT_MIN;

    const vector<vector<int>> & map = state.getCityMap();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(int p = 0; p < projects.size(); p++) {
                Project * currProject = &projects[p];

                if(fits(map, currProject, row, col)){

                    // create building and check its value
                    State new_state = state;
                    new_state.addBuilding(currProject, row, col);
                    int new_state_value = value(new_state, globalInfo);
                    
                    if(new_state_value > best_value){
                        best_state = new_state;
                        best_value = new_state_value;
                    }

                }
            }
        }
    }

    return best_state;
}

bool fits(const vector<vector<int>> & map, Project *proj, int x, int y){
    vector<vector<char>> plan_map = proj->getPlan();
    
    if(x + plan_map.size() >= map.size())
        return false;
    if(y + plan_map[0].size() >= map[0].size())
        return false;
    
    int line, column;

    for(size_t i = 0; i < plan_map.size(); i++){
        for(size_t j = 0; j < plan_map[i].size(); j++){
            if(plan_map[i][j] == '.')
                continue;

            line = x + i; 
            column = y + j;

            if(map[line][column] != -1)
                return false;
        } 
    }

    return true;
}