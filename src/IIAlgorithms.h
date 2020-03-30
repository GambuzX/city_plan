#pragma once

#include "State.h"

State hillClimbing(InputInfo * info);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);