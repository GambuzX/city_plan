#pragma once

#include <vector>
#include <algorithm>
#include "Operator.h"

// build the first building that encounters
class BuildAnyOperator : public Operator {
    public:
        BuildAnyOperator(const State & s) : Operator(s) {}
        virtual ~BuildAnyOperator() {};
        virtual std::string getName() const { return "BUILD ANY"; }
        virtual std::string getActionName() const { return "building any"; };

        virtual State apply(bool findBest) const {
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            bMatrix map = initialState.getFilledPositions();

            State state = initialState;
            for(size_t row = 0; row < map.size(); row++){
                for(size_t col = 0; col <= map[0].size(); col++){

                    // check if empty
                    if(map[row][col])
                        continue;
                    
                    // try to build all projects
                    for(size_t p = 0; p < projects.size(); p++) {
                        Project * currProject = (Project *) &projects[p];
                        if(state.canCreateBuilding(currProject, row, col, &map)){
                            state.createBuilding(currProject, row, col, true);
                            return state;
                        } 
                    }
                }
            }
            return initialState;
        }
};