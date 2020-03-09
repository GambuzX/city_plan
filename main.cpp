#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Building.h"

using namespace std;

int manhattanDistance(Building b1, Building b2);

int main() {

    vector<Building> buildings;
    unordered_map<int, Building*> buildingsHashMap;
    
    parseInput("input.txt", buildings);
    assignBuildingsIDs(buildings, buildingsHashMap);

    return 0;
}

void parseInput(string filename, vector<Building> & buildings) {
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
        buildings.push_back(Building(getBuildingType(type), val, plan));        
    }
    inp.close();
}

void assignBuildingsIDs(vector<Building> & buildings, unordered_map<int,Building*>& hashMap) {
    for(int i = 0; i < buildings.size(); i++)
        hashMap.insert(make_pair(buildings[i].getID(), &buildings[i]));
}

int manhattanDistance(Building b1, Building b2) {
    return 0;
}