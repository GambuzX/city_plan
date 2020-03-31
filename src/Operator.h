#pragma once

#include <string>
#include <vector>
#include "State.h"
#include "Tabu.h"

class Operator {
    protected:
        const State & initialState;
        std::vector<Tabu*> tabuList;

    public:
        Operator(const State & s) : initialState(s) {}
        virtual ~Operator() {};
        virtual State apply(bool findBest = false) const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getActionName() const = 0;
        virtual std::vector<Tabu*> getTabuList() const {return tabuList;};
};