#pragma once

#include <vector>
#include <unordered_map>
#include "TabuOperator.h"

class ReplaceTabuOperator : public TabuOperator {
    public:
        ReplaceTabuOperator(const State & s) : Operator(s) {}
        virtual ~ReplaceTabuOperator() {};
        virtual std::string getName() const { return "REPLACE RANDOM"; }
        virtual std::string getActionName() const { return "replacing random"; };

        virtual State apply(bool findBest) const {
            // global vars
            bMatrix map = initialState.getFilledPositions();
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            std::vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();
            
            if(!buildingsIDs.size()){
                return initialState;
            }
            
            // best choice variables    
            State state = initialState;
         
            size_t removeRandOffset = getRandomValue() % buildingsIDs.size();
            for (size_t i = 0; i < buildingsIDs.size(); i++) {
                uint b = buildingsIDs[(i + removeRandOffset) % buildingsIDs.size()]; // apply offset
                
                // remove building first   
                Building removed = state.removeBuilding(b, false);
                int row = removed.getRow(), col = removed.getCol();
                updateUsedMap(map, removed.getProject(), row, col, false);

                if(!checkRestrictedAction(row,col,removed.getProject()->getID(), REMOVE)){
                    // replace building project
                    size_t projRandOffset = getRandomValue() % projects.size(); //Starting point for the project list iteration
                    for(size_t p = 0; p < projects.size(); p++) {
                        size_t projectIndex = (p + projRandOffset) % projects.size(); 

                        Project * proj = (Project*) &projects[projectIndex]; 
                        if(proj->getID() == removed.getProject()->getID()) continue;

                        // check if can create
                        if(state.canCreateBuilding(proj, row, col, &map) && !checkRestrictedAction(row, col, proj->getID(), BUILD)){
                            state.updateMapLimitsRemove(removed);
                            uint buildingId = state.createBuilding(proj, row, col);

                            Tabu *removeTabu = new Tabu();
                            removeTabu->setBuildingRestriction(buildingId);
                            removeTabu->setOperationRestriction(REMOVE);

                            Tabu *buildTabu = new Tabu();
                            buildTabu->setPositionRestriction(removed.getRow(),removed.getCol());
                            buildTabu->setBuildingRestriction(removed.getProject()->getID());
                            buildTabu->setOperationRestriction(BUILD);

                            addTabu(removeTabu);
                            addTabu(buildTabu);

                            return state;
                        } 
                    }
                }

                // revert deletion
                state.createBuilding(removed.getProject(), row, col);
            }

            return initialState;
        }
};