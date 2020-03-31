#include <iostream>
#include "Genetic.h"
#include "BuildOperator.h"
#include "BuildRandomOperator.h"
#include "RemoveOperator.h"
#include "RemoveRandomOperator.h"
#include "ReplaceOperator.h"
#include "ReplaceRandomOperator.h"

#include <set>
#include <algorithm>

using namespace std;

State breed(const State &s1, const State &s2, BreedingAlgorithm breedingAlg);
State breedVertically(const State & s1, const State &s2);
State breedHorizontally(const State & s1, const State &s2);
State breedInTurns(const State &s1, const State &s2);
State mutate(State & s);
State bestIndividual(State * population, int populationSize);
pair<int, int> bestValue(State * population, int populationSize);
int select(State * population, int populationSize, int np, SelectionAlgorithm selAlg);
int tournamentSelection(State * population, int populationSize, int np);
int roulleteSelection(State * population, int populationSize);

int randPercent() { return (rand() % 100) + 1; }
double randZeroOne() { return (double) rand() / RAND_MAX; }

void printGenerationProgress(int gen, int total, string msg) {
    cout << "\r                                                   " << std::flush;
    cout << "\r[" << gen << "/" << total << "] " << msg << std::flush;
}

State geneticAlgorithm(InputInfo * globalInfo, SelectionAlgorithm selecAlg, BreedingAlgorithm breedingAlg, int populationSize, int generations, double mutationChance, int np) {

    cout << "[+] Starting genetic algorithm" << endl;

    cout << "[+] Generating initial population of size " << populationSize << endl;
    State * population = generatePopulationPtr(globalInfo, populationSize);
    pair<int,int> startBest = bestValue(population, populationSize);
    cout << "[!] Initial population generated" << endl;
    cout << "[!] Fittest starting individual is: (" <<  startBest.first << ", " << startBest.second << ")" << endl << endl;

    for(int g = 0; g < generations; g++) {
        cout << "[+] Starting generation " << g+1 << "/" << generations << endl;
        State * newPopulation = new State[populationSize];
        for (int p = 0; p < populationSize; p++) {
            // select parents to breed
            printGenerationProgress(p+1, populationSize, "Selecting parents");
            const State & p1 = population[select(population, populationSize, np, selecAlg)];
            const State & p2 = population[select(population, populationSize, np, selecAlg)];

            // child
            printGenerationProgress(p+1, populationSize, "Breeding parents");
            newPopulation[p] = breed(p1, p2, breedingAlg);

            // mutation
            if(randPercent() <= mutationChance) {
                printGenerationProgress(p+1, populationSize, "Mutating child");
                mutate(newPopulation[p]);
            }
        }
        printGenerationProgress(populationSize, populationSize, "Done");
        cout << endl;

        // update population
        delete[] population;
        population = newPopulation;
        
        // select and display the best from this generation
        pair<int,int> genBest = bestValue(population, populationSize);
        cout << "[!] Generation " << g+1 << " best individual was: (" <<  genBest.first << ", " << genBest.second << ")" << endl << endl;
    }

    // select best from final population
    State best = bestIndividual(population, populationSize);
    cout << "[!] Genetic algorithm finished" << endl;
    cout << "[!] Best individual found was: (" <<  best.value() << ", " << best.emptyCount() << ")" << endl << endl;

    delete[] population;
    return best;
}

State breed(const State & s1, const State & s2, BreedingAlgorithm breedingAlg) {
    switch (breedingAlg)
    {
        case BreedingAlgorithm::VerticalDivision:
            return breedVertically(s1, s2);
        case BreedingAlgorithm::HorizontalDivision:
            return breedHorizontally(s1, s2);
        case BreedingAlgorithm::SelectionInTurns:
            return breedInTurns(s1, s2);
        default:
            return breedVertically(s1, s2);
    }
}

State breedVertically(const State & s1, const State &s2) {
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
        else if(b.belowRow(division)) s2s1.push_back(&b);
    }
    for(uint id : s2IDs) {
        const Building & b = s2Buildings.at(id);
        if(b.aboveRow(division)) s2s1.push_back(&b);
        else if(b.belowRow(division)) s1s2.push_back(&b);
    }

    State option1(s1.getGlobalInfo()), option2(s1.getGlobalInfo());
    for (const Building * b : s1s2) option1.createBuilding(b->getProject(), b->getRow(), b->getCol());
    for (const Building * b : s2s1) option2.createBuilding(b->getProject(), b->getRow(), b->getCol());

    return (State::betterState(option1, option2) ? option2 : option1);
}

