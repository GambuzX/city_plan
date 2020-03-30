#include <vector>
#include <iostream>
#include <climits>
#include <tuple>
#include <random>
#include <time.h>

#include "IIAlgorithms.h"
#include "util.h"
#include "BuildOperator.h"
#include "RemoveOperator.h"
#include "ReplaceOperator.h"

using namespace std;

State hillClimbing(InputInfo * info) { // order buildings by occupied size / value rating ??

    cout << "[+] Starting hill climbing" << endl;

    cout << "[+] Generating initial state" << endl;
    State currentState = generateState(info);
    int previousValue, currentValue, previousEmpty, currentEmpty;   
    previousValue = currentValue = currentState.value();
    previousEmpty = currentEmpty = currentState.emptyCount();

    cout << "[!] Initial state (value, emptyCells): (" << currentValue << ", " << currentEmpty << ")" << endl << endl;
    while(1) {
            
        cout << "[+] Searching for neighbour" << endl;
        State neighbour = higherValueNeighbour(currentState, false);
        currentValue = neighbour.value();
        currentEmpty = neighbour.emptyCount();

        if (!State::betterState(previousValue, previousEmpty, currentValue, currentEmpty)) {
            cout << "[!] Could not find a better neighbour" << endl;
            cout << "[!] Reached local maximum: (" << previousValue << ", " << previousEmpty << ")" << endl;
            break;
        }

        cout << "[!] Found neighbour: (" << currentValue << ", " << currentEmpty << ")" << endl << endl;
        currentState = neighbour;
        previousValue = currentValue;
        previousEmpty = currentEmpty;
    }
    
    return currentState;
}


State higherValueNeighbour(const State & state, bool findBest){
    // operators to apply, in sequence
    vector<Operator*> operators{
        new BuildOperator(state),
        new ReplaceOperator(state),
        new RemoveOperator(state)
    };

    int bestValue, bestEmpty, newVal, newEmpty;
    State bestState = state;
    bestValue = state.value();
    bestEmpty = state.emptyCount();

    for (Operator * op : operators) {
        cout << "[+] Applying " << op->getName() << " operator" << endl;
        State newState = op->apply(findBest);
        newVal = newState.value();
        newEmpty = newState.emptyCount();
        if(State::betterState(bestValue, bestEmpty, newVal, newEmpty)) {
            cout << "[!] Found better state by " << op->getActionName() << ": (" << newVal << ", " << newEmpty << ")" << endl;
            if(!findBest) return newState;
            bestState = newState;
            bestValue = newVal;
            bestEmpty = newEmpty;
        }
    }

    return bestState;
}

State breeding(const State &s1, const State &s2){
    uint num_rows = s1.getGlobalInfo()->rows;
    uint mid_row = num_rows / 2;
    uint num_cols = s1.getGlobalInfo()->cols;

    tuple<vector<vector<uint>>, vector<vector<uint>>, uint> s1_results = divideState(s1, num_rows, mid_row, num_cols);
    tuple<vector<vector<uint>>, vector<vector<uint>>, uint> s2_results = divideState(s2, num_rows, mid_row, num_cols);
    
    State new_s1 = State(get<0>(s1_results), s1.getBuildings(), get<2>(s1_results), get<1>(s2_results), s2.getBuildings(), s1.getGlobalInfo());
    State new_s2 = State(get<0>(s2_results), s2.getBuildings(), get<2>(s2_results), get<1>(s1_results), s1.getBuildings(), s2.getGlobalInfo());
    
    int v1 = new_s1.value();
    int v2 = new_s2.value();
    if(v1 > v2)
        return new_s1;
    if(v2 > v1)
        return new_s2;

    if(new_s1.emptyCount() >= new_s2.emptyCount())
        return new_s1;
    
    return new_s2;
}

