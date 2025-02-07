#pragma once

#include <map>
#include "Project.h"

class Building {
    private:
        Project * proj;
        int row, col; //left top corner
        std::map<int, int> nearTypes;
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
        bool aboveRow(int separation) const;
        bool belowRow(int separation) const;
        bool beforeCol(int separation) const;
        bool afterCol(int separation) const;
        const std::map<int,int> & getNearTypes() const { return nearTypes; };
        void addNearType(int);
        void removeNearType(int);
        int getNearTypesCount() const;
};


int manhattanDistance(int row1, int col1, int row2, int col2);
int buildingsDist(const Building & b1, const Building & b2);
bool buildingsDistLessThanD(const Building & b1, const Building & b2, const int &D);