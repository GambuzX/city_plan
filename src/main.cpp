#include <iostream>
#include <vector>
#include <random>

#include "State.h"
#include "InputParse.h"
#include "IIAlgorithms.h"
#include "Genetic.h"

using namespace std;

int main(int argc, char * argv[]) {
    srand(time(NULL));
    InputInfo globalInfo = parseInput(argv[1]);
    //State localMaximum = hillClimbing(&globalInfo);
    State res = geneticAlgorithm(&globalInfo);
    cout << res.value() << endl;
    return 0;
}