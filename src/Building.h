#pragma once
#include "Project.h"

class Building {
    private:
        Project * proj;
        int row, col; //left top corner
    public:
        Building() {}
        Building(Project * proj, int row, int col) {
            this->proj = proj;
            this->row = row;
            this->col = col;
        }

        int getRow() const { return row; }
        int getCol() const { return col; }
        Project * getProject() const { return proj; }
};


int manhattanDistance(int row1, int col1, int row2, int col2);
int buildingsDist(const Building & b1, const Building & b2);
bool buildingsDistLessThanD(const Building & b1, const Building & b2, const int &D);