#pragma once

#include "State.h"

enum SelectionAlgorithm {
    Tournament,
    Roullete
};

enum BreedingAlgorithm {
    HorizontalDivision,
    VerticalDivision
};

State geneticAlgorithm(InputInfo * globalInfo, 
                       SelectionAlgorithm selecAlg = SelectionAlgorithm::Tournament, 
                       BreedingAlgorithm breedingAlg = BreedingAlgorithm::VerticalDivision, 
                       int populationSize = 100, 
                       int generations = 10, 
                       double mutationChance = 5, 
                       int np = 3
                       );