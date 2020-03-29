#include <iostream>
#include "Genetic.h"
#include "BuildOperator.h"
#include "BuildAnyOperator.h"
#include "RemoveOperator.h"
#include "RemoveRandomOperator.h"
#include "ReplaceOperator.h"
#include "ReplaceAnyOperator.h"

using namespace std;

State breed(const State &s1, const State &s2);
State mutate(State & s);
State bestIndividual(State * population, int populationSize);

int randPercent() { return (rand() % 100) + 1; }

void printGenerationProgress(int gen, int total, string msg) {
    cout << "\r                                                   " << std::flush;
    cout << "\r[" << gen << "/" << total << "] " << msg << std::flush;
}

State geneticAlgorithm(InputInfo * globalInfo, int populationSize, int generations, double mutationChance) {

    cout << "[+] Starting genetic algorithm" << endl;

    cout << "[+] Generating initial population of size " << populationSize << endl;
    State * population = generatePopulationPtr(globalInfo, populationSize);
    cout << "[!] Initial population generated" << endl << endl;

    for(int i = 0 ; i < populationSize; i++) cout << population[i].value() << " ";
    cout << endl;

    for(int g = 0; g < generations; g++) {
        cout << "[+] Starting generation " << g+1 << "/" << generations << endl;
        State * newPopulation = new State[populationSize];
        for (int p = 0; p < populationSize; p++) {
            // select parents to breed
            printGenerationProgress(p+1, populationSize, "Selecting parents");
            const State & p1 = population[0]; // implement selection algorithm
            const State & p2 = population[1];

            // child
            printGenerationProgress(p+1, populationSize, "Breeding parents");
            newPopulation[p] = breed(p1, p2);

            // mutation
            if(randPercent() <= mutationChance) {
                printGenerationProgress(p+1, populationSize, "Mutating child");
                mutate(newPopulation[p]);
            }
        }
        delete[] population;
        population = newPopulation;
        cout << endl << endl;
    }

    // select best from final population
    State best = bestIndividual(population, populationSize);
    for(int i = 0 ; i < populationSize; i++) cout << population[i].value() << " ";
    cout << endl;
    delete[] population;
    return best;
}

State breed(const State & s1, const State &s2) {
    int nRows = s1.getGlobalInfo()->rows;
    int division = rand() % nRows;

    const unordered_map<uint, Building> & s1Buildings = s1.getBuildings();
    const unordered_map<uint, Building> & s2Buildings = s2.getBuildings();
    const vector<uint> & s1IDs = s1.getAllBuildingsIDs();
    const vector<uint> & s2IDs = s2.getAllBuildingsIDs();

    // s1 on top s2 on bottom, and vice versa
    vector<const Building*> s1s2, s2s1;
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
    vector<Operator*> operators{
        new BuildOperator(s),
        new ReplaceOperator(s),
        new RemoveOperator(s),
        new BuildAnyOperator(s),
        new RemoveRandomOperator(s),
        new ReplaceAnyOperator(s)
    };
    State mutated = operators[rand() % operators.size()]->apply();
    for (size_t i = 0; i < operators.size(); i++) delete operators[i];
    return mutated;
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