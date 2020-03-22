#include <iostream>
#include <vector>

#include "State.h"
#include "InputParse.h"
#include "IIAlgorithms.h"

using namespace std;

int main() {
    InputInfo globalInfo = parseInput("inputs/example.txt");
    State initialState = State(&globalInfo);

    State localMaximum = hillClimbing(initialState);
    
    return 0;
}