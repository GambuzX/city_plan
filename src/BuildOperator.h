#pragma once

#include <vector>
#include <algorithm>
#include "Operator.h"

class BuildOperator : public Operator {
    public:
        BuildOperator(const State & s) : Operator(s) {}
        virtual ~BuildOperator() {};
        virtual std::string getName() const { return "BUILD"; }
        virtual std::string getActionName() const { return "building"; };

        virtual State apply(bool findBest) const {
            int D = initialState.getGlobalInfo()->maxWalkDist;
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            bMatrix map = initialState.getFilledPositions();

            State state = initialState;
            int initialValue = state.value();

            Project * bProject;
            uint bEmptyCount = state.emptyCount();
            int bRow, bCol, bValue = initialValue;

            int minRow = std::max(0, initialState.getMinRow()-D);
            int maxRow = std::min((int)map.size()-1, initialState.getMaxRow() + D);
            int minCol = std::max(0, initialState.getMinCol()-D);
            int maxCol = std::min((int)map[0].size()-1, initialState.getMaxCol() + D);

            for(int row = minRow; row <= maxRow; row++){
                for(int col = minCol; col <= maxCol; col++){

                    // check if empty
                    if(map[row][col])
                        continue;
                    
                    // try to build all projects
                    for(size_t p = 0; p < projects.size(); p++) {
                        Project * currProject = (Project *) &projects[p];

                        if(state.canCreateBuilding(currProject, row, col, &map)){
                            // create building and check its value
                            uint newBuildingID = state.createBuilding(currProject, row, col, false); // do not update map limits in this step
                            int newStateValue = state.value();   

                            if(State::betterState(bValue, bEmptyCount, newStateValue, state.emptyCount())) {
                                // if only want a better solution return immediately
                                if(!findBest) {
                                    state.updateMapLimitsCreate(currProject, row, col);
                                    return state;
                                }

                                // assign variables
                                bProject = currProject;
                                bRow = row;
                                bCol = col;
                                bValue = newStateValue;
                                bEmptyCount = state.emptyCount();
                            }

                            // remove building to maintain state
                            state.removeBuilding(newBuildingID, false);
                        } 
                    }
                }
            }
            // reached a better solution
            if(State::betterState(initialValue, state.emptyCount(), bValue, bEmptyCount)) {
                state.createBuilding(bProject, bRow, bCol);
                return state;
            }

            return initialState;
        }
};