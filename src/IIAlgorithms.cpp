#include <vector>
#include <iostream>
#include <climits>

#include "IIAlgorithms.h"

using namespace std;

State hillClimbing(const State & initialState) {

    State currentState = initialState; // random first choice, cuz no points. check both values at different steps
    int previousValue, currentValue;
    previousValue = currentValue = currentState.value();

    cout << "[+] Starting hill climbing with value " << currentValue << endl;
    while(1) {
        State neighbour = getHigherValueNeighbour(currentState);
        currentValue = neighbour.value();
        cout << "[+] Found neighbour: " << currentValue << endl;

        if (currentValue < previousValue) {     
            cout << "[+] Reached local maximum" << endl;
            break;
        }
        currentState = neighbour;
        previousValue = currentValue;
    }
    
    return currentState;
}


State getHigherValueNeighbour(const State & state){
    const vector<Project> & projects = state.getGlobalInfo()->bProjects;
    const vector<vector<int>> & map = state.getCityMap();
    int initialStateValue = state.value();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(int p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(currProject->fits(map, row, col)){

                    // create building and check its value
                    State newState = state;
                    newState.addBuilding(currProject, row, col);
                    int newStateValue = newState.value();
                    
                    // found better state
                    if(newStateValue > initialStateValue){
                        return newState;
                    }

                }
            }
        }
    }

    // did not find better neighbour
    return state;
}

State getHighestValueNeighbor(const State & state){
    const vector<Project> & projects = state.getGlobalInfo()->bProjects;
    State bestState = state;
    int bestValue = INT_MIN;

    const vector<vector<int>> & map = state.getCityMap();

    for(size_t row = 0; row < map.size(); row++){
        for(size_t col = 0; col < map[row].size(); col++){
            // check if empty
            if(map[row][col] != 0)
                continue;
            
            // try to build all projects
            for(int p = 0; p < projects.size(); p++) {
                Project * currProject = (Project *) &projects[p];

                if(currProject->fits(map, row, col)){

                    // create building and check its value
                    State newState = state;
                    newState.addBuilding(currProject, row, col);
                    int newStateValue = newState.value();
                    
                    if(newStateValue > bestValue){
                        bestState = newState;
                        bestValue = newStateValue;
                    }

                }
            }
        }
    }

    return bestState;
}