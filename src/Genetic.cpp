#include "Genetic.h"

using namespace std;

State breed(const State &s1, const State &s2);
tuple<vector<vector<uint>>, vector<vector<uint>>, uint> divideState(const State &s, const uint &num_rows, const uint &mid_row, const uint &num_cols);
State mutate(State & s);

int randPercent() { return (rand() % 100) + 1; }

State geneticAlgorithm(InputInfo * globalInfo, int populationSize, int generations, double mutationChance) {

    vector<State> population = generatePopulation(globalInfo, populationSize); //switch to pointers
    for(int g = 0; g < generations; g++) {
        vector<State> newPopulation(population.size());
        for (int p = 0; p < population.size(); p++) {
            // select parents to breed
            const State & p1 = population[0]; // implement selection algorithm
            const State & p2 = population[1];

            // child
            newPopulation[p] = breed(p1, p2);

            // mutation
            if(randPercent() <= mutationChance) mutate(newPopulation[p]);
        }
        population = newPopulation;
    }
    return bestIndividual(population);
}

State breed(const State &s1, const State &s2){
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

State mutate(State & s) {
    return s;
}

State bestIndividual(const vector<State> & population) {
    int bI = 0, bVal = population[0].value(), bEmpty = population[0].emptyCount();
    for(int i = 1; i < population.size(); i++) {
        int nVal = population[i].value();
        int nEmpty = population[i].emptyCount();
        if (State::betterState(bVal, bEmpty, nVal, nEmpty)) {
            bI = i;
            bVal = nVal;
            bEmpty = nVal;
        }
    }
    return population[bI];
}