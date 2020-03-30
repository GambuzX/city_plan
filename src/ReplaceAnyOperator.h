#pragma once

#include <vector>
#include <unordered_map>
#include "Operator.h"

class ReplaceAnyOperator : public Operator {
    public:
        ReplaceAnyOperator(const State & s) : Operator(s) {}
        virtual ~ReplaceAnyOperator() {};
        virtual std::string getName() const { return "REPLACE ANY"; }
        virtual std::string getActionName() const { return "replacing any"; };

        virtual State apply(bool findBest) const {
            // global vars
            bMatrix map = initialState.getFilledPositions();
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            std::vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();

            // best choice variables    
            State state = initialState;

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
                        state.createBuilding(proj, row, col);
                        return state;
                    } 
                }

                updateUsedMap(map, removed.getProject(), row, col, true);
            }

            return initialState;
        }
};