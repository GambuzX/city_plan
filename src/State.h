#pragma once

#include <vector>
#include <unordered_map>
#include "Building.h"
#include "InputParse.h"

typedef unsigned int uint;
typedef std::vector<std::vector<bool>> bMatrix;

class State {
    private:
        uint nextID;
        uint emptyCells;
        InputInfo * globalInfo;
        std::unordered_map<uint, Building> buildings;
        std::vector<uint> residentialBuildings; // ids of residential buildings
        std::vector<uint> utilityBuildings; // ids of utility buildings
        int minRow, maxRow, minCol, maxCol;

    public:

        State() {}
        State(InputInfo * globalInfo);
        State(const State &s);

        bool addRandomBuilding();
        bool canCreateBuilding(Project * proj, int row, int col) const;
        bool canCreateBuilding(Project * proj, int row, int col, bMatrix * filledPos) const;
        uint createBuilding(Project * proj, int row, int col, bool updateLimits = true);
        Building removeBuilding(uint id, bool updateLimits = true);
        void updateMapLimitsCreate(Project * proj, int row, int col);
        void updateMapLimitsRemove(const Building & removed);
        int value() const;
        void printMap() const;
        bool isPositionNearBuildings(int row, int col) const;
        std::vector<std::vector<uint>> getCityMap() const;
        bMatrix getFilledPositions() const;

        uint getNextID() const { return nextID; }
        uint emptyCount() const { return emptyCells; }
        InputInfo * getGlobalInfo() const { return globalInfo; }
        const std::unordered_map<uint, Building> & getBuildings() const { return buildings; }
        const std::vector<uint> & getResidentialBuildings() const { return residentialBuildings; }
        const std::vector<uint> & getUtilityBuildings() const { return utilityBuildings; }
        std::vector<uint> getAllBuildingsIDs() const;
        int getMinRow() const { return minRow; }
        int getMaxRow() const { return maxRow; }
        int getMinCol() const { return minCol; }
        int getMaxCol() const { return maxCol; }

        bool operator==(const State& s) const;

        static bool betterState(const State & s1, const State & s2);
        static bool betterState(int pValue, int pEmptyCells, int nValue, int nEmptyCells);
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

void updateUsedMap(bMatrix & map, Project * p, int row, int col, bool used);
State generateState(InputInfo *globalInfo);
std::vector<State> generatePopulation(InputInfo *global_info, int populationSize);
State * generatePopulationPtr(InputInfo *global_info, int populationSize);