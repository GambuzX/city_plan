#pragma once

#include "State.h"

State geneticAlgorithm(InputInfo * globalInfo, int populationSize=100, int generations=10, double mutationChance=1.9, int np=3);