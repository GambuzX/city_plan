#include "Project.h"

using namespace std;

int Project::nextID = 0;

BuildingType getBuildingType(char type) {
    switch(type) {
        case 'R':
            return BuildingType::residencial;
        case 'U':
            return BuildingType::utility;
    }
    return BuildingType::unknown;
}