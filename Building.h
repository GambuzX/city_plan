#pragma once
#include "Plan.h"

class Building {
    private:
        Plan * plan;
        int x, y; //left top corner
    public:
        Building(Plan * plan, int x, int y) {
            this->plan = plan;
            this->x = x;
            this->y = y;
        }

        int getX() const { return x; }
        int getY() const { return y; }
        Plan * getPlan() const { return plan; }
};