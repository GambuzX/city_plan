#pragma once
#include "Project.h"

class Building {
    private:
        Project * proj;
        uint row, col; //left top corner
    public:
        Building(Project * proj, uint row, uint col) {
            this->proj = proj;
            this->row = row;
            this->col = col;
        }

        uint getRow() const { return row; }
        uint getCol() const { return col; }
        Project * getProject() const { return proj; }
};


int manhattanDistance(uint row1, uint col1, uint row2, uint col2);
int buildingsDist(const Building & b1, const Building & b2);