#pragma once

#include <vector>
#include <unordered_map>
#include <random>
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


            size_t randomBuildingIndex = rand() % buildingsIDs.size(); //Starting point for the building list iteration

            // remove building first
            for (unsigned int b = 0; b < buildingsIDs.size(); ++b) {
                State state = initialState;

                size_t buildingIndex = (b + randomBuildingIndex) % buildingsIDs.size();

                std::cout << "[*] There are " << buildingsIDs.size() << " buildings available" << std::endl;

                Building removed = state.removeBuilding(buildingIndex, false);
                int row = removed.getRow(), col = removed.getCol();
                updateUsedMap(map, removed.getProject(), row, col, false);

                size_t randomProjectIndex = rand() % projects.size();
                std::cout << "[*] Attempting to replace one out of " << projects.size() << " candidates (index = " << randomProjectIndex << ")" << std::endl;

                // replace building project
                for(size_t p = 0; p < projects.size(); p++) {
                    size_t projectIndex = (p + randomProjectIndex) % projects.size(); //Starting point for the project list iteration

                    Project * proj = (Project*) &projects[projectIndex]; 
                    if(proj->getID() == removed.getProject()->getID()) continue;

                    // check if can create
                    if(state.canCreateBuilding(proj, row, col, &map)){
                        std::cout << "[*] Replaced  " << buildingIndex << " of project " << removed.getProject()->getID() << " for " << proj->getID() << std::endl;
                    
                        state.createBuilding(proj, row, col);
                        return state;
                    } 
                }

                updateUsedMap(map, removed.getProject(), row, col, true);
            }

            std::cout << "[*] Did nothing" << std::endl;
            return initialState;
        }
};