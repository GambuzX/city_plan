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

        State() {}
        State(InputInfo * globalInfo);

        State(const State &s);

        State(std::vector<std::vector<uint>> v1, std::unordered_map<uint, Building> um1, uint max_id1, 
              std::vector<std::vector<uint>> v2, std::unordered_map<uint, Building> um2, InputInfo *globalInfo);

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

        bool operator ==(const State& s) const;
};

namespace std
{
    template<>
    struct hash<State>
    {
        size_t operator()(const State & obj) const
        {
            return hash<int>()(obj.getNextID() + 
                                obj.emptyCount() + 
                                obj.getBuildings().size()
            );
        }
    };
}