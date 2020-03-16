#pragma once
#include <vector>
#include <string>

#include "Project.h"

struct InputInfo {
    int rows, cols, maxWalkDist;
    InputInfo(int r, int c, int D) : rows(r), cols(c), maxWalkDist(D) {}
};

InputInfo parseInput(std::string filename, std::vector<Project> & projects);