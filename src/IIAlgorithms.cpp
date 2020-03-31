#include <vector>
#include <iostream>
#include <climits>
#include <tuple>
#include <random>
#include <time.h>

#include "IIAlgorithms.h"
#include "Util.h"
#include "BuildOperator.h"
#include "RemoveOperator.h"
#include "ReplaceOperator.h"
#include "BuildRandomOperator.h"
#include "RemoveRandomOperator.h"
#include "ReplaceRandomOperator.h"

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
            cout << "Found different" << endl;
            break;
        }
    }

    for(size_t i = 0; i < operators.size(); i++) delete operators[i];

    return newState;
}

State simulatedAnnealing(InputInfo * info, int maxSteps, double maxTemperature){

    State currentState = generateState(info);
    int currentValue = currentState.value();
    cout << "[+] Starting state: " << currentValue << endl << endl;

    for (int s = 1; s < maxSteps; s++) {
        double temperature = maxTemperature * ((double)s / maxSteps);

        cout << "[+] Searching for neighbour" << endl;
        State neighbour = randomNeighbour(currentState);
        int neighbourValue = neighbour.value();
        
        double choice = ((double)getRandomValue()) / RAND_MAX;   
        double delta = neighbourValue - currentValue;
        double acceptProb = exp(delta / temperature);
        cout << "Evaluating neighbour choice = " << choice << ", prob = " << acceptProb << ", delta = " << delta << endl;
        if(delta > 0 || choice < acceptProb) {
            currentValue = neighbourValue;
            currentState = neighbour;
            cout << "[+] Found neighbour: " << currentValue << endl << endl;
        }    
    }
    
    return currentState;
}