#pragma once

#include <vector>

enum BuildingType {
    residencial,
    utility,
    unknown
};

BuildingType getBuildingType(char type) {
    switch(type) {
        case 'R':
            return BuildingType::residencial;
        case 'U':
            return BuildingType::utility;
    }
    return BuildingType::unknown;
}

class Plan {
    private:
        int id;
        BuildingType type;
        int value;
        std::vector<std::vector<char>> plan;
        static int nextID;

    public:
        Plan(BuildingType type, int value, std::vector<std::vector<char>> plan) { 
            this->id = nextID++;
            this->type = type;
            this->value = value;
            this->plan = plan;
        }

        int getID() { return this->id; }
        BuildingType getType() { return this->type; }
        int getValue() { return this->value; }
        std::vector<std::vector<char>> getPlan() { return plan; }
};

int Plan::nextID = 1;
