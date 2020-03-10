#pragma once
#include <vector>
#include <string>

#include <Plan.h>

struct InputInfo {
    int rows, cols, maxWalkDist;
    InputInfo(int r, int c, int D) : rows(r), cols(c), maxWalkDist(D) {}
};

InputInfo parseInput(std::string filename, std::vector<Plan> & plans);