#pragma once

#include <vector>

enum BuildingType {
    residencial,
    utility,
    unknown
};

BuildingType getBuildingType(char type);

class Project {
    private:
        int id;
        BuildingType type;
        int value;
        std::vector<std::vector<char>> plan;
        static int nextID;

    public:
        Project(BuildingType type, int value, std::vector<std::vector<char>> plan) { 
            this->id = nextID++;
            this->type = type;
            this->value = value;
            this->plan = plan;
        }

        int getID() { return this->id; }
        BuildingType getType() { return this->type; }
        int getValue() { return this->value; }
        std::vector<std::vector<char>> getPlan() const { return plan; }
};