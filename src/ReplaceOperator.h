#pragma once

#include <vector>
#include <unordered_map>
#include "Operator.h"

class ReplaceOperator : public Operator {
    public:
        ReplaceOperator(const State & s) : Operator(s) {}
        virtual std::string getName() const { return "REPLACE"; }
        virtual std::string getActionName() const { return "replacing"; };

        virtual State apply(bool findBest) const {
            // global vars
            int initialValue = initialState.value();
            bMatrix map = initialState.getFilledPositions();
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            std::vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();

            // best choice variables    
            State state = initialState;
            uint bestToRemove = 0;
            Project * bProject;
            int bRow, bCol;
            int bValue = initialValue, bEmptyCount = state.emptyCount();

            // remove building first
            for (uint b : buildingsIDs) {
                Building removed = state.removeBuilding(b, false);
                int row = removed.getRow(), col = removed.getCol();
                updateUsedMap(map, removed.getProject(), row, col, false);

                // replace building project
                for(size_t p = 0; p < projects.size(); p++) {
                    Project * proj = (Project*) &projects[p];
                    if(proj->getID() == removed.getProject()->getID()) continue;

                    // check if can create
                    if(state.canCreateBuilding(proj, row, col, &map)){

                        // create building and check its value
                        uint newBuildingID = state.createBuilding(proj, row, col, false); // do not update map limits in this step
                        int newStateValue = state.value();   

                        if(State::betterState(bValue, bEmptyCount, newStateValue, state.emptyCount())) {
                            // if only want a better solution return immediately
                            if(!findBest) {
                                state.updateMapLimitsRemove(removed);
                                state.updateMapLimitsCreate(proj, row, col);
                                return state;
                            }

                            // update best solution variables
                            bProject = proj;
                            bRow = row;
                            bCol = col;
                            bValue = newStateValue;
                            bEmptyCount = state.emptyCount();
                            bestToRemove = b;
                        }

                        // remove building to maintain state
                        state.removeBuilding(newBuildingID, false);
                    } 
                }
                updateUsedMap(map, removed.getProject(), row, col, true);
            }

            // reached a better solution, apply operations
            if(State::betterState(initialValue, state.emptyCount(), bValue, bEmptyCount)) {
                state.removeBuilding(bestToRemove);
                state.createBuilding(bProject, bRow, bCol);
                return state;
            }

            return initialState;
        }
};