#pragma once

#include <vector>
#include "Building.h"

class State {
    private:
        std::vector<Building> buildings;
        std::vector<std::vector<int>> cityMap;
        std::vector<int> residentialBuildings; // indexes of residential buildings
        std::vector<int> utilityBuildings; // indexes of utility buildings
    public:

        State(int rows, int cols) {
            cityMap = std::vector<std::vector<int>>(rows, std::vector<int>(cols, -1));
        }

        State(const State &s){
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
            buildings.push_back(Building(proj, x, y));
            // update city map with building index (buildings.size()-1)
        }

        const std::vector<Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<int>> & getCityMap() const { return cityMap; }
        const std::vector<int> getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<int> getUtilityBuildings() const { return utilityBuildings; }
};