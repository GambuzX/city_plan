#include "Genetic.h"
#include "BuildOperator.h"
#include "RemoveOperator.h"
#include "ReplaceOperator.h"

using namespace std;

State breed(const State &s1, const State &s2);
State mutate(State & s);
State bestIndividual(State * population, int populationSize);

int randPercent() { return (rand() % 100) + 1; }

State geneticAlgorithm(InputInfo * globalInfo, int populationSize, int generations, double mutationChance) {

    State * population = generatePopulationPtr(globalInfo, populationSize);
    for(int g = 0; g < generations; g++) {
        State * newPopulation = new State[populationSize];
        for (int p = 0; p < populationSize; p++) {
            // select parents to breed
            const State & p1 = population[0]; // implement selection algorithm
            const State & p2 = population[1];

            // child
            newPopulation[p] = breed(p1, p2);

            // mutation
            if(randPercent() <= mutationChance) mutate(newPopulation[p]);
        }
        delete[] population;
        population = newPopulation;
    }

    // select best from final population
    State best = bestIndividual(population, populationSize);
    delete[] population;
    return best;
}

State breed(const State & s1, const State &s2) {
    int nRows = s1.getGlobalInfo()->rows;
    int division = rand() % nRows;

    const unordered_map<uint, Building> & s1Buildings = s1.getBuildings();
    const unordered_map<uint, Building> & s2Buildings = s2.getBuildings();
    const list<uint> & s1IDs = s1.getAllBuildingsIDs();
    const list<uint> & s2IDs = s2.getAllBuildingsIDs();

    // s1 on top s2 on bottom, and vice versa
    list<const Building*> s1s2, s2s1;
    for(uint id : s1IDs) {
        const Building & b = s1Buildings.at(id);
        if(b.aboveRow(division)) s1s2.push_back(&b);
        if(b.belowRow(division)) s2s1.push_back(&b);
    }
    for(uint id : s2IDs) {
        const Building & b = s2Buildings.at(id);
        if(b.aboveRow(division)) s2s1.push_back(&b);
        if(b.belowRow(division)) s1s2.push_back(&b);
    }

    State option1(s1.getGlobalInfo()), option2(s1.getGlobalInfo());
    for (const Building * b : s1s2) option1.createBuilding(b->getProject(), b->getRow(), b->getCol());
    for (const Building * b : s2s1) option2.createBuilding(b->getProject(), b->getRow(), b->getCol());

    return (State::betterState(option1, option2) ? option2 : option1);
}

State mutate(State & s) {
    return s;
}

State bestIndividual(State * population, int populationSize) {
    int bI = 0, bVal = population[0].value(), bEmpty = population[0].emptyCount();
    for(int i = 1; i < populationSize; i++) {
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