State breedHorizontally(const State & s1, const State &s2) {
    int nCols = s1.getGlobalInfo()->cols;
    int division = rand() % nCols;

    const unordered_map<uint, Building> & s1Buildings = s1.getBuildings();
    const unordered_map<uint, Building> & s2Buildings = s2.getBuildings();
    const vector<uint> & s1IDs = s1.getAllBuildingsIDs();
    const vector<uint> & s2IDs = s2.getAllBuildingsIDs();

    // s1 on top s2 on bottom, and vice versa
    vector<const Building*> s1s2, s2s1;
    for(uint id : s1IDs) {
        const Building & b = s1Buildings.at(id);
        if(b.beforeCol(division)) s1s2.push_back(&b);
        else if(b.afterCol(division)) s2s1.push_back(&b);
    }
    for(uint id : s2IDs) {
        const Building & b = s2Buildings.at(id);
        if(b.beforeCol(division)) s2s1.push_back(&b);
        else if(b.afterCol(division)) s1s2.push_back(&b);
    }

    State option1(s1.getGlobalInfo()), option2(s1.getGlobalInfo());
    for (const Building * b : s1s2) option1.createBuilding(b->getProject(), b->getRow(), b->getCol());
    for (const Building * b : s2s1) option2.createBuilding(b->getProject(), b->getRow(), b->getCol());

    return (State::betterState(option1, option2) ? option2 : option1);
}

State breedInTurns(const State &s1, const State &s2) {
    
    const unordered_map<uint, Building> &s1Buildings = s1.getBuildings();
    const unordered_map<uint, Building> &s2Buildings = s2.getBuildings();
    vector<vector<uint>> s1Map = s1.getCityMap();
    vector<vector<uint>> s2Map = s2.getCityMap();
    bMatrix filledPos;
    State newState(s1.getGlobalInfo());
    bool s1Turn = true;

    for(size_t row = 0; row < s1Map.size(); row++) {
        for (size_t col = 0; col < s2Map.size(); col++) {
            if(s1Turn && s1Map[row][col] != 0) {
                const Building & b = s1Buildings.at(s1Map[row][col]);
                if(!newState.canCreateBuilding(b.getProject(), b.getRow(), b.getCol(), &filledPos)) continue;
                
                // can create building
                newState.createBuilding(b.getProject(), b.getRow(), b.getCol());
                updateUsedMap(filledPos, b.getProject(), b.getRow(), b.getCol(), true);
                s1Turn = false;
            }
            else if(!s1Turn && s2Map[row][col] != 0) {
                const Building & b = s2Buildings.at(s2Map[row][col]);
                if(!newState.canCreateBuilding(b.getProject(), b.getRow(), b.getCol(), &filledPos)) continue;
                
                // can create building
                newState.createBuilding(b.getProject(), b.getRow(), b.getCol());
                updateUsedMap(filledPos, b.getProject(), b.getRow(), b.getCol(), true);
                s1Turn = true;
            }
        }
    }
    
    return newState;
}

State mutate(State & s) {
    vector<Operator*> operators{
        new BuildOperator(s),
        new ReplaceOperator(s),
        new RemoveOperator(s),
        new BuildRandomOperator(s),
        new RemoveRandomOperator(s),
        new ReplaceRandomOperator(s)
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

pair<int, int> bestValue(State * population, int populationSize) {
    int bVal = population[0].value(), bEmpty = population[0].emptyCount();
    for(int i = 1; i < populationSize; i++) {
        int nVal = population[i].value();
        int nEmpty = population[i].emptyCount();
        if (State::betterState(bVal, bEmpty, nVal, nEmpty)) {
            bVal = nVal;
            bEmpty = nEmpty;
        }
    }
    return make_pair(bVal, bEmpty);
}


int select(State * population, int populationSize, int np, SelectionAlgorithm selAlg) {
    switch(selAlg) {
        case SelectionAlgorithm::Roullete:
            return roulleteSelection(population, populationSize);
        case SelectionAlgorithm::Tournament:
            return tournamentSelection(population, populationSize, np);
        default:
            return roulleteSelection(population, populationSize);
    };
}

int tournamentSelection(State * population, int populationSize, int np) {
    // choose participants
    np = min(np, populationSize);
    set<int> choosen;
    while(choosen.size() < (size_t) np) {
        int choice = random() % populationSize;
        choosen.insert(choice);
    }

    // choose best option among participants
    int bI, bVal = -1, bEmpty = -1;
    for(int choice : choosen) {
        const State & s = population[choice];
        int sVal = s.value(), sEmpty = s.emptyCount();
        if(State::betterState(bVal, bEmpty, sVal, sEmpty)) {
            bI = choice;
            bVal = sVal;
            bEmpty = sEmpty;
        }
    }
    return bI;
}

int roulleteSelection(State * population, int populationSize) {
    
    int total = 0;
    vector<double> cumulativeValues;
    for (int i = 0; i < populationSize; i++) {
        const State & s = population[i];
        int value = s.value();
        total += value;
        cumulativeValues.push_back(total);
    }

    double choice = randZeroOne();
    for (size_t i = 0; i < cumulativeValues.size(); i++) {
        cumulativeValues[i] = cumulativeValues[i] * 1.0 / total;

        if(i == 0 && choice <= cumulativeValues[i]) return i;
        if(cumulativeValues[i-1] < choice && choice <= cumulativeValues[i]) return i;
    }

    // error, return first
    return 0;
}