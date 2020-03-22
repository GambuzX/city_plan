#include <vector>
#include "Project.h"

using namespace std;

int Project::nextID = 1;

BuildingType getBuildingType(char type) {
    switch(type) {
        case 'R':
            return BuildingType::residencial;
        case 'U':
            return BuildingType::utility;
    }
    return BuildingType::unknown;
}

bool Project::fits(const vector<vector<int>> & map, int x, int y) {    
    if(x + plan.size() >= map.size())
        return false;
    if(y + plan[0].size() >= map[0].size())
        return false;
    
    int line, column;

    for(size_t i = 0; i < plan.size(); i++){
        for(size_t j = 0; j < plan[i].size(); j++){
            if(plan[i][j] == '.')
                continue;

            line = x + i; 
            column = y + j;

            if(map[line][column] != -1)
                return false;
        } 
    }

    return true;
}