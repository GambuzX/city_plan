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
        std::vector<std::vector<uint>> cityMap;
        std::vector<uint> residentialBuildings; // ids of residential buildings
        std::vector<uint> utilityBuildings; // ids of utility buildings
        uint minRow, maxRow, minCol, maxCol; //TODO store available cells?

        void updateMapLimits(uint sRow, uint eRow, uint sCol, uint eCol);
    public:

        State(InputInfo * globalInfo) {
            int rows = globalInfo->rows;
            int cols = globalInfo->cols;
            this->globalInfo = globalInfo;
            nextID = 1;
            emptyCells = rows*cols;
            cityMap = std::vector<std::vector<uint>>(rows, std::vector<uint>(cols, 0));
            minRow = maxRow = minCol = maxCol = 0;
        }

        State(const State &s){
            this->nextID = s.getNextID();
            this->emptyCells = s.emptyCount();
            this->globalInfo = s.getGlobalInfo();
            this->buildings = s.getBuildings();
            this->cityMap = s.getCityMap();
            this->residentialBuildings = s.getResidentialBuildings();
            this->utilityBuildings = s.getUtilityBuildings();
            this->minRow = s.getMinRow();
            this->maxRow = s.getMaxRow();
            this->minCol = s.getMinCol();
            this->maxCol = s.getMaxCol();
        }

        bool addRandomBuilding();
        bool canCreateBuilding(Project * proj, uint row, uint col) const;
        uint createBuilding(Project * proj, uint row, uint col);
        void removeBuilding(uint id);
        int value() const;
        void printMap() const;
        bool isPositionNearBuildings(uint row, uint col) const;

        uint getNextID() const { return nextID; }
        uint emptyCount() const { return emptyCells; }
        InputInfo * getGlobalInfo() const { return globalInfo; }
        const std::unordered_map<uint, Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<uint>> & getCityMap() const { return cityMap; }
        const std::vector<uint> & getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<uint> & getUtilityBuildings() const { return utilityBuildings; }
        std::vector<uint> getAllBuildingsIDs() const;
        uint getMinRow() const { return minRow; }
        uint getMaxRow() const { return maxRow; }
        uint getMinCol() const { return minCol; }
        uint getMaxCol() const { return maxCol; }
};