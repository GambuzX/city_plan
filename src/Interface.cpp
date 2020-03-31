#include "Interface.h"

#include <iostream>
#include <sstream>
#include <experimental/filesystem>

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

int getIntOption(string display_string = " Option", string invalid_string = "Option"){
    int option;
    bool cinFailed;
    do
    {
        cout << display_string << ": ";
        cin >> option;
        cinFailed = cin.fail();
        if (cinFailed)
        {
            cin.clear();
            cin.ignore(100000, '\n');
            cout << endl << " Invalid " << invalid_string << "!";
        }
        cout << endl << endl;
    } while (cinFailed);

    return option;
}

double getDoubleOption(string display_string, string invalid_string){
    double option;
    bool cinFailed;
    do
    {
        cout << display_string << ": ";
        cin >> option;
        cinFailed = cin.fail();
        if (cinFailed)
        {
            cin.clear();
            cin.ignore(100000, '\n');
            cout << endl << " Invalid " << invalid_string << "!";
        }
        cout << endl << endl;
    } while (cinFailed);

    return option;
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
        
        option = getIntOption();
        
        if(option > NUM_MAX_OPTIONS || option < 1){
            cout << " Invalid option!" << endl << endl;    
        }

    }while(option < 1 || option > NUM_MAX_OPTIONS);

    return option;
}

bool fileExists(string fileName){
    if(experimental::filesystem::exists(fileName)){
        if(experimental::filesystem::is_regular_file(fileName)){
            return true;
        }
    }

    return false;
}

string chooseFileName(string oper){
    cout << " CHOOSE " << oper << " FILE NAME: ";
    
    string fileName;
    bool file_exists = false;
    if(oper == "OUTPUT")
        file_exists = true;
    
    do{
        cin >> fileName;
        
        if(oper == "INPUT")
            file_exists = fileExists(fileName);
        
        if(!file_exists){    
            cout << " File doesn't exist!" << endl << endl;
            cout << " CHOOSE " << oper << " FILE NAME: ";
        }
    }while(!file_exists);
    return fileName;
}

string chooseInputFileName(){
    return chooseFileName("INPUT");
}

string chooseOutputFileName(){
    return chooseFileName("OUTPUT");
}

bool chooseBestNeighbour(){
    cout << endl << " Do you want to find the best neighbour?" << endl;
        
    char bestNeighbourOption;

    cout << " [Y/N] (default is N): ";
    cin >> bestNeighbourOption;
    
    cin.clear();
    cin.ignore(100000, '\n');

    if (cin.fail())
    {
        cout << endl << " Invalid option!";
        cout << endl;
        return false;
    }
    

    if(toupper(bestNeighbourOption) == 'Y'){
        cout << endl;
        return true;
    }
    cout << " The algorithm won't find the best neighbour..." << endl << endl;
    return false;
}

int chooseMaxSteps(){
    int maxSteps = getIntOption(" Choose Maximum Number of Steps (> 0)", "Maximum Number of Steps (Must be an int)");

    if(maxSteps <= 0){
        cout << " Invalid number! The maximum number of steps will be 100..." << endl << endl;
        return 100;
    }

    return maxSteps;
}

double chooseMaxTemperature(){
    double temperature = getDoubleOption(" Choose the starting \"temperature\" (> 0)", "Temperature (Must be a double)");

    if(temperature <= 0){
        cout << " Invalid Temperature! A temperature of 10 will be used..." << endl << endl;
        return 10;
    }

    return temperature;
}

SelectionAlgorithm chooseSelectionAlgorithm(){
    cout << endl << " Choose the selection algorithm: " << endl;
    cout << " \t1: Tournament;" << endl;
    cout << " \t2: Roullete." << endl << endl;
    
    int option = getIntOption();

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

    int option = getIntOption();

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
    int populationSize = getIntOption(" Choose Population Size (> 0)", "Population Size (Must be an int)");

    if(populationSize <= 0){
        cout << " Invalid size! Initial population with 100 states will be used..." << endl << endl;
        return 100;
    }

    return populationSize;
}

int chooseGenerations(){
    int numGenerations = getIntOption(" Choose Number of Generations (> 0)", "Number of Generations (Must be an int)");

    if(numGenerations <= 0){
        cout << " Invalid number! 10 generations will be used..." << endl << endl;
        return 10;
    }

    return numGenerations;
}

double chooseMutationChance(){
    double mutationChance = getDoubleOption(" Choose Mutation Chance [0-100]", "Mutation Chance (Must be a double)");

    if(mutationChance <= 0 || mutationChance > 100){
        cout << " Invalid Mutation Chance! A chance of 5% will be used..." << endl << endl;
        return 5;
    }

    return mutationChance;
}

int chooseNP(const int &populationSize){
    ostringstream oss;
    oss << " Choose NP value [0-" << populationSize << "]";
    int np = getIntOption(oss.str(), "NP (Must be an int)");

    if(np <= 0){
        cout << " Invalid np! NP equals to 3 will be used..." << endl << endl;
        return 3;
    }else if(np > populationSize){
        cout << " Invalid np! NP equals to " << populationSize << " will be used..." << endl << endl;
        return 3;
    }

    return np;
}