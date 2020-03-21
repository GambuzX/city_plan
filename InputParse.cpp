#include <iostream>
#include <fstream>
#include <unordered_set>
#include "InputParse.h"

using namespace std;

InputInfo parseInput(string filename, vector<Project> & projs) {
    cout << "[+] Parsing input" << endl;
    ifstream inp(filename); // open file

    int nRows, nCols, maxWalkDist, nBuildProjs; // global vars
    char type; int bRows, bCols, val; // building vars
    unordered_set<int> seenUtilities;
    vector<int> allUtilities;

    inp >> nRows >> nCols >> maxWalkDist >> nBuildProjs;
    for (int p = 0; p < nBuildProjs; p++) {
        inp >> type >> bRows >> bCols >> val;
        vector<vector<char>> plan(bRows, vector<char>(bCols, '.'));
        for (int row = 0; row < bRows; row++)
            for (int col = 0; col < bCols; col++)
                inp >> plan[row][col];
        projs.push_back(Project(getBuildingType(type), val, plan));

        // store all utilities
        if (type == 'U' && seenUtilities.find(val) == seenUtilities.end()) {
            allUtilities.push_back(val);
            seenUtilities.insert(val);
        }      
    }
    inp.close();
    return InputInfo(nRows, nCols, maxWalkDist, allUtilities);
}