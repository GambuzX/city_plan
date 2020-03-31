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

using namespace std;

State hillClimbing(InputInfo * info, int maxSteps, bool findBestNeighbour) { // order buildings by occupied size / value rating ??

    cout << "[+] Starting hill climbing" << endl;

    cout << "[+] Generating initial state" << endl;
    State currentState = generateState(info);
    int previousValue, currentValue, previousEmpty, currentEmpty;   
    previousValue = currentValue = currentState.value();
    previousEmpty = currentEmpty = currentState.emptyCount();

    cout << "[!] Initial state (value, emptyCells): (" << currentValue << ", " << currentEmpty << ")" << endl << endl;
    int step = 0;
    while(maxSteps < 0 || step++ < maxSteps) {
            
        cout << "[+] Searching for neighbour" << endl;
        State neighbour = higherValueNeighbour(currentState, findBestNeighbour);
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

