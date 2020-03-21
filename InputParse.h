#pragma once
#include <vector>
#include <string>

#include "Project.h"

struct InputInfo {
    int rows, cols, maxWalkDist;
    std::vector<int> allUtilities;
    InputInfo(int r, int c, int D, std::vector<int> u) : rows(r), cols(c), maxWalkDist(D), allUtilities(u) {}
};

InputInfo parseInput(std::string filename, std::vector<Project> & projects);