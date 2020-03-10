#include <fstream>
#include "InputParse.h"

using namespace std;

InputInfo parseInput(string filename, vector<Plan> & plans) {
    ifstream inp(filename); // open file

    int nRows, nCols, maxWalkDist, nBuildProjs; // global vars
    char type; int bRows, bCols, val; // building vars

    inp >> nRows >> nCols >> maxWalkDist >> nBuildProjs;
    for (int p = 0; p < nBuildProjs; p++) {
        inp >> type >> bRows >> bCols >> val;
        vector<vector<char>> plan(bRows, vector<char>(bCols, '.'));
        for (int row = 0; row < bRows; row++)
            for (int col = 0; col < bCols; col++)
                inp >> plan[row][col];
        plans.push_back(Plan(getBuildingType(type), val, plan));        
    }
    inp.close();
    return InputInfo(nRows, nCols, maxWalkDist);
}