#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <climits>

#include "State.h"
#include "InputParse.h"

using namespace std;

int buildingsDist(const Building & b1, const Building & b2);
int value(const State & state, vector<int> utilityTypes, int D);
State neighborhood(const State &state, const vector<Project> &projects, const vector<int> &utilityTypes, const int &D);
bool fits(const State &state, const Project &proj, int x, int y);
int hillClimbing(vector<Project> p, InputInfo i, State s);

int main() {

    vector<Project> bProjects;

    InputInfo globalInfo = parseInput("inputs/example.txt", bProjects);
    State initialState = State(globalInfo.rows, globalInfo.cols);

    hillClimbing(bProjects, globalInfo, initialState);
    
    return 0;
}

int hillClimbing(vector<Project> projects, InputInfo globalInfo, State initialState) {

    return 0;
}

int buildingsDist(const Building & b1, const Building & b2) {
    return 0;
}

int value(const State & state, vector<int> utilityTypes, int D) {
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

State neighborhood(const State &state, const vector<Project> &projects, const vector<int> &utilityTypes, const int &D){
    State *best_state = NULL;
    int best_value = INT_MIN;

    vector<vector<int>> map = state.getCityMap();

    for(size_t i = 0; i < map.size(); i++){
        for(size_t j = 0; j < map[i].size(); j++){
            if(map[i][j] != -1)
                continue;
            
            for(Project p : projects){
                if(fits(state, p, i, j)){
                    State* new_state = new State(state);
                    new_state->addBuilding(&p, i, j);
                    int new_state_value = value(*new_state, utilityTypes, D);
                    
                    if(new_state_value > best_value){
                        delete best_state;
                        best_state = new_state;
                        best_value = new_state_value;
                    }else{
                        delete new_state;
                    }
                }
            }
        }
    }

    return *best_state;
}

bool fits(const State &state, const Project &proj, int x, int y){
    vector<vector<char>> plan_map = proj.getPlan();
    vector<vector<int>> city_map = state.getCityMap();
    
    if(x + plan_map.size() >= city_map.size())
        return false;
    if(y + plan_map[0].size() >= city_map[0].size())
        return false;
    
    int line, column;

    for(size_t i = 0; i < plan_map.size(); i++){
        for(size_t j = 0; j < plan_map[i].size(); j++){
            if(plan_map[i][j] == '.')
                continue;

            line = x + i; 
            column = y + j;

            if(city_map[line][column] != -1)
                return false;
        } 
    }

    return true;
}