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

    if(argc > 3){
        cout << "ERROR: Program needs a maximum of 2 arguments." << endl;
        cout << "       city_plan [input_file] [output_file]" << endl;
        return -1;
    }
    if(argc > 1 && !fileExists(argv[1])){
        cout << "ERROR: Input file doesn't exist." << endl;
        return -1;
    }

    srand(time(NULL));
    
    drawLogo();

    bool exit = false;

    do{
        int option = menu();

        if(option < 1 || option > NUM_MAX_ALGORITHMS){
            return 0;
        }

        string fileName;
        if(argc > 1){
            fileName = argv[1];
        }else{
            fileName = chooseInputFileName();
        }

        InputInfo globalInfo = parseInput(fileName);

        State finalState;

        switch(option){
            case 1:{ /*HILL CLIMBING*/
                
                bool findBestNeighbour = chooseBestNeighbour();
                int maxSteps = chooseMaxSteps();

                finalState = hillClimbing(&globalInfo, maxSteps, findBestNeighbour);
                break;
            }
            case 2:{ /*SIMULATED ANNEALING*/
                
                int maxSteps = chooseMaxSteps();

                double temperature = chooseMaxTemperature();

                finalState = simulatedAnnealing(&globalInfo, maxSteps, temperature);
                break;
            }
            case 3:{ /*GENETIC ALGORITHM*/

                SelectionAlgorithm selectionAlgorithm = chooseSelectionAlgorithm();
                BreedingAlgorithm breedingAlgorithm = chooseBreedingAlgorithm();
                int populationSize = choosePopulationSize();
                int generations = chooseGenerations();
                double mutationChance = chooseMutationChance();
                int np = 3;

                if(selectionAlgorithm == SelectionAlgorithm::Tournament){
                    np = chooseNP(populationSize);
                }

                finalState = geneticAlgorithm(
                    &globalInfo,
                    selectionAlgorithm, 
                    breedingAlgorithm, 
                    populationSize, 
                    generations, 
                    mutationChance, 
                    np
                );

                break;
            }
            default:{
                return 0;
            }
        }

        if(argc > 2){
            fileName = argv[2];
        }else{
            fileName = chooseOutputFileName();
        }

        createOutput(finalState, fileName);
        
    }while(!exit);

    
    return 0;
}