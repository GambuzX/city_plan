#include <vector>

class Building {
    private:
        int id;
        BuildingType type;
        int value;
        std::vector<std::vector<char>> plan;
        static int nextID;

    public:
        Building(BuildingType type, int value, std::vector<std::vector<char>> plan) { 
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

int Building::nextID = 1;

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