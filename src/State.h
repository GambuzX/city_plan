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
        int minRow, maxRow, minCol, maxCol;

        void updateMapLimits(int sRow, int eRow, int sCol, int eCol);
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
        bool canCreateBuilding(Project * proj, int row, int col) const;
        uint createBuilding(Project * proj, int row, int col);
        void removeBuilding(uint id);
        int value() const;
        void printMap() const;
        bool isPositionNearBuildings(int row, int col) const;

        uint getNextID() const { return nextID; }
        uint emptyCount() const { return emptyCells; }
        InputInfo * getGlobalInfo() const { return globalInfo; }
        const std::unordered_map<uint, Building> & getBuildings() const { return buildings; }
        const std::vector<std::vector<uint>> & getCityMap() const { return cityMap; }
        const std::vector<uint> & getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<uint> & getUtilityBuildings() const { return utilityBuildings; }
        std::vector<uint> getAllBuildingsIDs() const;
        int getMinRow() const { return minRow; }
        int getMaxRow() const { return maxRow; }
        int getMinCol() const { return minCol; }
        int getMaxCol() const { return maxCol; }
};