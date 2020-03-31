#pragma once

#include "State.h"

State hillClimbing(InputInfo * info);
State simulatedAnnealing(InputInfo * info, int maxSteps, double maxTemperature);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);