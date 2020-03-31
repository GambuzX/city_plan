#pragma once

#include <string>
#include <time.h>

#include "Genetic.h"

#define NUM_MAX_OPTIONS 5

void drawLogo();
int menu();

/*File Names*/
std::string chooseInputFileName();
std::string chooseOutputFileName();
bool fileExists(std::string fileName);

/*Hill Climbing & Simulated Annealing Parameters*/
bool chooseBestNeighbour();
int chooseMaxSteps();
double chooseMaxTemperature();

/*Genetic Algortihms Parameters*/
SelectionAlgorithm chooseSelectionAlgorithm();
BreedingAlgorithm chooseBreedingAlgorithm();
int choosePopulationSize();
int chooseGenerations();
double chooseMutationChance();
int chooseNP(const int &populationSize);

/*Performance*/
double countTime(clock_t startTime, clock_t endTime);
void printTime(std::string algorithm, double elapsedTime);
void printTime(std::pair<State, double> hcInfo, 
               std::pair<State, double> saInfo, 
               std::pair<State, double> gaInfo);