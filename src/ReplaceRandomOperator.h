#pragma once

#include <vector>
#include <unordered_map>
#include "Operator.h"

class ReplaceRandomOperator : public Operator {
    public:
        ReplaceRandomOperator(const State & s) : Operator(s) {}
        virtual ~ReplaceRandomOperator() {};
        virtual std::string getName() const { return "REPLACE ANY"; }
        virtual std::string getActionName() const { return "replacing any"; };

        virtual State apply(bool findBest) const {
            // global vars
            bMatrix map = initialState.getFilledPositions();
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            std::vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();

            // best choice variables    
            State state = initialState;
         
            size_t removeRandOffset = rand() % buildingsIDs.size();
            for (size_t i = 0; i < buildingsIDs.size(); i++) {
                uint b = buildingsIDs[(i + removeRandOffset) % buildingsIDs.size()]; // apply offset

                // remove building first   
                Building removed = state.removeBuilding(b, false);
                int row = removed.getRow(), col = removed.getCol();
                updateUsedMap(map, removed.getProject(), row, col, false);

                // replace building project
                size_t projRandOffset = rand() % projects.size();
                for(size_t p = 0; p < projects.size(); p++) {
                    Project * proj = (Project*) &projects[(p + projRandOffset) % projects.size()]; // apply offset
                    if(proj->getID() == removed.getProject()->getID()) continue;

                    // check if can create
                    if(state.canCreateBuilding(proj, row, col, &map)){
                        state.updateMapLimitsRemove(removed);
                        state.createBuilding(proj, row, col);
                        return state;
                    } 
                }

                // revert deletion
                updateUsedMap(map, removed.getProject(), row, col, true);
            }

            return initialState;
        }
};