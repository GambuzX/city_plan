#include <vector>
#include <iostream>
#include <climits>
#include <tuple>
#include <random>
#include <time.h>

#include "IIAlgorithms.h"
#include "Util.h"
#include "BuildOperator.h"
#include "BuildRandomOperator.h"
#include "BuildRandomPositionOperator.h"
#include "BuildTabuOperator.h"
#include "RemoveRandomOperator.h"
#include "RemoveTabuOperator.h"
#include "RemoveOperator.h"
#include "ReplaceOperator.h"
#include "ReplaceRandomOperator.h"
#include "ReplaceTabuOperator.h"

using namespace std;

State hillClimbing(InputInfo * info, int maxSteps, OperatorsAccuracy accuracy) {

    cout << "[+] Starting hill climbing" << endl;
    cout << "[+] Generating initial state" << endl;
    State currentState = generateState(info);
    int previousValue, currentValue, previousEmpty, currentEmpty;   
    previousValue = currentValue = currentState.value();
    previousEmpty = currentEmpty = currentState.emptyCount();

    cout << "[!] Initial state (value, emptyCells): (" << currentValue << ", " << currentEmpty << ")" << endl << endl;
    int step = 0;
    while(maxSteps < 0 || step++ < maxSteps) {
            
        cout << "[" << step+1 << "/" << maxSteps << "] Searching for neighbour" << endl;
        State neighbour = higherValueNeighbour(currentState, accuracy);
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

    cout << "[!] Reached last step" << endl;
    cout << "[!] Best value found: (" << previousValue << ", " << previousEmpty << ")" << endl;
    
    return currentState;
}

bool getAccuracyOperators(const State & state, OperatorsAccuracy accuracy, vector<Operator*> & operators) {
    bool findBest = false;
    switch(accuracy) {
        case OperatorsAccuracy::LOW:
            operators = {
                new BuildRandomPositionOperator(state),
                new ReplaceOperator(state),
                new RemoveOperator(state)
            };
            break;
        case OperatorsAccuracy::MEDIUM:
            operators = {
                new BuildOperator(state),
                new ReplaceOperator(state),
                new RemoveOperator(state)
            };
            break;
        case OperatorsAccuracy::HIGH:
            operators = {
                new BuildOperator(state),
                new ReplaceOperator(state),
                new RemoveOperator(state)
            };
            findBest = true;
            break;
    }
    return findBest;
}


State higherValueNeighbour(const State & state, OperatorsAccuracy accuracy){
    // operators to apply, in sequence
    vector<Operator*> operators;
    bool findBest = getAccuracyOperators(state, accuracy, operators);    

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

    for(size_t i = 0; i < operators.size(); i++) delete operators[i];
    return bestState;
}

State randomNeighbour(const State & state){
    // operators to apply, in sequence
    vector<Operator*> operators{
        new BuildRandomOperator(state),
        new ReplaceRandomOperator(state),
        new RemoveRandomOperator(state)
    };

    int prevValue = state.value();
    uint prevEmpty = state.emptyCount(); //used to check if the operators were successful

    State newState = state;

    size_t randomOffset = getRandomValue() % operators.size();//Starting point for the operator list iteration

    for (uint i = 0; i < operators.size(); ++i) {
        size_t operatorIndex = (randomOffset +  i) % operators.size();
        Operator *op = operators[operatorIndex];
        cout << "[+] Applying " << op->getName() << " operator" << endl;
        newState = op->apply(false);
        int newVal = newState.value();
        if(prevValue != newVal || prevEmpty != newState.emptyCount()) {
            break;
        }
    }

    for(size_t i = 0; i < operators.size(); i++) delete operators[i];

    return newState;
}

State simulatedAnnealing(InputInfo * info, int maxSteps, double maxTemperature){

    cout << "[+] Starting Simulated Annealing" << endl;
    cout << "[+] Generating initial state" << endl;

    // current state
    State currentState = generateState(info);
    int currentValue = currentState.value();
    int currentEmpty = currentState.emptyCount();
    cout << "[!] Initial state (value, emptyCells): (" << currentValue << ", " << currentEmpty << ")" << endl << endl;

    // best state so far
    State bestState = currentState;
    int bestValue = currentValue;
    int bestEmpty = currentEmpty;

    for (int s = 1; s < maxSteps; s++) {
        cout << "[" << s << "/" << maxSteps << "] Searching for neighbour" << endl;
        double temperature = maxTemperature * ((double)s / maxSteps);

        State neighbour = randomNeighbour(currentState);
        int neighbourValue = neighbour.value();
        int neighbourEmpty = neighbour.emptyCount();
        
        double choice = ((double)getRandomValue()) / RAND_MAX;   
        double delta = neighbourValue - currentValue;
        double acceptProb = exp(delta / temperature);
        cout << "[!] Evaluating neighbour. Delta=" << delta << ", Probability=" << acceptProb << ", Choice=" << choice << endl;
        if(State::betterState(currentValue, currentEmpty, neighbourValue, neighbourEmpty) || choice < acceptProb) {
            currentState = neighbour;
            currentValue = neighbourValue;
            currentEmpty = neighbourEmpty;
            cout << "[!] Choose neighbour: (" << currentValue << ", " << currentEmpty << ")" << endl << endl;

            if(State::betterState(bestValue, bestEmpty, neighbourValue, neighbourEmpty)){
                bestState = currentState;
                bestValue = currentValue;
                bestEmpty = neighbourEmpty;
            }
        }
        else cout << "[!] Skipped neighbour" << endl << endl;
    }

    cout << "[!] Reached last step" << endl;
    cout << "[!] Best value found: (" << bestValue << ", " << bestEmpty << ")" << endl;
    return bestState;
}

State tabuNeighbour(const State & state){
    // operators to apply, in sequence
    vector<Operator*> operators{
        new BuildTabuOperator(state),
        new ReplaceTabuOperator(state),
        new RemoveTabuOperator(state)
    };

    int prevValue = state.value();
    uint prevEmpty = state.emptyCount(); //used to check if the operators were successful

    State newState = state;

    size_t randomOffset = getRandomValue() % operators.size();//Starting point for the operator list iteration

    for (uint i = 0; i < operators.size(); ++i) {
        size_t operatorIndex = (randomOffset +  i) % operators.size();
        Operator *op = operators[operatorIndex];
        cout << "[+] Applying " << op->getName() << " operator" << endl;
        newState = op->apply(false);
        int newVal = newState.value();
        if(prevValue != newVal || prevEmpty != newState.emptyCount()) {
            break;
        }
    }

    for(size_t i = 0; i < operators.size(); i++) delete operators[i];

    return newState;
}

State tabuSearch(InputInfo * info, int maxSteps, int maxTemperature, int maxTabus){
    State currentState = generateState(info);
    State bestState = currentState;

    TabuOperator::setMaxTabus(maxTabus);

    int currentValue = currentState.value();
    int bestValue = currentValue;
    int currentEmpty = currentState.emptyCount();
    cout << "[+] Starting state: " << currentValue << endl << endl;

    for (int s = 1; s < maxSteps; s++) {
        double temperature = maxTemperature * ((double)s / maxSteps);

        cout << "[+] Searching for neighbour" << endl;
        State neighbour = tabuNeighbour(currentState);
        int neighbourValue = neighbour.value();
        int neighbourEmpty = neighbour.emptyCount();

        if(currentValue != neighbourValue || currentEmpty != neighbourEmpty) {
            break;
        }
        
        double choice = ((double)getRandomValue()) / RAND_MAX;   
        double delta = neighbourValue - currentValue;
        double acceptProb = exp(delta / temperature);
        cout << "Evaluating neighbour choice = " << choice << ", prob = " << acceptProb << ", delta = " << delta << endl;
        if(delta > 0 || choice < acceptProb) {
            currentValue = neighbourValue;
            currentState = neighbour;
            cout << "[+] Found neighbour: " << currentValue << endl << endl;
        }    

        if(currentValue > bestValue){
            bestState = currentState;
            bestValue = currentValue;
        }
    }

    TabuOperator::clearTabus();
    
    cout << "Ended search with a value of " << bestValue << endl;
    return bestState;
}