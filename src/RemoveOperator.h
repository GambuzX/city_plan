#pragma once

#include <vector>
#include "Operator.h"

class RemoveOperator : public Operator {
    public:
        RemoveOperator(const State & s) : Operator(s) {}
        virtual std::string getName() const { return "REMOVE"; }
        virtual std::string getActionName() const { return "removing"; };

        virtual State apply(bool findBest) const {
            State state = initialState;
            int initialValue = state.value();

            uint bestToRemove = 0;
            int bValue = initialValue, bEmptyCount = state.emptyCount();

            std::list<uint> buildingsIDs = initialState.getAllBuildingsIDs();
            for (uint b : buildingsIDs) {

                Building removed = state.removeBuilding(b, false);
                int newValue = state.value();
                if(State::betterState(bValue, bEmptyCount, newValue, state.emptyCount())) {
                    // only want one better solution
                    if(!findBest) {
                        state.updateMapLimitsRemove(removed);
                        return state;
                    }

                    // update best solution so far
                    bestToRemove = b;
                    bValue = newValue;
                    bEmptyCount = state.emptyCount();
                }

                state.createBuilding(removed.getProject(), removed.getRow(), removed.getCol(), false);
            }

            // reached a better solution
            if(State::betterState(initialValue, state.emptyCount(), bValue, bEmptyCount)) {
                state.removeBuilding(bestToRemove);
                return state;
            }

            return initialState;
        }
};