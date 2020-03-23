#include <iostream>
#include <vector>

#include "State.h"
#include "InputParse.h"
#include "IIAlgorithms.h"

using namespace std;

int main(int argc, char * argv[]) {
    InputInfo globalInfo = parseInput(argv[1]);
    State initialState = State(&globalInfo);

    State localMaximum = hillClimbing(initialState);
    
    return 0;
}