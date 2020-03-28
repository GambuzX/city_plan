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
            const std::unordered_map<uint, Building> & buildings = initialState.getBuildings();
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;

            State bestState = initialState;
            int bestValue = initialState.value();

            std::vector<uint> buildingsIDs = initialState.getAllBuildingsIDs();
            for (int b : buildingsIDs) {
                std::unordered_map<uint, Building>::const_iterator it = buildings.find(b);
                if(it == buildings.end()) continue;

                // remove building so that it can be replaced
                State newStateRemove = initialState;
                newStateRemove.removeBuilding(b);

                // try to replace removed building with all projects
                for (size_t p = 0; p < projects.size(); p++) {
                    Project * proj = (Project*) &projects[p];
                    if(it->second.getProject()->getID() == proj->getID()) continue;

                    if(newStateRemove.canCreateBuilding(proj, it->second.getRow(), it->second.getCol())) {
                        State newStateReplace = newStateRemove;
                        newStateReplace.createBuilding(proj, it->second.getRow(), it->second.getCol());
                        int newStateValue = newStateReplace.value();

                        if(State::betterState(bestValue, bestState.emptyCount(), newStateValue, newStateReplace.emptyCount())) {
                            if(!findBest) return newStateReplace;

                            bestState = newStateReplace;
                            bestValue = newStateValue;
                        }

                    }
                }
            }
            return bestState;
        }
};