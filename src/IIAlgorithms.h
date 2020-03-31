#pragma once

#include "State.h"

State hillClimbing(InputInfo * info, int steps = 100, bool findBestNeighbour=false);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);