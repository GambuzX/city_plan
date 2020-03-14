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

        bool canAddBuilding(Building & b) const {
            // check if can add
            // pre conditions
            return true;
        }

        void addBuilding(Plan * plan, int x, int y) {
            buildings.push_back(Building(plan, x, y));
            // update city map with building index (buildings.size()-1)
        }

        const std::vector<Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<int>> & getCityMap() const { return cityMap; }
        const std::vector<int> getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<int> getUtilityBuildings() const { return utilityBuildings; }
};