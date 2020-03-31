#pragma once

#include "TabuOperator.h"
#include "Util.h"

class RemoveTabuOperator : public TabuOperator {
    public:
        RemoveTabuOperator(const State & s) : Operator(s) {}
        virtual ~RemoveTabuOperator() {};
        virtual std::string getName() const { return "REMOVE RANDOM"; }
        virtual std::string getActionName() const { return "removing random"; };

        virtual State apply(bool findBest) const {
            if(initialState.getBuildings().size() == 0) return initialState;
            
            std::vector<uint> allIDs = initialState.getAllBuildingsIDs();
            uint toRemove = getRandomValue() % allIDs.size();

            if(checkRestrictedAction(-1,-1,toRemove,REMOVE)){
                State state = initialState;
                Building removed = state.removeBuilding(allIDs[toRemove]);

                Tabu *buildTabu = new Tabu();
                buildTabu->setPositionRestriction(removed.getRow(),removed.getCol());
                buildTabu->setBuildingRestriction(removed.getProject()->getID());
                buildTabu->setOperationRestriction(BUILD);

                addTabu(buildTabu);
            }

            return state;
        }
};