#include "CreateOutput.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void createOutput(const State &finalState, string outputFileName){
    cout << endl << "[+] CREATING OUTPUT" << endl;

    if(outputFileName.length() == 0){
        cout << "[!] Using default output file: outputs/default.txt" << endl;
        outputFileName = "outputs/default.txt";
    }
    else{
        cout << "[!] OUTPUT FILE NAME: " << outputFileName << endl;
    }
    cin.ignore();
    ofstream f;
    do{
        f.open(outputFileName);
        if(f.fail()){
            cout << "[?] ERROR: CAN'T OPEN FILE. MAKE SURE THE SPECIFIED DIRECTORY EXISTS." << endl;
            cout << "[?] PLEASE PRESS ANY KEY WHEN YOU CREATE THE DIRECTORY." << endl;
            cin.ignore();
        }
    }while(f.fail());

    unordered_map<uint, Building> buildings = finalState.getBuildings();

    f << buildings.size() << endl;

    for(auto it = buildings.begin(); it != buildings.end(); it++){
        f << (*it).second.getProject()->getID() << " " 
          << (*it).second.getRow() << " " 
          << (*it).second.getCol() << endl;
    }

    cout << "[!] DONE" << endl;

    f.close();
}