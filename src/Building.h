#pragma once
#include "Project.h"

class Building {
    private:
        Project * proj;
        int x, y; //left top corner
    public:
        Building(Project * proj, int x, int y) {
            this->proj = proj;
            this->x = x;
            this->y = y;
        }

        int getX() const { return x; }
        int getY() const { return y; }
        Project * getProject() const { return proj; }
};


int manhattanDistance(int x1, int y1, int x2, int y2);
int buildingsDist(const Building & b1, const Building & b2);