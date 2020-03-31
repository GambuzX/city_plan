#pragma once

#include <string>
#include "State.h"

class Operator {
    protected:
        const State & initialState;
        const vector<OperatorAction> tabuList;

    public:
        Operator(const State & s) : initialState(s) {}
        virtual ~Operator() {};
        virtual State apply(bool findBest = false) const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getActionName() const = 0;
        virtual void setTabu(vector<OperatorAction> tabuList);
};