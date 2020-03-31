#pragma once

#include "State.h"

State hillClimbing(InputInfo * info, bool findBestNeighbour=false);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);