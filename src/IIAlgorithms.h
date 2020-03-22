#pragma once

#include "State.h"

State hillClimbing(const State & initialState);
State simulatedAnnealing(const State & initialState);
State tabuSearch(const State & initialState);

State higherValueNeighbour(const State & state, bool findBest);