#pragma once
#include "Plan.h"

class Building {
    private:
        Plan * plan;
        int x, y;
    public:
        Building(Plan * plan, int x, int y) {
            this->plan = plan;
            this->x = x;
            this->y = y;
        }
};