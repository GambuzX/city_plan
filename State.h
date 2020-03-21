#pragma once

#include <vector>
#include <unordered_map>
#include "Building.h"

typedef unsigned int uint;

class State {
    private:
        uint nextID;
        std::unordered_map<uint, Building> buildings;
        std::vector<std::vector<int>> cityMap;
        std::vector<int> residentialBuildings; // ids of residential buildings
        std::vector<int> utilityBuildings; // ids of utility buildings
    public:

        State(int rows, int cols) {
            nextID = 1;
            cityMap = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
        }

        State(const State &s){
            this->nextID = s.getNextID();
            this->buildings = s.getBuildings();
            this->cityMap = s.getCityMap();
            this->residentialBuildings = s.getResidentialBuildings();
            this->utilityBuildings = s.getUtilityBuildings();
        }

        bool canAddBuilding(Building & b) const {
            // check if can add
            // pre conditions
            return true;
        }

        void addBuilding(Project * proj, int x, int y) {
            buildings.insert(std::make_pair(nextID++, Building(proj, x, y)));
            // update city map with building index (buildings.size()-1)
        }

        const uint getNextID() const { return nextID; }
        const std::unordered_map<uint, Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<int>> & getCityMap() const { return cityMap; }
        const std::vector<int> & getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<int> & getUtilityBuildings() const { return utilityBuildings; }
};