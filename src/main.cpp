#include <iostream>
#include <vector>
#include <random>

#include "State.h"
#include "InputParse.h"
#include "IIAlgorithms.h"
#include "Interface.h"

using namespace std;

int main(int argc, char * argv[]) {

    if(argc > 3){
        cout << "ERROR: Program needs a maximum of 2 arguments." << endl;
        cout << "       city_plan [input_file] [output_file]" << endl;
        return -1;
    }

    srand(time(NULL));
    
    drawLogo();

    bool exit = false;

    do{
        int option = menu();

        string fileName;
        if(argc > 1){
            fileName = argv[1];
        }else{
            fileName = chooseFileName();
        }

        InputInfo globalInfo = parseInput(fileName);

        switch(option){
            case 1:{
                /*HILL CLIMBING*/
                /* TODO
                    - booleano -> determinar se faz a pesquisa completa
                */
                State localMaximum = hillClimbing(&globalInfo);
                break;
            }
            case 2:{ 
                /*SIMULATED ANNEALING*/
                break;
            }
            case 3:{ 
                /*ALGORITMO GENÉTICO*/
                /* TODO
                    - tamanho da população
                    - probabilidade de mutação 
                    - número de gerações
                */
                break;
            }
            default:{
                exit = true;
                break;
            }
        }
    }while(!exit);

    
    return 0;
}