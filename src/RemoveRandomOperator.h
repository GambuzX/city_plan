#pragma once

#include <vector>
#include <random>
#include "Operator.h"

class RemoveRandomOperator : public Operator {
    public:
        RemoveRandomOperator(const State & s) : Operator(s) {}
        virtual std::string getName() const { return "REMOVE RANDOM"; }
        virtual std::string getActionName() const { return "removing random"; };

        virtual State apply(bool findBest) const {
            uint toRemove = rand() % initialState.getAllBuildingsIDs().size();
            State state = initialState;
            state.removeBuilding(toRemove);
            return state;
        }
};