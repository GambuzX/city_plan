#pragma once

#include <vector>
#include <unordered_map>
#include "Building.h"
#include "InputParse.h"

typedef unsigned int uint;

class State {
    private:
        uint nextID;
        uint emptyCells;
        InputInfo * globalInfo;
        std::unordered_map<uint, Building> buildings;
        std::vector<std::vector<int>> cityMap;
        std::vector<int> residentialBuildings; // ids of residential buildings
        std::vector<int> utilityBuildings; // ids of utility buildings
    public:

        State(InputInfo * globalInfo) {
            int rows = globalInfo->rows;
            int cols = globalInfo->cols;
            this->globalInfo = globalInfo;
            nextID = 1;
            emptyCells = rows*cols;
            cityMap = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
        }

        State(const State &s){
            this->nextID = s.getNextID();
            this->emptyCells = s.getEmptyCells();
            this->globalInfo = s.getGlobalInfo();
            this->buildings = s.getBuildings();
            this->cityMap = s.getCityMap();
            this->residentialBuildings = s.getResidentialBuildings();
            this->utilityBuildings = s.getUtilityBuildings();
        }

        bool canCreateBuilding(Project * proj, int x, int y) const;
        void createBuilding(Project * proj, int x, int y);
        int value() const;

        uint getNextID() const { return nextID; }
        uint getEmptyCells() const { return emptyCells; }
        InputInfo * getGlobalInfo() const { return globalInfo; }
        const std::unordered_map<uint, Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<int>> & getCityMap() const { return cityMap; }
        const std::vector<int> & getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<int> & getUtilityBuildings() const { return utilityBuildings; }
};