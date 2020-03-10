#pragma once

#include <vector>
#include "Building.h"

class State {
    private:
        std::vector<Building> buildings;
        std::vector<std::vector<int>> cityMap;
    public:

        State(int rows, int cols) {
            cityMap = std::vector<std::vector<int>>(rows, std::vector<int>(cols, -1));
        }

        bool canAddBuilding(Building & b) {
            // check if can add
            // pre conditions
            return true;
        }

        void addBuilding(Plan * plan, int x, int y) {
            buildings.push_back(Building(plan, x, y));
            // update city map with building index (buildings.size()-1)
        }
};