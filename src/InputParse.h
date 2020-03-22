#pragma once
#include <vector>
#include <string>

#include "Project.h"

struct InputInfo {
    int rows, cols, maxWalkDist;
    std::vector<int> allUtilities;
    std::vector<Project> bProjects;
    InputInfo(int r, int c, int D, std::vector<int> u, std::vector<Project> b) {
        rows = r;
        cols = c;
        maxWalkDist = D;
        allUtilities = u;
        bProjects = b;
    }
};

InputInfo parseInput(std::string filename);