tuple<vector<vector<uint>>, vector<vector<uint>>, uint> divideState(const State &s, const uint &num_rows, const uint &mid_row, const uint &num_cols){
    vector<uint> buildingIDs = s.getAllBuildingsIDs();
    unordered_map<uint, Building> buildings = s.getBuildings();

    vector<vector<uint>> top = vector<vector<uint>>(mid_row, vector<uint>(num_cols, 0));
    vector<vector<uint>> bottom = vector<vector<uint>>(num_rows-mid_row, vector<uint>(num_cols, 0));

    uint top_max_id = 0;

    for(size_t i = 0; i < buildingIDs.size(); i++){
        Building b = buildings.at(buildingIDs[i]);

        if(b.getRow() < (int)mid_row){
            if(b.getRow() + b.getProject()->getPlan().size() <= mid_row){
                for(size_t j = 0; j < b.getProject()->getPlan().size(); j++){
                    for(size_t k = 0; k < b.getProject()->getPlan()[0].size(); k++){
                        top[b.getRow() + j][b.getCol() + k] = b.getProject()->getID();
                    }
                }
                if(buildingIDs[i] > top_max_id)
                    top_max_id = buildingIDs[i];
            }else
                continue;
            
        }else{
            for(size_t j = 0; j < b.getProject()->getPlan().size(); j++){
                for(size_t k = 0; k < b.getProject()->getPlan()[0].size(); k++){
                    bottom[b.getRow() + j - mid_row][b.getCol() + k] = b.getProject()->getID();
                }
            }
        }
    } 

    return make_tuple(top, bottom, top_max_id);
}

State generateState(InputInfo *globalInfo){
    vector<Project> &projs = globalInfo->bProjects;

    State s(globalInfo);

    bMatrix used(globalInfo->rows, vector<bool>(globalInfo->cols, false));
    int col_inc = 1;
    for(int row = 0; row < globalInfo->rows; row += 1){
        for(int col = 0; col < globalInfo->cols; col += col_inc){

            if(used[row][col]) continue;
            col_inc = 1;

            Project & p = projs[rand() % projs.size()];

            if(s.canCreateBuilding(&p, row, col, &used)){
                s.createBuilding(&p, row, col);
                updateUsedMap(used, &p, row, col, true);
                col_inc = p.getPlan()[0].size();
            }
        }
    }

    return s;
}

vector<State> generatePopulation(InputInfo *global_info, int populationSize){
    vector<State> population(populationSize);

    for(int i = 0; i < populationSize; i++){
        population[i] = generateState(global_info);
    }

    return population;
}

State randomNeighbour(const State & state){
    // operators to apply, in sequence
    vector<Operator*> operators{
        new BuildOperator(state),
        new ReplaceOperator(state),
        new RemoveOperator(state)
    };

    int prevValue = state.value();

    State newState = state;

    for (Operator * op : operators) {
        cout << "[+] Applying " << op->getName() << " operator" << endl;
        State newState = op->apply(false);
        int newVal = newState.value();
        if(prevValue != newVal) {
            break;
        }
    }

    return newState;
}

State simulatedAnnealing(const State & initialState, int maxSteps, int temperature){
    //initialState.printMap();
    State currentState = initialState;
    int currentValue = currentState.value();
    int stepCount = 0;

    while(stepCount < maxSteps && temperature){
        if(!(stepCount % 100))
            --temperature; //To be revised

        cout << "[+] Searching for neighbour" << endl;
        State neighbour = randomNeighbour(currentState);
        int neighbourValue = neighbour.value();

        if (currentValue >= neighbourValue){  
            double randomValue = ((double)rand()) / ((double)RAND_MAX);   
            double delta = currentValue - neighbourValue; 

            if(delta / temperature <= randomValue) { //acceptance probability
                currentValue = neighbourValue;
                currentState = neighbour;
                cout << "[+] Found neighbour: " << currentValue << endl << endl;
            }
        }
        else{
            currentValue = neighbourValue;
            currentState = neighbour;
            cout << "[+] Found neighbour: " << currentValue << endl << endl;
        }        
    }
    
    return currentState;
}

