#include "CreateOutput.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void createOutput(const State &finalState, string outputFileName){
    cout << "CREATING OUTPUT..." << endl;

    if(outputFileName.length() == 0){
        cout << "Using default output file: outputs/default.txt" << endl;
        outputFileName = "outputs/default.txt";
    }
    
    ofstream f(outputFileName);

    unordered_map<uint, Building> buildings = finalState.getBuildings();

    f << buildings.size() << endl;

    for(auto it = buildings.begin(); it != buildings.end(); it++){
        f << (*it).second.getProject()->getID() << " " 
          << (*it).second.getRow() << " " 
          << (*it).second.getCol() << endl;
    }

    f.close();
}