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
#include "BuildAnyOperator.h"
#include "RemoveRandomOperator.h"
#include "ReplaceAnyOperator.h"

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
        new BuildAnyOperator(state),
        new ReplaceAnyOperator(state),
        new RemoveRandomOperator(state)
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

State simulatedAnnealing(InputInfo * info, int maxSteps){

    State currentState = generateState(info);
    int currentValue = currentState.value();
    cout << "[+] Starting state: " << currentValue << endl << endl;
    int stepCount = 0;

    for (int s = 1; s <= maxSteps; s++) {
        double temperature = (double)maxSteps / s;

        cout << "[+] Searching for neighbour" << endl;
        State neighbour = randomNeighbour(currentState);
        int neighbourValue = neighbour.value();
        
        double choice = ((double)rand()) / RAND_MAX;   
        double delta = neighbourValue - currentValue;
        double acceptProb = exp(-delta / temperature);
        if(delta > 0 || choice > acceptProb) {
            currentValue = neighbourValue;
            currentState = neighbour;
            cout << "[+] Found neighbour: " << currentValue << endl << endl;
        }    
    }
    
    return currentState;
}