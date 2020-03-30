#pragma once

#include <string>

#include "Genetic.h"

void drawLogo();
int menu();

/*Input File Name*/
std::string chooseInputFileName();

/*Genetic Algortihms Parameters*/
SelectionAlgorithm chooseSelectionAlgorithm();
BreedingAlgorithm chooseBreedingAlgorithm();
int choosePopulationSize();
int chooseGenerations();
double chooseMutationChance();
int chooseNP(const int &populationSize);