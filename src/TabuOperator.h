#pragma once

#include <string>
#include <vector>
#include "State.h"
#include "Tabu.h"

class TabuOperator {
    protected:
        const State & initialState;
        
        static std::vector<Tabu*> tabuList;
        static size_t maxTabus;
        static void addTabu(Tabu *tabu){
            tabuList.push_back(tabu);
            while(tabuList.size() > maxTabus){
                delete(tabuList[0]);
                tabuList.erase(tabuList.begin());
            }
        }
        static bool checkRestrictedAction(int row, int col, int projectID, OperationType operation){
            for(Tabu *tabu : tabuList){
                if (tabu->restrictedAction(row,col,projectID,operation)) return true;
            }
            return false;
        }

    public:
        Operator(const State & s) : initialState(s) {}
        virtual ~Operator() {};
        virtual State apply(bool findBest = false) const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getActionName() const = 0;
        
        static void setMaxTabus(size_t max){maxTabus = max;}

        static void clearTabus(){
            while(tabuList.size()){
                delete(tabuList[0]);
                tabuList.erase(tabuList.begin());
            }
        }
};