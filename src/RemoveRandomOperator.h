#pragma once

#include <random>
#include "Operator.h"

class RemoveRandomOperator : public Operator {
    public:
        RemoveRandomOperator(const State & s) : Operator(s) {}
        virtual ~RemoveRandomOperator() {};
        virtual std::string getName() const { return "REMOVE RANDOM"; }
        virtual std::string getActionName() const { return "removing random"; };

        virtual State apply(bool findBest) const {
            if(initialState.getBuildings().size() == 0) return initialState;
            
            std::vector<uint> allIDs = initialState.getAllBuildingsIDs();
            uint toRemove = rand() % allIDs.size();
            std::cout << "[*] Removed  " << toRemove << std::endl;
            State state = initialState;
            state.removeBuilding(allIDs[toRemove]);
            std::cout << "Buildings n = " << initialState.getBuildings().size() << std::endl;
            return state;
        }
};