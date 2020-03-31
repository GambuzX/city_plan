#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Operator.h"
#include "Util.h"

// build the first building that encounters
class BuildRandomOperator : public Operator {
    public:
        BuildRandomOperator(const State & s) : Operator(s) {}
        virtual ~BuildRandomOperator() {};
        virtual std::string getName() const { return "BUILD RANDOM"; }
        virtual std::string getActionName() const { return "building random"; };

        virtual State apply(bool findBest) const {
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            bMatrix map = initialState.getFilledPositions();

            State state = initialState;
            for(size_t row = 0; row < map.size(); row++){
                for(size_t col = 0; col <= map[0].size(); col++){

                    // check if empty
                    if(map[row][col])
                        continue;
                    
                    size_t randomOffset = getRandomValue() % projects.size();//Starting point for the project list iteration
                    std::cout << "[*] Found an empty cell (" << row << ", " << col << "), attempting to build one out of " << projects.size() << " candidates (starting index = " << randomOffset << ")" << std::endl;
                    
                    // try to build all projects
                    for(size_t p = 0; p < projects.size(); p++) {
                        size_t projectIndex = (p + randomOffset) % projects.size(); 

                        Project * currProject = (Project *) &projects[projectIndex];
                        if(state.canCreateBuilding(currProject, row, col, &map)){
                            std::cout << "[*] Built  " << currProject->getID() << std::endl;
                    
                            state.createBuilding(currProject, row, col, true);
                            return state;
                        } 
                    }
                }
            }
            std::cout << "[*] Did nothing" << std::endl;
            return initialState;
        }
};