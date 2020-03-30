#pragma once

#include "State.h"

State hillClimbing(InputInfo * info);
State simulatedAnnealing(InputInfo * info, int maxSteps, int temperature);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);