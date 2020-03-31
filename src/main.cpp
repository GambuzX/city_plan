#include <iostream>
#include <vector>
#include <random>

#include "State.h"
#include "InputParse.h"
#include "IIAlgorithms.h"
#include "Genetic.h"
#include "Interface.h"
#include "CreateOutput.h"

#define NUM_MAX_ALGORITHMS NUM_MAX_OPTIONS - 1

using namespace std;

int main(int argc, char * argv[]) {
    srand(time(NULL));
    InputInfo globalInfo = parseInput(argv[1]);
    State localMaximum = hillClimbing(&globalInfo);

    return 0;
}