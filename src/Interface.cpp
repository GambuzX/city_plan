#include "Interface.h"

#include <iostream>

using namespace std;

void drawLogo(){
    cout << endl;
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << " !       __   ___              _     _                     !" << endl;  
    cout << " !      |   |  |  \\/     .|   |_||  |_| |\\ |               !" << endl;
    cout << " !      |__ |  |  /      | |  |  |_ | | | \\|               !" << endl;
    cout << " !                       |'|            ._____             !" << endl;
    cout << " !               ___    |  |            |.   |' .---\"|     !" << endl;
    cout << " !       _    .-'   '-. |  |     .--'|  ||   | _|    |     !" << endl;
    cout << " !    .-'|  _.|  |    ||   '-__  |   |  |    ||      |     !" << endl;
    cout << " !    |' | |.    |    ||       | |   |  |    ||      |     !" << endl;
    cout << " ! ___|  '-'     '    \"\"       '-'   '-.'    '`      |____ !" << endl;
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
}

int menu(){
    cout << endl << " MAIN MENU" << endl;
    int option = -1;

    do{
        cout << " Choose an algorithm: " << endl;
        cout << " \t1: Hill Climbing;" << endl;
        cout << " \t2: Simulated Annealing;" << endl;
        cout << " \t3: Genetic Algorithm." << endl;
        cout << " \t4: Exit program." << endl <<  endl;
        
        cout << " Option: ";
        cin >> option;
        cout << endl << endl;
        
        if(option > NUM_MAX_OPTIONS || option < 1){
            cout << " Invalid option!" << endl << endl;    
        }

    }while(option < 1 || option > NUM_MAX_OPTIONS);

    return option;
}

string chooseFileName(string oper){
    cout << " CHOOSE " << oper << " OPTION: ";

    string fileName;
    cin >> fileName;
    
    return fileName;
}

string chooseInputFileName(){
    return chooseFileName("INPUT");
}

string chooseOutputFileName(){
    return chooseFileName("OUTPUT");
}

SelectionAlgorithm chooseSelectionAlgorithm(){
    cout << endl << " Choose the selection algorithm: " << endl;
    cout << " \t1: Tournament;" << endl;
    cout << " \t2: Roullete." << endl << endl;
    
    cout << " Option: ";
    int option;

    cin >> option;
    cout << endl;

    switch(option){
        case 1:
            return SelectionAlgorithm::Tournament;
        case 2: 
            return SelectionAlgorithm::Roullete;
    }

    cout << " Invalid option! Tournament Algorithm will be used..." << endl << endl;

    return SelectionAlgorithm::Tournament;
}

BreedingAlgorithm chooseBreedingAlgorithm(){
    cout << " Choose the breeding algorithm: " << endl;
    cout << " \t1: Horizontal Division;" << endl;
    cout << " \t2: Vertical Division;" << endl;
    cout << " \t3: Selection In Turns." << endl << endl;

    cout << " Option: ";
    int option;

    cin >> option;
    cout << endl;

    switch(option){
        case 1:
            return BreedingAlgorithm::HorizontalDivision;
        case 2: 
            return BreedingAlgorithm::VerticalDivision;
        case 3: 
            return BreedingAlgorithm::SelectionInTurns;
    }

    cout << " Invalid option! Vertical Division will be used..." << endl << endl;

    return BreedingAlgorithm::VerticalDivision;
}

int choosePopulationSize(){
    int populationSize;

    cout << " Choose Population Size (> 0): ";
    cin >> populationSize;
    cout << endl;

    if(populationSize <= 0){
        cout << " Invalid size! Initial population with 100 states will be used..." << endl << endl;
        return 100;
    }

    return populationSize;
}

int chooseGenerations(){
    int numGenerations;

    cout << " Choose Number of Generations (> 0): ";
    cin >> numGenerations;
    cout << endl;
    if(numGenerations <= 0){
        cout << " Invalid number! 10 generations will be used..." << endl << endl;
        return 10;
    }

    return numGenerations;
}

double chooseMutationChance(){
    double mutationChance;

    cout << " Choose Mutation Chance [0-100]: ";
    cin >> mutationChance;
    cout << endl;
    if(mutationChance <= 0 || mutationChance > 100){
        cout << " Invalid mutation chance! A chance of 5% will be used..." << endl << endl;
        return 5;
    }

    return mutationChance;
}

int chooseNP(const int &populationSize){
    int np;

    cout << " Choose NP value [0-" << populationSize << "]: ";
    cin >> np;
    cout << endl;
    if(np <= 0){
        cout << " Invalid np! NP equals to 3 will be used..." << endl << endl;
        return 3;
    }else if(np > populationSize){
        cout << " Invalid np! NP equals to " << populationSize << " will be used..." << endl << endl;
        return 3;
    }

    return np;
}