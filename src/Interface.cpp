#include "Interface.h"

#include <iostream>

#define NUM_MAX_OPTIONS 4

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
    cout << " MAIN MENU" << endl;
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

string chooseFileName(){
    cout << " CHOOSE INPUT OPTION: ";

    string fileName;
    cin >> fileName;
    
    return fileName;
}