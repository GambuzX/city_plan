#pragma once

#include <string>

#include "Genetic.h"

#define NUM_MAX_OPTIONS 4

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