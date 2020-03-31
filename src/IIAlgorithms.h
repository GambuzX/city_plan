#pragma once

#include "State.h"

State hillClimbing(InputInfo * info, int steps = 100, bool findBestNeighbour = false);
State simulatedAnnealing(InputInfo * info, int maxSteps = 100, double maxTemperature = 10);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);