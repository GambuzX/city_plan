#pragma once

#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Operator.h"
#include "Util.h"

// build the first building that encounters
class BuildRandomPositionOperator : public Operator {
    private:
        int iterations;
    public:
        BuildRandomPositionOperator(const State & s, int iterations=1000) : Operator(s) {
            this->iterations = std::min(iterations, s.getGlobalInfo()->rows * s.getGlobalInfo()->cols);
        }
        virtual ~BuildRandomPositionOperator() {};
        virtual std::string getName() const { return "BUILD RANDOM POSITION"; }
        virtual std::string getActionName() const { return "building in random position"; };

        virtual State apply(bool findBest) const {
            const std::vector<Project> & projects = initialState.getGlobalInfo()->bProjects;
            bMatrix map = initialState.getFilledPositions();
            State state = initialState;
            int initialValue = state.value();
            size_t maxRow = map.size(), maxCol = map[0].size();
            std::set<std::string> seen;
            int rRow, rCol;
            std::string hash;

            for (int i = 0; i < iterations; i++) {
                // random position
                rRow = rand() % maxRow;
                rCol = rand() % maxCol;
                hash = std::to_string(rRow) + "-" + std::to_string(rCol);
                if(seen.find(hash) != seen.end()) continue; // already tried
                seen.insert(hash);

                // not empty
                if(map[rRow][rCol]) continue;

                // try to build all projects, starting in a random one
                size_t randomOffset = getRandomValue() % projects.size();               
                for(size_t p = 0; p < projects.size(); p++) {
                    size_t projectIndex = (p + randomOffset) % projects.size();
                    Project * currProject = (Project *) &projects[projectIndex];

                    if(state.canCreateBuilding(currProject, rRow, rCol, &map)){
                        uint newBuildingID = state.createBuilding(currProject, rRow, rCol, false); // do not update map limits in this step
                        int newStateValue = state.value();   

                        // only return if better state
                        if(State::betterState(initialValue, initialState.emptyCount(), newStateValue, state.emptyCount())) {
                            state.updateMapLimitsCreate(currProject, rRow, rCol);
                            return state;
                        }

                        // remove building to maintain state
                        state.removeBuilding(newBuildingID, false);
                    } 
                }
            }
            return initialState;
        }
};